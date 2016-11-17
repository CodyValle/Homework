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
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define INIT_MUTEX 1
#define INIT_EMPTY 100
#define INIT_FULL  0
#define EMPTY_ARG  0 //used when argument is optional

const int PROD = 0;
const int CONS = 1;

void criticalSection(int who)
{
    if (who == PROD)
        printf("Producer baking bread\n");
    else
        printf("Consumer eating bread\n");
}

void producer(int mutex, int empty, int full, struct sembuf* wait, struct sembuf* signal)
{
    for(int i = 0; i < 5; i++)
    {
        semop(empty, wait, 1);
        semop(mutex, wait, 1);
        criticalSection(PROD);
        semop(mutex, signal, 1);
        semop(full, signal, 1);

        //sleep(1);
    }
}

void consumer(int mutex, int empty, int full, struct sembuf* wait, struct sembuf* signal)
{
    for (int i = 0; i < 5; i++)
    {
        semop(full, wait, 1);
        semop(mutex, wait, 1);
        criticalSection(CONS);
        semop(mutex, signal, 1);
        semop(empty, signal, 1);
    }
}

//set struct values for down/up ops to be used in semop
//By design, we will always refer to the 0th semaphore in the set
void set_sem_struct(struct sembuf sem[], int op_val)
{
    sem[0].sem_num = 0;
    sem[0].sem_op = op_val;
    sem[0].sem_flg = SEM_UNDO;
}

//retrieve value held by the 0th semaphore, which is the second arg.
//0 in the final argument indicates that it is not used
void get_sem_values(int mutex, int empty, int full)
{
    int m, e, f;
    m = semctl(mutex, 0, GETVAL, 0);
    e = semctl(empty, 0, GETVAL, 0);
    f = semctl(full, 0, GETVAL, 0);
    printf("mutex: %d empty: %d full: %d\n", m,e,f);
}

//set mutex, empty, full to initial values
//these semaphore sets consist of a single semaphore, the 0th
void set_sem_values(int mutex, int empty, int full)
{
    semctl(mutex, 0, SETVAL, INIT_MUTEX);
    semctl(empty, 0, SETVAL, INIT_EMPTY);
    semctl(full, 0, SETVAL, INIT_FULL);
}

//create a semaphore set of 1 semaphore specified by key
int create_semaphore(int key)
{
    int new_sem;
    if ((new_sem = semget(key, 1, 0777 | IPC_CREAT)) == -1)
    {
        perror("semget failed");
        exit(1);
    }
    return new_sem;
}

//Display error and exit if semop fails
void fatal_error(char sem[])
{
    perror(strcat("Exiting. Failed on ",sem));
    exit(1);
}

//remove semaphores
void cleanup(int mutex, int empty, int full)
{
    semctl(mutex, 1, IPC_RMID, EMPTY_ARG);
    semctl(empty, 1, IPC_RMID, EMPTY_ARG);
    semctl(full, 1, IPC_RMID, EMPTY_ARG);
}

int main()
{
    struct sembuf wait_s[1], signal_s[1]; //create semaphore structs
    int mutex, empty, full;   //will hold semaphore identifiers
    int status;

    //set wait or signal structs used in semop
    set_sem_struct(wait_s, -1);
    set_sem_struct(signal_s,1);

    //create semaphore sets using arbitrary int unique to the semaphore set.
    mutex = create_semaphore(0);
    empty = create_semaphore(1);
    full = create_semaphore(2);

    //set semaphores to initial values
    set_sem_values(mutex, empty, full);

    printf("Intial semaphore values\n");
    get_sem_values(mutex, empty, full);

    // Attempting the fork
	pid_t pid = fork();

	if (pid < 0) // Then the fork failed
	{
		fprintf(stderr, "Failed to Fork");
		return -1;
	}

    //parbegin
        if (pid == 0) // This is the child process
        {
            consumer(mutex, empty, full, wait_s, signal_s);
            _Exit(3); // Close the child process
        }
        else
        {
            producer(mutex, empty, full, wait_s, signal_s);
            while (wait(&status) != pid); // Wait for the child process to finish
        }
    //parend

    printf("Final semaphore values\n");
    get_sem_values(mutex, empty, full);
    cleanup(mutex, empty, full); //remove semaphores
}
