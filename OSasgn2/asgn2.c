// Author: Cody Valle
// Date: Sept. 10th, 2016
// Description: Main file for a shell for Operating Systems class

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

// Non-exhaustive ANSI color codes
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

#define IRED "\x1B[61m"
#define IGRN "\x1B[62m"

#define BGRN "\x1B[92m"
#define BWHT "\x1B[97m"

#define RST "\x1B[0m"

// Global variables
const char PROMPT[] = CYN "Prompt" BWHT "$ " RST;
const char OUTPUT[] = BGRN;
const unsigned MAX_INPUT = 80; // Maximum input per command
const char WHITESPACE[] = {' ','\t'}; // Whitespace characters

// Enable pseudo-bool type for fun
const int true = 1;
const int false = 0;
typedef int bool;

// Input struct
typedef struct {
	int argc; // Number of arguments
	char** argv; // Pointer to array of argument strings
} Input;

// Prints out the prompt
void printPrompt()
{
	printf(PROMPT); // Simple stuff
}

// Wraps malloc by returning initialized space
void* initSpace(int size)
{
	char* ret = (char*)malloc(size); // Allocate space
	if (!ret) return 0; // Ensure the space was allocated
	memset(ret, 0, size); // Initialize them all to zero
	return ret; // Return initialized space
}

// Get input from the user
char* getInput()
{
	char* in = initSpace(MAX_INPUT + 1); // Plus a null
	char* start = in;
	
	for (unsigned i = 0; i < MAX_INPUT; ++i) // Enforce the character max
	{
		unsigned char c = getchar(); // Get a character
		if (c == '\n') break; // Stop if enter is pressed
		*in++ = c; // Save the character
	}
	
	*in = '\0'; // Terminate the string
	
	#ifdef DEBUG
		printf(YEL); // Yellow color for debug stuff
		printf("_DEBUG_ Recieved input string: %s\n", start);
		printf("_DEBUG_ String length: %lu\n", strlen(start));
	#endif
	
	return start;
}

// Process input and fill an Input struct argument
Input* processInput(char* in)
{
	while (*in != '\0' && strchr(WHITESPACE, *in)) ++in; // Trim leading whitespace
	if (*in == '\0')
	{
		#ifdef DEBUG
			printf("_DEBUG_ Empty or pure whitespace input.\n");
		#endif
		return 0; // Fail if there are no words
	}
	
	// Create return variable
	Input* out = initSpace(sizeof(Input));
	
	char* start = in; // Save the location of the first character
	
	unsigned char_count = 0, // Number of characters in the input
					 word_count = 1; // Number of words (there is at least one if we get this far)
	
	for (; *in != '\0'; ++in) // Advance until null is found
	{
		if (strchr(WHITESPACE, *in)) // End of word
		{
			while (in[1] != '\0' && strchr(WHITESPACE, in[1])) ++in; // Advance to next word
			word_count += in[1] == '\0' ? 0 : 1; // Increase word count if there is another word
		}
		else ++char_count; // Increase the number of characters (including one after every word)
	}
	
	#ifdef DEBUG
		printf("_DEBUG_ Counted %d valid characters.\n", char_count);
		printf("_DEBUG_ Counted %d words.\n", word_count);
		printf("_DEBUG_ Mallocing %d bytes for *argv[0]\n", char_count + word_count);
		printf("_DEBUG_ Mallocing %lu bytes for *argv\n", sizeof(char*) * word_count);
	#endif
	
	char* words = initSpace(char_count + word_count); // The number of characters in the input string, plus a final null
	if (!words) return 0;
	out->argc = word_count; // Set the number of arguments
	out->argv = initSpace(sizeof(char*) * word_count); // A pointer for every word
	char** writer = out->argv; // Tracks where to write the addresses to
	
	bool word_start = true; // We start at the beginning of a word

	// Iterate through again, filling the argv portion of the Input struct
	for (; *start != '\0'; ++start, ++words)
	{
		*words = *start; // Copy the character over
		if (word_start)
		{
			*writer++ = words; // Save the location of the beginning of the word in argv, then move pointer along
			word_start = false; // No longer at the beginning of a word
		}
		else if (strchr(WHITESPACE, *start)) // End of word
		{
			*words = '\0'; // Terminate the current string
			while (start[1] != '\0' && strchr(WHITESPACE, start[1])) ++start; // Advance to next word
			word_start = start[1] != '\0'; // We will be at the beginning of a word next iteration
		}
	}
	
	#ifdef DEBUG
		printf("_DEBUG_ Addresses of argv\n");
		for (int i = 0; i < out->argc; ++i)
			printf("_DEBUG_ Index %d: %p\n", i, (void*)out->argv[i]);
	#endif
	
	return out; // Return the Input
}

// Main 
int main()
{
	Input* input = 0; // For inputs
	bool loop;
	
	do // Main loop
	{
		printPrompt(); // Print the prompt
		
		char* in = getInput(); // Get the user's input
		input = processInput(in); // Get and process the user's input
		free(in); // Done with that
		if (!input) continue; // In case nothing was entered
		loop = strcmp("exit", input->argv[0]); // If the first argument was exit, then exit
		
		if (loop)
		{
			printf(OUTPUT); // Change the output color if specified
			
			// Do something with the input
			printf("There are %d arguments:\n", input->argc);
			for (int i = 0; i < input->argc; ++i)
				printf("%s\n",input->argv[i]);
		}
		
		// Free all the memory pointed at by the Input struct
		free(input->argv[0]); // The first pointer owns all the other pointers' data
		free(input->argv); // Free the stored pointers
		free(input);
	} while (loop);
	
	return 0;
}

