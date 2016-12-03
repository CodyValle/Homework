

package finalproject;

public class DocDataStructure implements Comparable<DocDataStructure>{
    String document;
    double weight;
    DocDataStructure(String document, double weight){
        this.document = document;
        this.weight = weight;
    }
    public int compareTo(DocDataStructure other){
        return Double.compare(this.weight, other.weight);
    }
}
