/**
 * Scott Rein and Cody Valle
 * srein@zagmail.gonzaga.edu, cvalle@zagmail.gonzaga.edu
 * Operating Systems Assignment 5
 * CPSC 346.2
 * This program takes four numbers as parameters (e.g. a.out 1 1 1 1)
 *  and runs a parent child process that do not access their respective
 *  critical sections at the same time.
**/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

// Enable pseudo-bool type for fun
typedef unsigned char bool;
const bool true = 1;
const bool false = 0;

// Pointer to the shared memory
unsigned char* shared = NULL;

// This is the critical section
void cs(char process, int time_crit_sect)
{
    if (process == 'p')
    {
        fprintf(stderr, "parent in critical section\n");
        sleep(time_crit_sect);
        fprintf(stderr, "parent leaving critical section\n");
    }
    else
    {
        fprintf(stderr, "child in critical section\n");
        sleep(time_crit_sect);
        fprintf(stderr, "child leaving critical section\n");
    }
}

void non_cs(int time_non_crit_sect)
{
    sleep(time_non_crit_sect);
}

// This is what the parent process does
void parent(int time_crit_sect, int time_non_crit_sect)
{
    for (int i = 0; i < 4; i++)
    {
        shared[0] = true;
        shared[2] = true;
        if (!shared[3])
            while (shared[1] && shared[2]);

        cs('p', time_crit_sect);

        shared[2] = true;

        non_cs(time_non_crit_sect);
    }
    shared[3] = true;
}

// This is what the child process does
void child(int time_crit_sect, int time_non_crit_sect)
{
    for (int i = 0; i < 4; i++)
    {
        shared[1] = true;
        shared[2] = false;
        if (!shared[3])
            while (shared[0] && !shared[2]);

        cs('c', time_crit_sect);

        shared[2] = false;

        non_cs(time_non_crit_sect);
    }
    shared[3] = true;
}

// Main
int main(int argc, char** argv)
{
    // Load the wait times from the command line
    int time_parent = argc >= 2 ? atoi(argv[1]) : 2;
    int time_child = argc >= 3 ? atoi(argv[2]) : 1;
    int time_parent_non_cs = argc >= 4 ? atoi(argv[3]) : 3;
    int time_child_non_cs = argc >= 5 ? atoi(argv[4]) : 1;

    // Make the shared pointer shared between parent and child
    int shmid = shmget(0, 4, 0777 | IPC_CREAT);
    shared = shmat(shmid, 0, 0);
    // Initialize the shared pointer
    shared[0] = true;
    shared[1] = true;
    shared[2] = true;
    shared[3] = false;

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
        child(time_child, time_child_non_cs);
		_Exit(3); // Close the child process
	}
	else
    {
        parent(time_parent, time_parent_non_cs);
        while (wait(&status) != pid); // Wait for the child process to finish
    }

    // Free shared memory
    shmdt(shared);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
