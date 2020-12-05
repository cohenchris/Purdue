#include "part1.h"

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define REQUIRED_NUM_ARGS (3)

// The producers add the characters in g_prod_str to the g_buffer, in order.

char *g_prod_str = "The greatest teacher, failure is.";

bounded_buffer g_buffer = {0};

// This mutex must be held whenever you use the g_buffer.

pthread_mutex_t g_buffer_mutex = {0};

// g_empty_sem is the number of characters in the g_buffer,
// that need to be emptied.
// Producers should signal g_empty_sem and consumers should wait on it.

sem_t g_empty_sem = {0};

// g_full_sem is the opposite of g_empty_sem. It is the number of
// unfilled slots in the g_buffer that need to be filled. Consumers
// should signal it, and producers should wait on it.

sem_t g_full_sem = {0};

/*
 * Produce the characters (that is, add them to the g_buffer) from g_prod_str,
 * in order. Signal consumers appropriately after each character.
 * Receive an ID via an (int *).
 */

void *producer(void *ptr) {
  int thread_id = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  printf("Producer %d starting\n", thread_id);
  fflush(NULL);

  for (size_t i = 0; i < strlen(g_prod_str); i++) {
    sem_wait(&g_full_sem);
    pthread_mutex_lock(&g_buffer_mutex);

    // Add each character to the circular buffer
    // Modulus is used to maintain the circular nature of the buffer

    g_buffer.buf[g_buffer.tail] = g_prod_str[i];
    g_buffer.tail = (g_buffer.tail + 1) % BUF_SIZE;

    printf("Thread %d produced %c\n", thread_id, g_prod_str[i]);

    pthread_mutex_unlock(&g_buffer_mutex);
    sem_post(&g_empty_sem);
  }

  pthread_exit(EXIT_SUCCESS);
} /* producer() */

/*
 * Consume characters from the g_buffer. Stop after consuming the length of
 * g_prod_str, meaning that if an equal number of consumers and producers are
 * started, they will all exit. Signal producers appropriately of new free
 * space in the g_buffer. Receive and ID as an argument via an (int *).
 */

void *consumer(void *ptr) {
  int thread_id = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  printf("Consumer %d starting\n", thread_id);
  fflush(NULL);

  for (size_t i = 0; i < strlen(g_prod_str); i++) {
    sem_wait(&g_empty_sem);
    pthread_mutex_lock(&g_buffer_mutex);

    // Consume each character from the circular buffer
    // Modulus is used to maintain the circular nature of the buffer

    char c = g_buffer.buf[g_buffer.head];
    g_buffer.head = (g_buffer.head + 1) % BUF_SIZE;

    printf("Thread %d consumed %c\n", thread_id, c);

    pthread_mutex_unlock(&g_buffer_mutex);
    sem_post(&g_full_sem);
  }

  pthread_exit(EXIT_SUCCESS);
} /* consumer() */

/*
 * Start a number of producers indicated by the first argument, and a number
 * of consumers indicated by the second argument.
 * Wait on all threads at the end to prevent premature exit.
 */

int main(int argc, char **argv) {
  if (argc != REQUIRED_NUM_ARGS) {
    fprintf(stderr, "Please pass two arguments.\n");
    exit(EXIT_FAILURE);
  }

  int num_producers = atoi(argv[1]);
  int num_consumers = atoi(argv[2]);

  if ((num_producers <= 0) || (num_consumers <= 0)) {
    fprintf(stderr, "Arguments must be a valid integer greater than 0.\n");
    exit(EXIT_FAILURE);
  }

  // Initialize the g_buffer_mutex and condition variables

  pthread_mutex_init(&g_buffer_mutex, NULL);

  // Since the g_buffer starts out with no characters,
  // g_empty_sem should be 0 and g_full_sem should be the full
  // size of the g_buffer.

  sem_init(&g_empty_sem, 0, 0);
  sem_init(&g_full_sem, 0, BUF_SIZE);

  // Sets head and tail for circular buffer equal to 0, the start of the array

  g_buffer.head = 0;
  g_buffer.tail = 0;

  /* Creates 'num_producers' threads for the producers. */

  pthread_t * producers_threads = malloc(num_producers * sizeof(pthread_t));

  for (int i = 0; i < num_producers; i++) {
    int * t_id = malloc(sizeof(char));
    *t_id = i;
    pthread_create(&producers_threads[i], NULL, (void * (*)(void *)) producer,
        t_id);
  }

  /* Create 'num_consumers' threads for the consumers. */

  pthread_t * consumers_threads = malloc(num_consumers * sizeof(pthread_t));

  for (int i = 0; i < num_consumers; i++) {
    int * t_id = malloc(sizeof(char));
    *t_id = i;
    pthread_create(&consumers_threads[i], NULL, (void * (*)(void *)) consumer,
        t_id);
  }

  /* Join both producer and consumer threads */

  for (int i = 0; i < num_producers; i++) {
    pthread_join(producers_threads[i], NULL);
  }

  /*
  for (int i = 0; i < num_consumers; i++) {
    pthread_join(consumers_threads[i], NULL);
  }
  */

  pthread_mutex_destroy(&g_buffer_mutex);

  return EXIT_SUCCESS;
} /* main() */
