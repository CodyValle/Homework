/**
 * Author: Cody Valle
 * Date: 10/7/2016
 * Description: This program takes a TPR and an FPR and calculates the post-test probability.
**/

import java.util.Scanner;

public class PostTest
{
	public static void main(String[] args)
	{
		Scanner scan = new Scanner(System.in); // Create the scanner
		
		// Get the pre-test probability
		System.out.print("What is the pre-test probability? ");
		float pre = scan.nextFloat();
		
		// Get the TPR
		System.out.print("What is the TPR? ");
		float TPR = scan.nextFloat();
		
		// Get the FPR
		System.out.print("What is the FPR? ");
		float FPR = scan.nextFloat();
		
		// Calculate the post-test probability
		float post = pre * TPR / (pre * TPR + (1 - TPR) * FPR);
		System.out.println("The post-test probability is " + post + "."); // Print it out
	}
}