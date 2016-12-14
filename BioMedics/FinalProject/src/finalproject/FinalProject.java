/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package finalproject;

import java.util.*;
import java.io.*;

/**
 *
 * @author cvalle
 */
public class FinalProject {

    final static String fileLocation = "E:/School/2016Fall/Homework/BioMedics/FinalProject/";
    
    protected List<String> getFileTokens(String filepath, String split)
    {
        StringBuilder sb = null;
        
        try {
            InputStream is = new FileInputStream(filepath);
            BufferedReader buf = new BufferedReader(new InputStreamReader(is));

            String line = buf.readLine();
            sb = new StringBuilder();

            while(line != null){
                line = line.trim();
                if (!line.isEmpty())
                    sb.append(line).append("\n");
                line = buf.readLine();
            }
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
        if (sb != null) {
            String[] lines = sb.toString().split(split);
            List<String> tokens = new ArrayList<>();
            for (String s: lines) {
                tokens.add(s.trim());
            }
            return tokens;
        }
        else return null;
    }
    
    protected List<IwStructure> getIw(List<String> query, List<String> sentencesInZ, Set<String> conceptsInZ) {
        // The return object
        List<IwStructure> Iw = new ArrayList<>();
        
        // topicsInSentence.get("sentence") is the number of topics that are in the sentence
        Map<String, Integer> topicsInSentence = new HashMap<>();
        sentencesInZ.stream().forEach((sentence) -> {
            
            int count = 0;
            for (String topic: query)
                if (sentence.contains(topic))
                    ++count;
            
            topicsInSentence.put(sentence, count);
        });
        
        // Calculate weight for every concept
        conceptsInZ.stream().forEach((concept) -> {
            double fwq = 0, fcj = 0, fwqcj = 0;
            for (String sentence: sentencesInZ) {
                boolean cInSent = sentence.contains(concept);
                double partialCNT = (double)topicsInSentence.get(sentence) / query.size();
                
                fwq += partialCNT;
                fcj += cInSent ? 1 : 0;
                fwqcj += cInSent ? partialCNT : 0;
            }
            Double iwcj = (fwqcj * sentencesInZ.size()) / (fwq * fcj);
            Iw.add(new IwStructure(concept, iwcj));
        });
        
        // Sort and return
        Collections.sort(Iw);
        Collections.reverse(Iw);
        return Iw;
    }
    
    protected int getTopicOccurences(String topic, Map<String, List<String> > sentencesInFile) {
        // Count how many documents the topic occurs in
        int count = 0;
        
        for (String fileName: sentencesInFile.keySet()) {
            boolean found = false;
            for (String sentence: sentencesInFile.get(fileName)) {
                for (String concept: sentence.split(" ")) {
                    if (topic.equals(concept.trim())) {
                        ++count;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
        }
        
        // Return that number
        return count;
    }
    
    protected double getTF(String topic, List<String> sentences) {
        // Count the number of times this concept is found in this document
        //  and count the number of concepts in this document
        int hits = 0, concepts = 0;
        for (String sentence: sentences) {
            for (String concept: sentence.split(" ")) {
                ++concepts;
                if (topic.equals(concept)) {
                    ++hits;
                }
            }
        }
        
        // Return the ratio of occurences to the number of concepts
        return (double)hits / concepts;
    }
    
    protected List<IwStructure> calculateATFIDF(List<String> Query, Map<String, List<String> > sentencesInFile) {
        // Calculate the IDF for every topic
        Map<String,Double> IDFMap = new HashMap<>();
        Query.stream().forEach((topic) -> {
            int topicOccurences = getTopicOccurences(topic, sentencesInFile);
            int totalDocs = sentencesInFile.keySet().size();
                
            double IDF = 0;
            if (topicOccurences != 0)
                IDF = Math.log10((double)totalDocs / topicOccurences);
            
            IDFMap.put(topic, IDF);
        });
        
        // Calculate the ATFIDF for every file
        Set<String> fileNames = sentencesInFile.keySet();
        Map<String,Double> store = new HashMap<>();
        fileNames.stream().forEach((fileName) -> {
            store.put(fileName, new Double(0));
        });
        
        fileNames.stream().forEach((fileName) -> {
            Query.stream().forEach((curTopic) -> {
                double TF = getTF(curTopic, sentencesInFile.get(fileName));
                Double IDF = IDFMap.get(curTopic);
                store.put(fileName, store.get(fileName) + TF * IDF);
            });
        });
        
        // Return object
        List<IwStructure> ATFIDF = new ArrayList<>();
        
        // Convert the store map to a list of IwStructures
        store.entrySet().stream().forEach((entry) -> {
            ATFIDF.add(new IwStructure(entry.getKey(), entry.getValue()));
        });
        
        // Sort in descending order
        Collections.sort(ATFIDF);
        Collections.reverse(ATFIDF);
        
        return ATFIDF;
    }
    
    protected Set<String> getUniqueConceptsInFiles(List<String> fileList, Map<String, List<String> > fileSentences) {
        // Create return variable
        Set<String> uniqueConcepts = new HashSet<>();
        
        // For every file, for every sentence in that file, add each concept to the set
        fileList.stream().forEach((String z) -> {
            fileSentences.get(z).stream().forEach((s) -> {
                for (String c: s.split(" "))
                    uniqueConcepts.add(c.trim()); // Add the concept to the set
            });
        });
        
        // Return it
        return uniqueConcepts;
    }
    
    protected List<String> getSentencesInFiles(List<String> fileList, Map<String, List<String> > fileSentences) {
        // Create return variable
        List<String> sentences = new ArrayList<>();
        
        // For every file, for every sentence in that file, add each concept to the set
        fileList.stream().forEach((z) -> {
            fileSentences.get(z).stream().forEach((s) -> {
                sentences.add(s.trim()); // Add the concept to the set
            });
        });
        
        // Return it
        return sentences;
    }
    
    protected double calculateP10(List<String> Z, List<IwStructure> ranking) {
        // Count of number of relevant and total
        int relevant = 0;
        int total = 0;
        
        // Iterate through ranked or top ten
        ListIterator litr = ranking.listIterator();
        for (int i = 0; i < 10 && litr.hasNext(); i++) {
            ++total; // Increase total
            // Increment relevant if found
            String ranked = ((IwStructure)litr.next()).concept;
            if (Z.contains(ranked))
                ++relevant;
        }
        
        return (double)relevant / total;
    }
    
    protected double calculateAveP(List<String> Z, List<IwStructure> ranking) {
        int relevant = 0;
        int total = 0;
        double sum = 0;
        
        ListIterator litr = ranking.listIterator();
        for (int i = 0; i < 10 && litr.hasNext(); i++) {
            ++total;
            String ranked = ((IwStructure)litr.next()).concept;
            if (Z.contains(ranked))
            {
                ++relevant;
                sum += (double)relevant / total;
            }
        }
        
        return (double)sum / relevant;
    }
    
    protected double calculateRBO(List<String> kProfile, List<IwStructure> docProfile, int num, double phi) {
        // Set up return
        double ret = 0.0;

        // We need to iterate through the profiles
        ListIterator kProfItr = kProfile.listIterator();
        ListIterator docProfItr = docProfile.listIterator();
        
        // Holds the concepts at the current depth
        Set<String> kConcepts = new HashSet<>();
        Set<String> docConcepts = new HashSet<>();
         
        // For the top concepts
        for (int i = 0; i < num; ++i) {
            kConcepts.add((String)kProfItr.next());
            docConcepts.add(((IwStructure)docProfItr.next()).concept);
            
            // Get the intersection of the two sets
            boolean greater = kConcepts.size() > docConcepts.size();
            Set<String> intersection = new HashSet<>(greater ? docConcepts : kConcepts);
            intersection.retainAll(greater ? kConcepts : docConcepts);
            
            // Add to the return
            ret += Math.pow(phi, i) * (double)intersection.size() / (i + 1);
        }
        
        // Normalize and return
        return ret * (1. - phi);
    }
    
    public double runProgram(String str) {
        // Report which topic this is
        System.out.println("For topic: " + str);
        
        // The user query
        List<String> Query = getFileTokens(fileLocation + str + "/topic.txt", " ");
        // The goldstandard files
        List<String> Z = getFileTokens(fileLocation + str + "/goldstandard.txt", ",");
        
        // All sentences in each file
        Map<String, List<String> > sentencesInFiles = new HashMap<>();
        // All file names
        List<String> allFiles = new ArrayList<>();
        
        // Fill sentencesInFiles and allFiles
        File dir = new File(fileLocation + str + "/CUI_Files/");
        for(File i : dir.listFiles()){
            allFiles.add(i.getName());
            List<String> sentences = getFileTokens(i.getPath(), "\n");
            sentencesInFiles.put(i.getName(), sentences);
        }
        
        // Calculate ATFIDF for the query
        List<IwStructure> ATFIDF = calculateATFIDF(Query, sentencesInFiles);
        
        // Get all sentences and concepts in Z
        List<String> sentencesInZ = getSentencesInFiles(Z, sentencesInFiles);
        Set<String> conceptsInZ = getUniqueConceptsInFiles(Z, sentencesInFiles);

        // Calculate Iws
        List<IwStructure> Iw = getIw(Query, sentencesInZ, conceptsInZ);
        
        // Calculate precision
        double pAt10 = calculateP10(Z, ATFIDF);
        double aveP = calculateAveP(Z, ATFIDF);
        System.out.println(" Round: 1");
        System.out.println("   P@10: " + pAt10);
        System.out.println("   AveP: " + aveP);
        
        /** Second round **/
        int RBOconcepts = 80; // Default
        final double RBOphi = 0.875;
        
        // Will hold all Iw values for every document
        Map<String, List<IwStructure> > documentProfiles = new HashMap<>();
        for (String fileName: allFiles) {
            // Get necessary data
            Set<String> conceptsInDocument = new HashSet<>();
            sentencesInFiles.get(fileName).stream().forEach((s) -> {
                for (String c: s.split(" "))
                    conceptsInDocument.add(c.trim()); // Add the concept to the set
            });

            // Calculate Iws
            List<IwStructure> documentIws = getIw(Query, sentencesInFiles.get(fileName), conceptsInDocument);
            
            // Maximize the number of concepts to use for calculating RBOs
            RBOconcepts = Math.min(RBOconcepts, documentIws.size());

            // Add to profiles list
            documentProfiles.put(fileName, documentIws);
        }
        
        // Calculate kProfile
        List<String> kProfile = new ArrayList<>();
        
        // Top ranked concepts
        int RBOconceptsMax = 0;
        ListIterator PZitr = Iw.listIterator();
        for (; RBOconceptsMax < RBOconcepts && PZitr.hasNext(); ++RBOconceptsMax)
            kProfile.add(((IwStructure)PZitr.next()).concept);
        
        // Maximize the number of concepts to use for calculating RBOs
        RBOconcepts = Math.min(RBOconcepts, RBOconceptsMax);
        
        // Calculate RBO values
        List<IwStructure> docRBOs = new ArrayList<>();
        for (String fileName: allFiles) {
            // Get the document profile
            List<IwStructure> thisDoc = documentProfiles.get(fileName);
            // Calculate RBO
            Double RBO = calculateRBO(kProfile, thisDoc, RBOconcepts, RBOphi);
            // Add to docRBOs
            docRBOs.add(new IwStructure(fileName, RBO));
        }
        
        // Sort in descending order
        Collections.sort(docRBOs);
        Collections.reverse(docRBOs);
        
        // Calculate precision
        pAt10 = calculateP10(Z, docRBOs);
        aveP = calculateAveP(Z, docRBOs);
        System.out.println(" Round: 2");
        System.out.println("   P@10: " + pAt10);
        System.out.println("   AveP: " + aveP);
        
        /** Third Round **/

        // Get all the relevant documents ranked in the top ten
        List<String> relevantDocuments = new ArrayList<>();
        ListIterator litr = docRBOs.listIterator();
        for (int i = 0; i < 10; i++) {
            String doc = ((IwStructure)litr.next()).concept;
            if (Z.contains(doc))
                relevantDocuments.add(doc);
        }
        
        // Concepts and sentences in relevantDocuments
        List<String> sentencesInDocument = getSentencesInFiles(relevantDocuments, sentencesInFiles);
        Set<String> conceptsInDocument = getUniqueConceptsInFiles(relevantDocuments, sentencesInFiles);
        // Calculate Iws
        List<IwStructure> topConcepts = getIw(Query, sentencesInDocument, conceptsInDocument);
        
        // Calculate new kProfile
       kProfile = new ArrayList<>();
        
        // Top ranked concepts in the new Z
        PZitr = topConcepts.listIterator();
        for (int i = 0; i < RBOconcepts && PZitr.hasNext(); ++i)
            kProfile.add(((IwStructure)PZitr.next()).concept);
        
        // Calculate RBO values
        docRBOs = new ArrayList<>();
        for (String fileName: allFiles) {
            // Get the document profile
            List<IwStructure> thisDoc = documentProfiles.get(fileName);
            // Calculate RBO
            Double RBO = calculateRBO(kProfile, thisDoc, RBOconcepts, RBOphi);
            // Add to docRBOs
            docRBOs.add(new IwStructure(fileName, RBO));
        }
        
        // Sort in descending order
        Collections.sort(docRBOs);
        Collections.reverse(docRBOs);
        
        // Calculate precision
        pAt10 = calculateP10(Z, docRBOs);
        aveP = calculateAveP(Z, docRBOs);
        System.out.println(" Round: 3");
        System.out.println("   P@10: " + pAt10);
        System.out.println("   AveP: " + aveP);
        
        return aveP;
    }
        
    public static void main(String[] args) {
        FinalProject p1 = new FinalProject();
        
        double aveP1 = p1.runProgram("Topic_Data_Set_I");
        double aveP2 = p1.runProgram("Topic_Data_Set_II");
        
        System.out.println("MAP: " + ((aveP1 + aveP2) / 2));
    }
}
