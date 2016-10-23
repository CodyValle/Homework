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
public class ArrayTester {
    public static void main(String[] args)
    {
        double[] array = {1,2,3,4,5,6,7,8,9};
        
        System.out.println("Mean: " + ArrayOps.mean(array) + ".");
        System.out.println("StdDev: " + ArrayOps.stddev(array) + ".");
        System.out.println("Max: " + ArrayOps.max(array) + ".");
        System.out.println("Min: " + ArrayOps.min(array) + ".");
    }
}
