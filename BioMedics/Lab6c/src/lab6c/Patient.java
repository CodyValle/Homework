/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab6c;

import java.util.List;
import java.util.ArrayList;

/**
 *
 * @author cvalle
 */
public class Patient {
    
    private List<Double> weights = new ArrayList<>();

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Patient p = new Patient();
        
        double array[] = {1,2,3,4,5,6,7,8,9};
        for (double a : array)
            p.addWeight(a);
        
        System.out.println("Mean: " + ArrayListOps.mean(p.getWeights()) + ".");
        System.out.println("StdDev: " + ArrayListOps.stddev(p.getWeights()) + ".");
        System.out.println("Max: " + ArrayListOps.max(p.getWeights()) + ".");
        System.out.println("Min: " + ArrayListOps.min(p.getWeights()) + ".");
    }
    
    public void addWeight(double weight) {
        weights.add(weight);
    }
    
    public List<Double> getWeights() {
        return weights;
    }
    
}
