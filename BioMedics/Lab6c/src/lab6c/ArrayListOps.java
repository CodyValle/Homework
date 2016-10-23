/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab6c;

import java.util.List;

/**
 *
 * @author cvalle
 */
public class ArrayListOps {
    public static double mean(List<Double> in) {
        double sum = 0;
        for (Double a : in)
            sum += a;
        return sum / in.size();
    }
    
    public static double stddev(List<Double> in) {
        double mean = mean(in);
        double temp = 0;
        for (double a : in)
            temp += (a - mean) * (a - mean);
        return Math.sqrt(temp / in.size());
    }
    
    public static double max(List<Double> in) {
        double max = in.get(0);
        for(double a : in)
            if (a > max) max = a;
        return max;
    }
    
    public static double min(List<Double> in) {
        double min = in.get(0);
        for(double a : in)
            if (a < min) min = a;
        return min;
    }
}
