/**
 * Scott Rein and Cody Valle
 * srein@zagmail.gonzaga.edu, cvalle@zagmail.gonzaga.edu
 * Operating Systems Assignment 6
 * CPSC 346.2
**/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

const int PROD = 0;
const int CONS = 1;

// Enable pseudo-bool type for fun
typedef unsigned char bool;
const bool true = 1;
const bool false = 0;

// Semaphore
typedef struct semaphore
{
    int mutex; // Mutex lock
    int empty; // Open slots
    int full; // Full slots
} semaphore;

// Pointer to the shared memory
semaphore* shared = NULL;

void criticalSection(int who)
{
    if (who == PROD)
        printf("Producer making an item\n");
    else
        printf("Consumer consuming an item\n");
}

void producer()
{
    for(int i = 0; i < 5; i++)
    {
        shared->empty--;
        shared->mutex--;
        criticalSection(PROD);
        shared->mutex++;
        shared->full++; //experiment with putting delays after this line
    }
}

void consumer()
{
    for (int i = 0; i < 5; i++)
    {
        shared->full--;
        shared->mutex--;
        criticalSection(CONS);
        shared->mutex++;
        shared->empty++; //experiment with putting delays after this line
    }
}

int main(int argc, char** argv)
{
    // Make the shared pointer shared between parent and child
    int shmid = shmget(0, sizeof(semaphore), 0777 | IPC_CREAT);
    shared = shmat(shmid, 0, 0);
    shared->mutex = 1;
    shared->empty = 100;
    shared->full = 0;

    printf("Initial semaphore values: mutex = %d, empty = %d, full = %d\n", shared->mutex, shared->empty, shared->full);

    // Attempting the fork
	int status;

	pid_t pid = fork();

	if (pid < 0) // Then the fork failed
	{
		fprintf(stderr, "Failed to Fork");
		return -1;
	}

    //parbegin
        if (pid == 0) // This is the child process
        {
            consumer();
            _Exit(3); // Close the child process
        }
        else
        {
            producer();
            while (wait(&status) != pid); // Wait for the child process to finish
        }
    //parend

    printf("Final semaphore values: mutex = %d, empty = %d, full = %d\n", shared->mutex, shared->empty, shared->full);

    // Free shared memory
    shmdt(shared);
    shmctl(shmid, IPC_RMID, 0);
}
