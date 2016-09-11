// Author: Cody Valle
// Date: Sept. 10th, 2016
// Description: Main file for a shell for Operating Systems class

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables
const unsigned MAX_INPUT = 8; // Maximum input per command
const char* WHITESPACE = " \t"; // Whitespace characters

// Enable pseudo-bool type for fun
const int true = 1;
const int false = 0;
typedef int bool;

// Input struct
typedef struct {
	unsigned argc;
	char** argv;
} Input; 

// Get input from the user
char* getInput()
{
	char* in = (char*)malloc(MAX_INPUT);
	char* start = in;
	char c;
	
	for (unsigned i = 0; i < MAX_INPUT; ++i) // Enforce the 80 character max
	{
		c = getchar(); // Get a character
		if (c == '\n') break; // Stop if enter is pressed
		*in++ = c; // Save the character
	}
	
	*in = '\0'; // Terminate the string
	
	return start;
}

// Process input and fill an Input struct argument
Input* processInput(char* in)
{
	while (*in != '\0' && strchr(WHITESPACE, *in)) ++in; // Trim leading whitespace
	if (*in == '\0') return 0; // Fail if there are no words
	
	// Create return variable
	Input* out = (Input*)malloc(sizeof(Input));
	out->argc = 0;
	out->argv = 0;
	
	char* start = in; // Save the location of the first character
	
	unsigned char_count = 0, // Number of characters in the input
					 word_count = 1; // Number of words
	
	for (; *in != '\0'; ++in) // Advance until null is found
	{
		if (strchr(WHITESPACE, *in)) // End of word
		{
			while (in[1] != '\0' && strchr(WHITESPACE, in[1])) ++in; // Advance to next word
			word_count += in[1] == '\0' ? 0 : 1; // Increase word count if there is another word
		}
		/*
		else if (*in == '"') // Quoted input
		{
			/// Needs to account for '\"', and add to char_count
			do ++in; // Move along until end of string is found
			while (*in != '\0' && *in != '"');
		}
		*/
		else ++char_count; // Increase the number of characters (including one after every word)
	}
	
	char* words = (char*)malloc(char_count + 1); // The number of characters in the input string, plus a final null
	out->argc = word_count; // Set the number of arguments
	out->argv = (char**)malloc(sizeof(char*) * word_count); // A pointer for every word
	word_count = 0; // Reset
	
	bool word_start = true; // We start at the beginning of a word

	// Iterate through again, filling the argv portion of the Input struct
	for (; *start != '\0'; ++start, ++words)
	{
		*words = *start; // Copy the character over
		if (word_start)
		{
			out->argv[word_count * sizeof(char*)] = words; // Save the location of the beginning of the word in argv
			word_start = false; // No longer at the beginning of a word
		}
		else if (strchr(WHITESPACE, *start)) // End of word
		{
			*words = '\0'; // Terminate the current string
			while (start[1] != '\0' && strchr(WHITESPACE, start[1])) ++start; // Advance to next word
			if (start[1] != '\0')
			{
				word_start = true; // We will be at the beginning of a word next iteration
				++word_count; // Increase word count if there is another word
			}
		}
	}
	*words = '\0'; // Write the final null
	
	return out; // Return the Input
}

// Main 
int main()
{
	Input* input = 0; // For inputs
	bool loop = true;
	
	while (loop) // Main loop
	{
		printf("Prompt$ "); // Print the prompt
		
		char* in = getInput(); // Get the user's input
		input = processInput(in); // Get and process the user's input
		free(in); // Done with that
		if (!input) continue; // In case nothing was entered
		loop = strcmp("exit", input->argv[0]); // If the first argument was exit, then exit
		
		if (loop)
		{
			// Do something with the input
			printf("There are %d arguments:\n", input->argc);
			for (unsigned i = 0; i < input->argc; ++i)
				printf("%s\n",input->argv[i * sizeof(char*)]);
		}
		
		// Free all the memory pointed at by the Input struct
		free(input->argv[0]); // The first pointer owns all the other pointers' data
		free(input->argv); // Free the stored pointers
		free(input);
	}
	
	return 0;
}
