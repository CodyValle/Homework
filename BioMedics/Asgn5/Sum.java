/**
 * Author: Cody Valle
 * Date: 10/7/2016
 * Description: This program gets integers from the command line, and
 *  adds it to the total sum. When a 0 is received, the program terminates.
**/

import java.util.Scanner;

public class Sum
{
	public static void main(String[] args)
	{
		Scanner scan = new Scanner(System.in); // Create the scanner
		
		int total = 0; // Holds the total sum of the numbers
		int input = 0; // Holds the current input
		do
		{
			// Get a number input
			System.out.print("What number? ");
			input = scan.nextInt();
			
			total += input; // Add the number tot he total
			System.out.println("The current total is " + total + "."); // Print out the current total
		} while (input != 0); // Run until input is 0
	}
}