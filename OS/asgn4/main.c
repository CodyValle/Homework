#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

const unsigned NUM_THREADS = 3;

int* numbers; // the global array

int randomNumber(int min, int max)
{
    return (rand() % (max - min)) + min;
}

void swap(int* a, int* b)
{
    int s = *a;
    *a = *b;
    *b = s;
}

void selectionSort(int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        int min = i;
        for (int j = i; j < end; ++j)
        {
            if (numbers[j] < numbers[min])
                min = j;
        }
        swap(&numbers[i], &numbers[min]);
    }
}

void* thread1(void* in)
{
    int N = *((int*)in);

    selectionSort(0, N / 2 + (N % 2 == 0 ? 0 : 1));

    pthread_exit(NULL);
}

void* thread2(void* in)
{
    int N = *((int*)in);

    selectionSort(N / 2 + (N % 2 == 0 ? 0 : 1), N);

    pthread_exit(NULL);
}

typedef struct
{
    char* file;
    int N;
} params;

void* thread3(void* in)
{
    params* p = (params*)in;

    FILE* outfile = fopen(p->file, "w"); // Open file for writing

    int starty = p->N / 2 + (p->N % 2 == 0 ? 0 : 1), endy = p->N,
        endx = starty;

    for (int x = 0, y = starty; x + y < endy + endx;)
    {
        if (numbers[x] < numbers[y])
            fprintf(outfile, "%d ", numbers[x++]);
        else
            fprintf(outfile, "%d ", numbers[y++]);

        if (x == endx)
            while (y < endy)
                fprintf(outfile, "%d ", numbers[y++]);
        else if (y == endy)
            while (x < endx)
                fprintf(outfile, "%d ", numbers[x++]);
    }

    fclose(outfile);

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Error! Usage: threads infile outfile [N]\n");
        exit(-1);
    }

    int N = 100;
    int* pn = &N;
    if (argc > 3)
        N = atoi(argv[3]);

    numbers = (int*)malloc(N * sizeof(int));
    if (!numbers)
    {
        printf("Error! Failed to allocate memory for global number array\n");
        exit(-2);
    }
    memset(numbers, 0, N);

    srand(time(NULL)); // Seed random

    FILE* outfile = fopen(argv[1], "w"); // Open file for writing

    for (int i = 0; i < N; ++i)
    {
        int num = randomNumber(0, 100);
        fprintf(outfile, "%d ", num);
        numbers[i] = num;
    }

    fclose(outfile);


    pthread_t threads[NUM_THREADS];

    int status = 0;

    status = pthread_create(&threads[0], NULL, thread1, (void*)pn);
    if (status != 0)
        printf("Error in thread 1!\n");

    status = pthread_create(&threads[1], NULL, thread2, (void*)pn);
    if (status != 0)
        printf("Error in thread 2!\n");

    /** Currently a hack. **/
    for (unsigned i = 0; i < NUM_THREADS - 1; ++i)
        pthread_join(threads[i], NULL);
    /***/

    params* p = malloc(sizeof(params));
    if (!p)
    {
        printf("Error! Couldn't allocate space for param structure.\n");
        free(numbers);
        exit(-3);
    }
    memset(p, 0, sizeof(params));
    p->file = argv[2];
    p->N = N;

    status = pthread_create(&threads[2], NULL, thread3, (void*)p);
    if (status != 0)
        printf("Error in thread 3!\n");

    /** Remember to change int i = 2 to int i = 0 when the above hack is removed **/
    for (unsigned i = 2; i < NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);

    free(numbers);
    free(p);

    return 0;
}
