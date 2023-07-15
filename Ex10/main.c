#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 1000

long a = 0;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_calc(void* args) {
  // pthread_mutex_lock(&mutex);
  for (int i = 0; i < 100000; i++) a++;
  // pthread_mutex_unlock(&mutex);
  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, thread_calc, NULL);
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("a = %ld\n", a);

  return 0;
}