/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab6b;

/**
 *
 * @author cvalle
 */
public class ArrayOps {
    public static double mean(double[] in)
    {
        double sum = 0;
        for (double a : in)
            sum += a;
        return sum / in.length;
    }
    
    public static double stddev(double[] in)
    {
        double mean = mean(in);
        double temp = 0;
        for (double a : in)
            temp += (a - mean) * (a - mean);
        return Math.sqrt(temp / in.length);
    }
    
    public static double max(double[] in)
    {
        double max = in[0];
        for(double a : in)
            if (a > max) max = a;
        return max;
    }
    
    public static double min(double[] in)
    {
        double min = in[0];
        for(double a : in)
            if (a < min) min = a;
        return min;
    }
}
