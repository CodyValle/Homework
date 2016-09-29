// Authors: Cody Valle, Scott Rein
// Date: Sept. 27th, 2016
// Description: Main file for a shell for Operating Systems class
// Long description: Functional shell program capable of remembering past commands
// 	and executing current and past commands.

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

//#define DEBUG

// Global variables
#ifdef __unix__ // On Unix...
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

		// PROMPT commands
		// && will print an '&'
		// &c will print the full current working directory
		// &u will print the user that owns the shell
    const char PROMPT[] = CYN "&u" BWHT "@" RED "&c" BWHT "$ " RST;
    const char OUTPUT[] = BGRN;
#else // On Windows
    const char PROMPT[] = "&u@&c$ ";
#endif // _WIN32

const unsigned MAXINPUT = 80; // Maximum input per command
const char WHITESPACE[] = {' ','\t'}; // Whitespace characters

// Enable pseudo-bool type for fun
typedef int bool;
const int true = 1;
const int false = 0;

// Input struct
typedef struct {
	int argc; // Number of arguments
	char** argv; // Pointer to array of argument strings
} Input;

// Frees all memory owned by an Input struct
void delete_input(Input* input)
{
  free(input->argv[0]); // The first pointer owns all the other pointers' data
	free(input->argv); // Free the stored pointers
  free(input); // Free the created Input struct
}

// Parses the PROMPT string and writes the desired output to ptr
void parsePrompt(char* ptr)
{
	for (unsigned i = 0; i < sizeof(PROMPT); ++i)
	{
		if (PROMPT[i] == '&') // Check for a switch
		{
			++i; // Increment i
			switch (PROMPT[i])
			{
			case '&': // Write a literal '&'
				ptr += sprintf(ptr, "%c", '&');
				break;
				
			case 'c': // Write the full current working directory
			{
				char cwd[256]; // The hope is that we don't overflow buf
				
				if (getcwd(cwd, sizeof(cwd)) != NULL) // Great! write it to buf
					ptr += sprintf(ptr, "%s", cwd);
				else // We write a default value
					ptr += sprintf(ptr, "path");
					
				break;
			}
				
			case 'u': // Write the user who owns the shell process
			{
				#ifdef __unix__
					char usr[_POSIX_LOGIN_NAME_MAX]; // The longest a name can be!
				
					if (getlogin_r(usr, sizeof(usr)) == 0) // Great! write it to buf
						ptr += sprintf(ptr, "%s", usr);
					else // We write a default value
						ptr += sprintf(ptr, "user");
				#else
					ptr += sprintf(ptr, "user");
				#endif
				
				break;
			}
				
			default: break; // Don't know what the switch was, so we will ignore it
			}
		}
		else // Write the character to the buffer
			ptr += sprintf(ptr, "%c", PROMPT[i]);
	}
}

// Prints out the prompt
void printPrompt(char* prompt)
{
	printf("%s", prompt); // Slightly more complicated
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
	char* in = initSpace(MAXINPUT + 1); // Plus a null
	char* start = in;

	for (unsigned i = 0; i < MAX_INPUT; ++i) // Enforce the character max
	{
		unsigned char c = getchar(); // Get a character
		if (c == '\n') break; // Stop if enter is pressed
		*in++ = c; // Save the character
	}

	*in = '\0'; // Terminate the string

	#ifdef DEBUG
        #ifdef __unix__
            printf(YEL); // Yellow color for debug stuff
        #endif
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
	out->argv = initSpace(sizeof(char*) * (word_count + 1)); // A pointer for every word, and a zero on the end
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
	
	*writer = 0; // NULL-terminate the argv

	#ifdef DEBUG
		printf("_DEBUG_ Addresses of argv\n");
		for (int i = 0; i < out->argc; ++i)
			printf("_DEBUG_ Index %d: %p\n", i, (void*)out->argv[i]);
	#endif

	return out; // Return the Input
}

// Wraps all the past commands data in a circular array
typedef struct
{
  unsigned max_cmds;
  unsigned front;
  unsigned back;
  unsigned count;
  Input** cmds;
} PastCommands;

// Initializes a PastCommands structure with up to max possible commands
PastCommands* get_past_commands_structure(const unsigned max)
{
  PastCommands* ret = initSpace(sizeof(PastCommands)); // Make the space
  ret->max_cmds = max; // Assign default values to everything
  ret->front = 0;
  ret->back = 0;
  ret->count = 0;
  ret->cmds = initSpace(sizeof(Input) * max);
  return ret; // This is essentially a constructor from C++...
}

// Frees all memory owned by a PastCommands struct
void free_past_commands(PastCommands* past)
{
  for (unsigned i = 0; i < past->count; ++i)
	{
		unsigned index = (past->front + i) % past->max_cmds;
		delete_input(past->cmds[index]); // Free all the memory pointed at by the Input struct
	}

  free(past->cmds); // Free the pointer to the Input pointers

  free(past); // Oh, look, a destructor...
}

// Adds a command to a PastCommands struct
void add_command(PastCommands* past, Input* input)
{
  if (past->count == past->max_cmds) // Then front and back are pointing at the same element
  {
  	delete_input(past->cmds[past->front]); // Free the memory before we overwrite the pointer
    past->front = (past->front + 1) % past->max_cmds; // Move front to the next command
  }
  else
    ++past->count; // Increase the count

  past->cmds[past->back] = input; // Same as the freed memory if past.count == past.max_cmds
  past->back = (past->back + 1) % past->max_cmds; // Move back along
}

// Gets an Input struct from a PastCommands struct. 0 is the latest command added
Input* get_command(PastCommands* past, unsigned index)
{
  if (index >= past->count) // This is not the command you're looking for
    return 0;

  unsigned temp_index = past->back; // temp_index tracks the command between back and front
  while (index + 1 > 0) // Count down to the command of our dreams
  {
    if (temp_index == 0) // Circular array stuff
      temp_index = past->max_cmds;

    --temp_index; // Reduce these indices
    --index;
  }

  return past->cmds[temp_index]; // We want this one!
}

// Prints out all commands stored in a PastCommands struct with their indices
void print_history(PastCommands* past)
{
  if (past->count == 0) return;

  unsigned index = past->count;
  do
  {
    printf("%d: ", index - 1); // The callable command index

    Input* input = past->cmds[(past->front + past->count - index) % past->max_cmds]; // Get the input for this iteration
    for (int i = 0; i < input->argc; ++i) // Print out the full argv
			printf("%s ",input->argv[i]);
		printf("\n");

		--index; // Next index
  } while (index > 0);
}

// Executes the Input struct
int execute(Input* input)
{
	// Attempting the fork
	int status;
	
	pid_t pid = fork();
	
	if (pid < 0) // Then the fork failed
	{
		fprintf(stderr, "Failed to Fork");
		return -1;
	}

	if (pid == 0) // This is the child process
	{
		if (execvp(input->argv[0], input->argv) < 0) // Execute the command and check return
			printf("%s: command not found\n", input->argv[0]);
		
		_Exit(3); // Close the child process
	}
	else
		while (wait(&status) != pid); // Wait for the child process to finish
	
  return 0;
}

// Main
int main(int argc, char* argv[])
{
	int mem_input = 0;
	if (argc > 1) // Parse input
		mem_input = atoi(argv[1]);

	unsigned mem = mem_input <= 0 ? 10 : mem_input; // Default value of 10

	Input* input = 0; // For inputs
	bool loop;

	PastCommands* past_commands = get_past_commands_structure(mem); // Get an initialized PastCommands structure that can save multiple commands
	
	char prompt[256]; // Buffer for printing out the prompt
	parsePrompt(prompt);

	do // Main loop
	{
		printPrompt(prompt); // Print the prompt

		char* in = getInput(); // Get the user's input
		input = processInput(in); // Get and process the user's input
		free(in); // No longer needed

		if (!input) continue; // In case nothing was entered
		loop = strcmp("exit", input->argv[0]); // If the first argument was exit, then exit

		if (loop)
		{
	    #ifdef __unix__
        printf(OUTPUT); // Change the output color if specified
      #endif

      // Check for history command
      bool history = !strcmp("history", input->argv[0]);

      if (history)
      {
      	print_history(past_commands);
	    	delete_input(input); // We are not saving this command
	    }
      else
      {
			  // Check whether we are executing a past command
			  if (input->argv[0][0] == '!')
			  {
			  	int index = 0; // Which command do we want to recall
			  	if (input->argv[0][1] != '!') // Check for special charater
				  	index = atoi(&input->argv[0][1]); // Skip the '!' character

			  	Input* recalled_input = get_command(past_commands, index); // Get the desired command

			    if (recalled_input) // If we got a valid command
			    	execute(recalled_input); // Execute
			    else
			    	printf("Error! No command that far in history.\n");

			    delete_input(input); // We are not saving this command

			  } // End if (... == '!')
			  else
			  { // Execute this input
					add_command(past_commands, input); // Save the Input in the past_commands
					execute(input);
			  }
      } // End else of if (history)

		} // End if (loop)
		else // We are exiting
		{
		  delete_input(input);
		}
	} while (loop); // End do

	free_past_commands(past_commands); // Clean up

	return 0;
}