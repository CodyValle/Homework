#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

const unsigned NUM_THREADS = 3;

int* numbers; // the global array

// Returns a random integer between min and max.
int randomNumber(int min, int max)
{
    return (rand() % (max - min)) + min;
}

// Swaps the values pointed at by a and b.
void swap(int* a, int* b)
{
    int s = *a;
    *a = *b;
    *b = s;
}

// Performs a selection sort on the array passed in from the start index to the end index
void selectionSort(int array[], int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        int min = i;
        for (int j = i; j < end; ++j)
        {
            if (array[j] < array[min])
                min = j;
        }
        swap(&array[i], &array[min]);
    }
}

// Performs the duties of thread 1
void* thread1(void* in)
{
    int N = *((int*)in);

    selectionSort(numbers, 0, N / 2 + (N % 2 == 0 ? 0 : 1));

    pthread_exit(0);
}

// Performs the duties of thread 2
void* thread2(void* in)
{
    int N = *((int*)in);

    selectionSort(numbers, N / 2 + (N % 2 == 0 ? 0 : 1), N);

    pthread_exit(0);
}

// Structure for parameters to pass into thread3
typedef struct
{
    char* file; // The name of the file to write to
    int N; // The number of numbers to read
} params;

// Performs the duties of thread 3
void* thread3(void* in)
{
    params* p = (params*)in; // cast the pointer to what it is

    FILE* outfile = fopen(p->file, "w"); // Open file for writing

    int starty = p->N / 2 + (p->N % 2 == 0 ? 0 : 1), // starty is the middle index
	endy = p->N, // endy is the last index or the second array
        endx = starty; // endx is the last index of the first array

    for (int x = 0, y = starty; x + y < endy + endx;)
    {
	// Write the lowest number to the file, and increment the respective index
        if (numbers[x] < numbers[y])
            fprintf(outfile, "%d ", numbers[x++]);
        else
            fprintf(outfile, "%d ", numbers[y++]);

	// If one array is done, write the rest of the other to the file
        if (x == endx)
            while (y < endy)
                fprintf(outfile, "%d ", numbers[y++]);
        else if (y == endy)
            while (x < endx)
                fprintf(outfile, "%d ", numbers[x++]);
    }

    fclose(outfile);

    pthread_exit(0);
}

// main method
int main(int argc, char** argv)
{
	// Safety
    if (argc < 3)
    {
        printf("Error! Usage: threads infile outfile [N]\n");
        exit(-1);
    }

	// Get the number of numbers to sort
    int N = 100;
    int* pn = &N;
    if (argc > 3)
        N = atoi(argv[3]);

	// Minimum N of 2
	if (N < 2) N = 2;


	// Initialize the array
    numbers = (int*)malloc(N * sizeof(int));
    if (!numbers)
    {
        printf("Error! Failed to allocate memory for global number array\n");
        exit(-2);
    }
    memset(numbers, 0, N);

    srand(time(NULL)); // Seed random

    FILE* outfile = fopen(argv[1], "w"); // Open file for writing

	// Fill the array and unsorted file
    for (int i = 0; i < N; ++i)
    {
        int num = randomNumber(0, 100);
        fprintf(outfile, "%d ", num);
        numbers[i] = num;
    }

    fclose(outfile);

    pthread_t threads[NUM_THREADS];

    int status = 0;

	// Create the sorting threads
    status = pthread_create(&threads[0], NULL, thread1, (void*)pn);
    if (status != 0)
        printf("Error in thread 1!\n");

    status = pthread_create(&threads[1], NULL, thread2, (void*)pn);
    if (status != 0)
        printf("Error in thread 2!\n");

	// Create the parameters for thread 3
    params* p = malloc(sizeof(params));
    if (!p)
    {
        printf("Error! Couldn't allocate space for param structure.\n");
        free(numbers); // Free allocated memory
        exit(-3);
    }
    memset(p, 0, sizeof(params));
    p->file = argv[2];
    p->N = N;

	// Join on the two threads before creating the third
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	// Create thread 3
    status = pthread_create(&threads[2], NULL, thread3, (void*)p);
    if (status != 0)
        printf("Error in thread 3!\n");

	// Join thread 3
  pthread_join(threads[2], NULL);

	// Free allocated memory
    free(numbers);
    free(p);

    pthread_exit(0); // Treat main as a thread
}
