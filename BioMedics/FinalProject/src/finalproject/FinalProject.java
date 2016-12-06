/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package finalproject;

import java.util.*;
import java.io.*;
import java.lang.*;

/**
 *
 * @author cvalle
 */
public class FinalProject {

    final static String fileLocation = "E:/School/2016Fall/Homework/BioMedics/FinalProject";
    
    protected String[] getFileTokens(String filepath, String split)
    {
        StringBuilder sb = null;
        
        try {
            InputStream is = new FileInputStream(filepath);
            BufferedReader buf = new BufferedReader(new InputStreamReader(is));

            String line = buf.readLine();
            sb = new StringBuilder();

            while(line != null){
               sb.append(line).append("\n");
               line = buf.readLine();
            }
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
        if (sb != null) {
            return sb.toString().trim().split(split);
        }
        else return null;
    }
    
    protected List<IwStructure> getIw(String[] Q, List<String> Sentences, Set<String> C)
    {
        List<IwStructure> Iw = new ArrayList<>();
        Iterator<String> iterator = C.iterator();
        while(iterator.hasNext()) {
            String concept = iterator.next();
            double fwq = 0, fcj = 0, fwqcj = 0;
            
            for (String sentence: Sentences) {
                int cInS = 0;
                for (String q: Q) {
                    for (String s: sentence.split(" ")) {
                        if (q.equals(s.trim())) {
                            cInS++;
                            break;
                        }
                    }
                }
                
                boolean cInSent = false;
                for (String s: sentence.split(" ")) {
                    if (concept.equals(s.trim())) {
                        cInSent = true;
                        break;
                    }
                }
                double partialCNT = (double)cInS / Q.length;
                
                fwq += partialCNT;
                fcj += cInSent ? 1 : 0;
                fwqcj += cInSent ? partialCNT : 0;
            }
            Double iwcj = (Sentences.size() * fwqcj) / (fwq * fcj);
            Iw.add(new IwStructure(concept, iwcj));
        }
        
        Collections.sort(Iw);
        Collections.reverse(Iw);
        
        return Iw;
    }
    
    protected void runProgram(String str) {
        String[] Query = getFileTokens(fileLocation + "/Topic_Data_Set_" + str + "/topic_" + str + ".txt", " ");
        String[] Z = getFileTokens(fileLocation + "/Topic_Data_Set_" + str + "/goldstandard_" + str + ".txt", ", ");
        
        // Get all sentences and concepts in Z
        List<String> Sentences = new ArrayList<>();
        Set<String> uniqueCinZ = Collections.synchronizedSet(new HashSet<String>());
        for (String z: Z) {
            String[] sentencesPerFile = getFileTokens(fileLocation + "/Topic_Data_Set_" + str + "/CUI_Files_" + str + "/" + z, "\n ");
            for (String s: sentencesPerFile) {
                if (s.trim().length() == 0) continue;
                Sentences.add(s.trim());
                for (String c: s.trim().split(" "))
                    uniqueCinZ.add(c.trim());
            }
        }
        
        System.out.println(uniqueCinZ.size());
        
        // Calculate Iw
        List<IwStructure> Iw;
        boolean createIw = false;
        
        if (createIw) {
            Iw = getIw(Query, Sentences, uniqueCinZ);
            try {
                FileOutputStream fos = new FileOutputStream(fileLocation + "/Topic_Data_Set_" + str + "/DocumentIw_" + str + ".bin");
                try (ObjectOutputStream oos = new ObjectOutputStream(fos)) {
                    oos.writeObject(Iw);
                }
            } catch (IOException e) {
                System.out.println("Error! " + e);
            }
            return;
        }
        else {
            try {
                FileInputStream fis = new FileInputStream(fileLocation + "/Topic_Data_Set_" + str + "/DocumentIw_" + str + ".bin");
                try (ObjectInputStream ois = new ObjectInputStream(fis)) {
                    Iw = (List<IwStructure>) ois.readObject();
                }
            }
            catch (IOException | ClassNotFoundException e) {
                System.out.println("Error! " + e);
                return;
            }
        }
        
        for (String z: Z) {
            String[] sentencesPerFile = getFileTokens(fileLocation + "/Topic_Data_Set_" + str + "/CUI_Files_" + str + "/" + z, "\n ");
            Set<String> uniqueCinFile = Collections.synchronizedSet(new HashSet<String>());
            for (String s: sentencesPerFile) {
                if (s.trim().length() == 0) continue;
                Sentences.add(s.trim());
                for (String c: s.trim().split(" "))
                    uniqueCinFile.add(c.trim());
            }
            
            // Develop the Iw for this document
            
        }
        
        for (int i = 0; i < 100; ++i)
            System.out.println(Iw.get(i).concept + " " + Iw.get(i).weight);
    }
        
    public static void main(String[] args) {
        FinalProject p1 = new FinalProject();
        
        p1.runProgram("I");
    }
    /*
    
    protected StringTokenizer getFileTokens(String filepath)
    {
        StringBuilder sb = null;
        
        try {
            InputStream is = new FileInputStream(filepath);
            BufferedReader buf = new BufferedReader(new InputStreamReader(is));

            String line = buf.readLine();
            sb = new StringBuilder();

            while(line != null){
               sb.append(line).append("\n");
               line = buf.readLine();
            }
        } catch (Exception e) {
            System.out.println("Error: " + e);
        }
        if (sb != null)
            return new StringTokenizer(sb.toString());
        else return null;
    }
    
    public static void main(String[] args) {
        
        FinalProject p1 = new FinalProject();
        Scanner scan = new Scanner(System.in);
        
        //File query = new File(fileLocation + "/query.txt");
        //File docfile = new File(fileLocation + "/fileweights.log");
        File goldstandard = new File(fileLocation + "/goldstandard.txt");
        
        ArrayList<IwStructure> docs_and_weights = new ArrayList<IwStructure>();
        float P10 = 0;
        float P20 = 0;
        float avep = 0;
        
        while(true){
        System.out.println("1: Calculate and Output Document Weights topic 1");
        System.out.println("2: Calculate and Output Document Weights topic 2");
        System.out.println("4: Get P@10 (You must read the document weight from file first)");
        System.out.println("5: Get P@20 (You must read the document weight from file first)");
        System.out.println("6: Get AveP (You must read the document weight from file first)");
        System.out.println("7: Exit");
        System.out.print("Please choose: ");
        
        String choice = "";
        choice = scan.nextLine();
        System.out.print("\n");
        
        if(Objects.equals(choice,"1")){
            StringTokenizer topics = p1.getFileTokens(fileLocation + "/Topic_Data_Set_I" + "/topic_I.txt");
            File dir = new File(fileLocation + "/Topic_Data_Set_I/CUI_Files_I/");
            
            Map<String, Map<String,Double> > Weights = new HashMap<>();

            while (topics.hasMoreTokens()){
                String curTopic = topics.nextToken();

                int topicOccurences = 0;
                int totalDocs = 0;
                for(File i : dir.listFiles()){
                    topicOccurences += p1.get_one_IDF_count(i, curTopic);
                    totalDocs++;
                }
                double IDF = 0;
                if (topicOccurences != 0)
                    IDF = Math.log10((double)totalDocs / topicOccurences); 
                
                for(File i : dir.listFiles()){
                    float TF = p1.getTF(i, curTopic);
                    
                    double weight = TF * IDF;
                    
                    Map<String,Double> in = new HashMap<>();
                    in.put(curTopic, weight);
                    
                    Weights.put(i.getName(), in);
                }
            }
            
            for(Map.Entry<String, Map<String,Double> > entry: Weights.entrySet()) {
                Double totalWeight = 0.0;
                for(Map.Entry<String,Double> entry2: entry.getValue().entrySet()) {
                    totalWeight += entry2.getValue();
                }
                docs_and_weights.add(new IwStructure(entry.getKey(), totalWeight));
            }
            Collections.sort(docs_and_weights);
            Collections.reverse(docs_and_weights);
            
            ListIterator litr = docs_and_weights.listIterator();
            for (int i = 0; i < 10; i++) {
                IwStructure struct = (IwStructure)litr.next();
                System.out.println(struct.concept + ": " + struct.weight);
            }
            
            
            System.out.println(p1.getP10(docs_and_weights, new File(fileLocation + "/Topic_Data_Set_I/goldstandard_I.txt")));
            System.out.println(p1.getP20(docs_and_weights, new File(fileLocation + "/Topic_Data_Set_I/goldstandard_I.txt")));
            System.out.println(p1.getAveP(docs_and_weights, new File(fileLocation + "/Topic_Data_Set_I/goldstandard_I.txt")));
            break;
        }
        else if(Objects.equals(choice,"2")){
            File dir = new File(fileLocation + "/CUI_Files_II/");
            
            
            for(File i : dir.listFiles()){
                try{
                p1.convertMmFileToCuiFiles(i.getPath(), fileLocation + "/part1files/"+i.getName());
                }catch(IOException e){}
            }
            
            //p1.calcWeights(query);  
        }
        
        else if(Objects.equals(choice,"3")){
            //docs_and_weights=p1.getWeights(docfile);
            System.out.println("--> Successfully Read Document Weights\n");
        }
        
        else if(Objects.equals(choice,"4")){
            P10 = p1.getP10(docs_and_weights, goldstandard);
            System.out.println("--> P@10 = "+P10);
            System.out.println(" ");
        }
        else if(Objects.equals(choice,"5")){
            P20 = p1.getP20(docs_and_weights, goldstandard);
            System.out.println("--> P@20 = "+P20);
            System.out.println(" ");
        }
        else if(Objects.equals(choice,"7")){
            System.out.println("--> Thank You");
            System.out.println(" ");
            break;
        }
        else if(Objects.equals(choice,"6")){
            avep = p1.getAveP(docs_and_weights, goldstandard);
            System.out.println("--> AveP = "+avep);
            System.out.println(" ");
        }
        else{
            System.out.println("--> Invalid input, try again");
            System.out.println("\n");  
        }
        }
        
    }
    public float getP10(ArrayList<IwStructure> docs, File goldstandard){
         float P10 = 0;
         float count = 0;
         float relevant = 0;
         try(
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(goldstandard)));){
            String line = null;
            String tokens [];
            while( (line = br.readLine())!= null ){
                tokens = line.split(", ");
                
                ListIterator litr = docs.listIterator();
                while(count<10){  
                    IwStructure struct = (IwStructure)litr.next();
                    
                    for(String i : tokens){
                        if(i.equals(struct.concept)){
                            relevant++;
                        }
                    }
                    count++;

                }
            }
         }catch(IOException e){}
         if(count!=0){
             P10 = relevant/count;
         }
         
         return P10;
    }
    public float getP20(ArrayList<IwStructure> docs, File goldstandard){
         float P20 = 0;
         float count = 0;
         float relevant = 0;
         ArrayList<IwStructure> info = new ArrayList<IwStructure>();
         try(
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(goldstandard)));){
            String line = null;
            String tokens [];
            while( (line = br.readLine())!= null ){
                tokens = line.split(", ");
                
                ListIterator litr = docs.listIterator();
                while(count<20){  
                    IwStructure struct = (IwStructure)litr.next();
                    for(String i : tokens){
                        if(Objects.equals(i,struct.concept)){
                            relevant++;
                        }
                    }
                    count++;

                }
            }
         }catch(IOException e){}
         if(count!=0){
             P20 = relevant/count;
         }
         
         return P20;
    }
    public float getAveP(ArrayList<IwStructure> docs, File goldstandard){
         float avep = 0;
         float count = 0;
         float relevant = 0;
         ArrayList<IwStructure> info = new ArrayList<IwStructure>();
         try(
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(goldstandard)));){
            String line = null;
            String tokens [];
            while( (line = br.readLine())!= null ){
                tokens = line.split(", ");
                
                ListIterator litr = docs.listIterator();
                while(litr.hasNext()){  
                    count++;
                    IwStructure struct = (IwStructure)litr.next();
                    for(String i : tokens){
                        if(Objects.equals(i,struct.concept)){
                            relevant++;
                            avep += relevant/count;
                        }
                    }
                    

                }
            }
         }catch(IOException e){}
         
         avep = avep/relevant;
         return avep;
    }
    public ArrayList<IwStructure> getWeights(File docfile){
        ArrayList<IwStructure> info = new ArrayList<IwStructure>();
        try(
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(docfile)));){
            String line = null;
            String docname = new String("");
            String weightstring = new String("");
            double weightnum = 0;
            while( (line = br.readLine())!= null ){
                int i =0;
                while(i<line.length()&& !Character.isDigit(line.charAt(i)))i++;
                int j = i;
                while(j<line.length()&&Character.isDigit(line.charAt(j)))j++;
                docname = line.substring(i,(j+4));
                
                i = j+4;
                while(i<line.length()&&!Character.isDigit(line.charAt(i)))i++;
                j = i+2;
                while(j<line.length()&&Character.isDigit(line.charAt(j)))j++;
                
                
                if(Objects.equals(line.charAt(j),'E')){
                    weightstring = line.substring(i,j+3);
                }
                else{
                    weightstring = line.substring(i,j);
                }
                weightnum = Double.valueOf(weightstring);
                IwStructure ds = new IwStructure(docname, weightnum);
                info.add(ds);
                
            }
            Collections.sort(info);
            Collections.reverse(info);
            
            
        }catch(IOException e){}
                
        
        return info;
    }   
    public void calcWeights(File query){
        System.out.println("Scanning query file at: "+query.getPath());
        try(
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(query)));){
            String line = null;
            
            while( (line = br.readLine())!= null ){
                String[] tokens, old_tokens;
                old_tokens = line.split(" ");
                tokens = new String[old_tokens.length-1];
                
                for(int i=0; i<old_tokens.length-1; i++){
                    tokens[i] = old_tokens[i+1];
                }
                
                System.out.println("Printing Query Tokens...");
                for(String i : tokens){
                    System.out.println(i);
                }
                
                System.out.println("\n");
                
                File dir = new File(fileLocation + "/part1files");
                ArrayList<IwStructure> tfs = new ArrayList<IwStructure>();
                
                double total = 0;
                total = dir.listFiles().length;
                System.out.println("total = "+total);
                
                for(File file : dir.listFiles()){
                    double curWeight = 0;
                    for(String i : tokens){
                        double curTF = 0;
                        double curIDF = 0;
                        double TF_IDF = 0;
                        
                        //get the current TF value for this CUI in this document
                        curTF = getTF(file, i);
                        System.out.println("TF for "+i+": "+curTF);
                        //get IDF for the current CUI
                        for(File f : dir.listFiles()){
                            //curIDF returns 0 if not in the file, else 1
                            curIDF += get_one_IDF_count(f, i);
                        }
                        //calculate the IDF for the current CUI
                        System.out.println("IDF count for "+i+": "+curIDF);
                        if(curIDF!=0){
                            curIDF = Math.log10(total/curIDF);
                        }
                        System.out.println("Calculated IDF for "+i+": "+curIDF);
                        //sum all the TF-IDF values for current document
                        TF_IDF = curTF*curIDF;
                        System.out.println("TF-IDF for "+i+": "+TF_IDF);
                        curWeight += TF_IDF;
                        System.out.println("Weight for "+getFile(file)+" after checking "+i+" : "+curWeight);
                    }
                    System.out.println("Total weight for document "+getFile(file)+" : "+curWeight+"\n");
                    
                    //output the document name and its weight to fileweights.log
                    try(FileWriter fw = new FileWriter("fileweights.log", true);
                                    BufferedWriter bw = new BufferedWriter(fw);
                                    PrintWriter out = new PrintWriter(bw))
                                {
                                    out.print("File: "+file.getName());
                                    out.println("  Weight: "+curWeight+" ");
                                } catch (IOException e) {}
                }
                
            }
        }catch(IOException e){}
        
    }
    public float get_one_IDF_count(File file, String query){
        
        
        try(BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(file)));)
         {
            
            String line = null;
            
            while( (line = br.readLine())!= null ){
                String tokens [];
                tokens = line.split(" ");
                
                for(String i : tokens){
                  if((!(Objects.equals(i,"")))&&(!(Objects.equals(i," ")))){
                    if(Objects.equals(i, query)){
                        
                        return 1;
                    }   
                  }
                  
                }
            }
        }catch(IOException e){}
        
        return 0;
    }
    public float getTF(File file, String query){
        float total = 0;
        float query_total = 0;
         try(BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(file)));)
         {
            
            String line = null;
            
            while( (line = br.readLine())!= null ){
                String tokens [];
                tokens = line.split(" ");
                
                for(String i : tokens){
                  if((!(Objects.equals(i,"")))&&(!(Objects.equals(i," ")))){
                    total++;
                    
                    if(Objects.equals(i, query)){
                        query_total++;
                    }   
                  }
                  
                }
                
            }
        }catch(IOException e){}
        
        return (query_total)/(total);
    }
    
public void convertMmFileToCuiFiles(String metaMapFilePath, String cuiFilePath) throws
IOException
 {
    BufferedReader br = new BufferedReader(new InputStreamReader(new
        FileInputStream(metaMapFilePath)));
    File cuiFile = new File(cuiFilePath);
    PrintWriter out = new PrintWriter(cuiFile);
    String cui = null;
    String line = null;
    boolean sentenceReadFlag = false;
        try{
        while (true)
        {
            if(!sentenceReadFlag)
                line = br.readLine();
            if (line == null) // end of file
            {
                break;
            } else if (line.trim().length() == 0)
            {
                continue;
            } else
            {
              StringTokenizer st = new StringTokenizer(line);
              String token = st.nextToken();
            if(token.equals("Processing")) //output new line after processing every line
            {
                if(cui != null)
                    out.print("\n");
                line = br.readLine();
                sentenceReadFlag = false;
                if(line.trim().length() == 0)
                    continue;
                st = new StringTokenizer(line);
                token = st.nextToken();
            }
            if (token.equals("Phrase:"))
            {
                line = br.readLine(); //read next line after the Phrase line
                if(line.startsWith("Processing"))//it's done for one sentence
                {
                    sentenceReadFlag = true;
                    continue;
                }
            else
                sentenceReadFlag = false;
            if(line.trim().length() != 0)
            {
                while(true){ // read multiple CUIs
                    line = br.readLine();
                    if(line.trim().length() == 0)//it's done for one phrase
                        break;
                    if(line.startsWith("Processing"))//it's done for one sentence
                    {
                        sentenceReadFlag = true;
                        break;
                    }
                    if(line.contains(":")) //valid lines containing a CUI
                    {
                        st = new StringTokenizer(line);
                        token = st.nextToken();
                        if (Integer.parseInt(token)<=1000 && Integer.parseInt(token)>=700)
                        {
                            token = st.nextToken();
                            if(token.startsWith("C")){
                                StringTokenizer st1 = new StringTokenizer(token,":");
                                cui = st1.nextToken();
                                out.print(cui.replace('C', ' ')); // obtain CUI
                            }
                            else{break;} // exclude "E" lines
                        }
                    }
                    else
                        continue;
                }
            } else { continue; }
            }
            }  
        }
        } catch (Exception e){
        System.out.println("Exception: " + e.getMessage());
        }
        out.close();
        br.close();
}

    public String getFile(File file){
        String [] tokens;
        String path = file.getPath();
        
        if(path.contains("MetaMapFiles")){
            tokens = path.split("MetaMapFiles");
        }
        else if(path.contains("part1files")){
            tokens = path.split("part1files");
        }
        else{
            tokens = new String[5];
        }
        return tokens[1].substring(1);
    }
    */
}
