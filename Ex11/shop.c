#include <locale.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_BUYER 3
#define NUM_SHOPS 5

void print_buyers();
void print_shops();

void *start_buy(void *args);
void *append_process(void *args);

int shop_resource[5] = {1000, 1000, 1000, 1000, 1000};
int buyers[NUM_BUYER] = {10000, 10000, 10000};
int buyers_index[3] = {0, 1, 2};
pthread_mutex_t shop_mutexes[5];

int main() {
  pthread_t buyer_threads[NUM_BUYER], loader_thread;

  for (int i = 0; i < NUM_SHOPS; i++)
    pthread_mutex_init(&shop_mutexes[i], NULL);

  for (int i = 0; i < NUM_BUYER; i++)
    pthread_create(&buyer_threads[i], NULL, start_buy, buyers_index + i);
  pthread_create(&loader_thread, NULL, append_process, NULL);

  for (int i = 0; i < NUM_BUYER; i++) pthread_join(buyer_threads[i], NULL);
  pthread_join(loader_thread, NULL);

  for (int i = 0; i < NUM_SHOPS; i++) pthread_mutex_destroy(&shop_mutexes[i]);

  return 0;
}

int is_end() { return buyers[0] + buyers[1] + buyers[2] + buyers[3]; }

void *start_buy(void *args) {
  int index = *((int *)args);

  while (buyers[index] > 0) {
    for (int i = 0; i < NUM_SHOPS; i++) {
      if (pthread_mutex_trylock(&shop_mutexes[i])) {
        if (shop_resource[i] > 500) {
          buyers[index] -= 500;
          shop_resource[i] -= 500;
          pthread_mutex_unlock(&shop_mutexes[i]);
          printf("Покупатель %d купил 500 из магазина %d. Осталось %d. Потребность : :%d\n",
                 index + 1, i + 1, shop_resource[i], buyers[index]);
          sleep(1);
          break;
        } else {
          pthread_mutex_unlock(&shop_mutexes[i]);
        }
      }
    }
  }
  return NULL;
}

void *append_process(void *args) {
  while (is_end()) {
    for (int i = 0; i < NUM_SHOPS; i++) {
      if (!pthread_mutex_trylock(&shop_mutexes[i])) {
        shop_resource[i] += 500;
        pthread_mutex_unlock(&shop_mutexes[i]);
        printf("Положил в магазин %d 500. Сейчас там %d\n", i + 1,
               shop_resource[i]);
        sleep(1);
      }
    }
  }
  return NULL;
}