#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void *thread_behavior(void *arg)
{
    int id = *((int *)arg);
    int sleep_time = rand() % 8 + 1; // Sleep for 1 to 8 seconds
    printf("Thread %d: will sleep for %d seconds\n", id, sleep_time);
    sleep(sleep_time);
    printf("Thread %d: exiting\n", id);
    pthread_exit(NULL);
}

void pattern_1_threads(int num_threads)
{
    printf("** Pattern 1 (Threads): creating %d threads\n", num_threads);

    pthread_t threads[num_threads];
    int ids[num_threads];

    // Create all threads
    for (int i = 0; i < num_threads; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_behavior, &ids[i]);
        printf("Main: created thread %d\n", i);
    }

    // Join all threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Main: thread %d has exited\n", i);
    }

    printf("** Pattern 1: All threads have exited\n");
}

void *thread_chain_behavior(void *arg)
{
    int id = *((int *)arg);
    int sleep_time = rand() % 8 + 1; // Sleep for 1 to 8 seconds

    printf("Thread %d: will sleep for %d seconds\n", id, sleep_time);
    sleep(sleep_time);

    // Create the next thread in sequence if not at the end
    if (id + 1 < *((int *)arg + 1))
    {
        printf("Thread %d: creating thread %d\n", id, id + 1);
        pthread_t next_thread;
        int next_ids[2] = {id + 1, *((int *)arg + 1)};
        pthread_create(&next_thread, NULL, thread_chain_behavior, next_ids);
        pthread_join(next_thread, NULL);
        printf("Thread %d: thread %d has exited\n", id, id + 1);
    }

    printf("Thread %d: exiting\n", id);
    pthread_exit(NULL);
}

void pattern_2_threads(int num_threads)
{
    printf("** Pattern 2 (Threads): creating %d threads\n", num_threads);

    pthread_t first_thread;
    int ids[2] = {0, num_threads};
    pthread_create(&first_thread, NULL, thread_chain_behavior, ids);

    pthread_join(first_thread, NULL);
    printf("** Pattern 2: All threads have exited\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <number of threads> <pattern number>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    if (num_threads < 1 || num_threads > 256)
    {
        fprintf(stderr, "Number of threads should be between 1 and 256.\n");
        return 1;
    }

    // Choose the pattern based on input
    switch (pattern)
    {
    case 1:
        pattern_1_threads(num_threads);
        break;
    case 2:
        pattern_2_threads(num_threads);
        break;
    default:
        fprintf(stderr, "Invalid pattern number. Please choose 1 or 2.\n");
        return 1;
    }

    return 0;
}