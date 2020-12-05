#include "part2.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MIN_NITROGEN_FOR_N2 (2)
#define MIN_OXYGEN_FOR_O2 (2)
#define MIN_N2_FOR_NO2 (1)
#define MIN_O2_FOR_NO2 (2)
#define MIN_O2_FOR_O3 (3)
#define REQUIRED_NUM_ARGS (3)
#define UNUSED(x) (void)(x)

// The number of molecules of each type
// (O, N, O2, and N2)

int g_num_oxygen = 0;
int g_num_nitrogen = 0;
int g_num_o2 = 0;
int g_num_n2 = 0;

// Mutex for each molecule count variable
// (O, N, O2, and N2)

pthread_mutex_t g_oxygen_mutex = {0};
pthread_mutex_t g_nitrogen_mutex = {0};
pthread_mutex_t g_o2_mutex = {0};
pthread_mutex_t g_n2_mutex = {0};

// The semaphores are named according to the molecule that waits on it.
// For example, create_o3 waits on g_sig_o3, which must be signaled by
// create_o2.

sem_t g_sig_o2 = {0};
sem_t g_sig_n2 = {0};
sem_t g_sig_no2 = {0};
sem_t g_sig_o3 = {0};

/*
 * Create oxygen atoms. The number of atoms to create is specified by the
 * argument, which is an int cast to (void *).
 */

void *create_oxygen(void *ptr) {
  int how_many = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  pthread_mutex_lock(&g_oxygen_mutex);

  for (int i = 0; i < how_many; i++) {
    printf("An atom of oxygen was created.\n");
    g_num_oxygen++;
  }

  pthread_mutex_unlock(&g_oxygen_mutex);

  // Signal consumers of oxygen that there are now oxygen molecules to consume
  // In this case, o2 is the only molecule that takes plain oxygen molecules

  sem_post(&g_sig_o2);

  pthread_exit(EXIT_SUCCESS);
} /* create_oxygen() */

/*
 * Create nitrogen atoms. The number of atoms to create is specified by the
 * argument, which is an int cast to (void *).
 */

void *create_nitrogen(void *ptr) {
  int how_many = *((int *) ptr);
  free(ptr);
  ptr = NULL;

  pthread_mutex_lock(&g_nitrogen_mutex);

  for (int i = 0; i < how_many; i++) {
    printf("An atom of nitrogen was created.\n");
    g_num_nitrogen++;
  }

  pthread_mutex_unlock(&g_nitrogen_mutex);

  // Signal consumers of nitrogen that there are now oxygen molecules to consume
  // In this case, n2 is the only molecule that takes plain nitrogen molecules

  sem_post(&g_sig_n2);

  pthread_exit(EXIT_SUCCESS);
} /* create_nitrogen() */

/*
 * Form N2 molecules, by combining nitrogen atoms.
 */

void *create_n2(void *ptr) {
  UNUSED(ptr);

  sem_wait(&g_sig_n2);

  while (1) {
    pthread_mutex_lock(&g_nitrogen_mutex);
    pthread_mutex_lock(&g_n2_mutex);

    if (g_num_nitrogen >= MIN_NITROGEN_FOR_N2) {
      printf("Two atoms of nitrogen combined to produce one molecule of N2.\n");
      g_num_nitrogen -= MIN_NITROGEN_FOR_N2;
      g_num_n2++;
    }

    pthread_mutex_unlock(&g_nitrogen_mutex);
    pthread_mutex_unlock(&g_n2_mutex);

    // Signals consumers of n2 that there are now n2 molecules to consume
    // In this case, no2 is the only molecule that consumes n2 molecules

    sem_post(&g_sig_no2);
  }
} /* create_n2() */

/*
 * Form O2 molecules, by combining oxygen atoms.
 */

void *create_o2(void *ptr) {
  UNUSED(ptr);

  sem_wait(&g_sig_o2);

  while (1) {
    pthread_mutex_lock(&g_oxygen_mutex);
    pthread_mutex_lock(&g_o2_mutex);

    if (g_num_oxygen >= MIN_OXYGEN_FOR_O2) {
      printf("Two atoms of oxygen combined to produce one molecule of O2.\n");
      g_num_oxygen -= MIN_OXYGEN_FOR_O2;
      g_num_o2++;
    }

    pthread_mutex_unlock(&g_oxygen_mutex);
    pthread_mutex_unlock(&g_o2_mutex);

    // Signals consumers of o2 that there are now o2 molecules to consume
    // In this case, no2 and o3 molecules both consume o2

    sem_post(&g_sig_no2);
    sem_post(&g_sig_o3);
  }
} /* create_o2() */

/*
 * Form NO2 molecules, by combining N2 and O2 molecules.
 */

void *create_no2(void *ptr) {
  UNUSED(ptr);

  sem_wait(&g_sig_no2);

  while (1) {
    pthread_mutex_lock(&g_n2_mutex);
    pthread_mutex_lock(&g_o2_mutex);

    if ((g_num_n2 >= MIN_N2_FOR_NO2) && (g_num_o2 >= MIN_O2_FOR_NO2)) {
      printf("One molecule of N2 and two molecules of O2 combined to "
             "produce two molecules of NO2.\n");
      g_num_n2 -= MIN_N2_FOR_NO2;
      g_num_o2 -= MIN_O2_FOR_NO2;
    }

    pthread_mutex_unlock(&g_n2_mutex);
    pthread_mutex_unlock(&g_o2_mutex);

    // There are no molecules that consume no2, so nothing to signal
  }
} /* create_no2() */

/*
 * Form O3 molecules, by combining O2 molecules.
 */

void *create_o3(void *ptr) {
  UNUSED(ptr);

  sem_wait(&g_sig_o3);

  while (1) {
    pthread_mutex_lock(&g_o2_mutex);

    if (g_num_o2 >= MIN_O2_FOR_O3) {
      printf("Three molecules of O2 combined to produce two molecules of"
             " O3.\n");
      g_num_o2 -= MIN_O2_FOR_O3;
    }

    pthread_mutex_unlock(&g_o2_mutex);

    // There are no molecules that consume o3, so nothing to signal
  }
} /* create_o3() */


/*
 * Create threads to run each chemical reaction. Wait on all threads, even
 * though many won't exit, to avoid any premature exit. The number of oxygen
 * atoms to be created is specified by the first command-line argument, and the
 * number of nitrogen atoms with the second.
 */

int main(int argc, char **argv) {
  if (argc != REQUIRED_NUM_ARGS) {
    fprintf(stderr, "Please pass two arguments.\n");
    exit(EXIT_FAILURE);
  }

  int * num_oxygen_atoms = malloc(sizeof(int));
  *num_oxygen_atoms = atoi(argv[1]);

  int * num_nitrogen_atoms = malloc(sizeof(int));
  *num_nitrogen_atoms = atoi(argv[2]);

  if ((*num_oxygen_atoms <= 0) || (*num_nitrogen_atoms <= 0)) {
    fprintf(stderr, "Arguments must be a valid integer greater than 0.\n");
    exit(EXIT_FAILURE);
  }

  // Initialize mutex for every molecule variable

  pthread_mutex_init(&g_oxygen_mutex, NULL);
  pthread_mutex_init(&g_nitrogen_mutex, NULL);
  pthread_mutex_init(&g_o2_mutex, NULL);
  pthread_mutex_init(&g_n2_mutex, NULL);

  sem_init(&g_sig_n2, 0, 0);
  sem_init(&g_sig_o2, 0, 0);
  sem_init(&g_sig_o3, 0, 0);
  sem_init(&g_sig_no2, 0, 0);

  // Code to create a thread for each molecule formation function

  pthread_t oxygen_thread = {0};
  pthread_create(&oxygen_thread, NULL, (void * (*)(void *)) create_oxygen,
                 num_oxygen_atoms);

  pthread_t nitrogen_thread = {0};
  pthread_create(&nitrogen_thread, NULL, (void * (*)(void *)) create_nitrogen,
                 num_nitrogen_atoms);

  pthread_t o2_thread = {0};
  pthread_create(&o2_thread, NULL, (void * (*)(void *)) create_o2, NULL);

  pthread_t n2_thread = {0};
  pthread_create(&n2_thread, NULL, (void * (*)(void *)) create_n2, NULL);

  pthread_t no2_thread = {0};
  pthread_create(&no2_thread, NULL, (void * (*)(void *)) create_no2, NULL);

  pthread_t o3_thread = {0};
  pthread_create(&o3_thread, NULL, (void * (*)(void *)) create_o3, NULL);


  // Waits until threads are complete before main continues, otherwise we run
  // the risk of executing an exit which will terminate the process and all
  // threads before the threads have completed.

  pthread_join(oxygen_thread, NULL);
  pthread_join(nitrogen_thread, NULL);
  pthread_join(o2_thread, NULL);
  pthread_join(n2_thread, NULL);
  pthread_join(no2_thread, NULL);
  pthread_join(o3_thread, NULL);

  pthread_mutex_destroy(&g_oxygen_mutex);
  pthread_mutex_destroy(&g_nitrogen_mutex);
  pthread_mutex_destroy(&g_o2_mutex);
  pthread_mutex_destroy(&g_n2_mutex);

  exit(EXIT_SUCCESS);
} /* main() */
