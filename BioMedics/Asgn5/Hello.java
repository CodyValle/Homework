/**
 * Author: Cody Valle
 * Date: 10/7/2016
 * Description: This program takes a TPR and an FPR and calculates the post-test probability.
**/

import Java.util.Scanner

public class Hello
{
	public static void main(String[] args)
	{
		Scanner scan = new Scanner(System.in); // Create the scanner
		
		// Get the TPR
		System.out.print("What is the TPR? ");
		float TPR = scan.nextFloat();
		System.out.println();
		
		// Get the FPR
		System.out.print("What is the FPR? ");
		float FPR = scan.nextFloat();
		System.out.println();
		
		// Calculate the post-test probability
		float post = TPR / FPR;
		System.out.println("The post-test probability is " + post + "."); // Print it out
	}
}