

package finalproject;

import java.io.Serializable;

public class IwStructure implements Comparable<IwStructure>, Serializable  {
    protected String concept;
    protected double weight;
    
    IwStructure(String concept, double weight){
        this.concept = concept;
        this.weight = weight;
    }
    
    @Override
    public int compareTo(IwStructure other){
        return Double.compare(this.weight, other.weight);
    }
    
    @Override
    public String toString() {
        return concept + " " + weight;
    }
}
