#include <locale.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define NUM_BUYER 3
#define NUM_SHOPS 5

void sig_winch();
void print_buyers();
void print_shops();

void *start_buy(void *args);
void *append_process(void *args);

WINDOW *win;
int shop_resource[5] = {1000, 1000, 1000, 1000, 1000};
int buyers[NUM_BUYER] = {10000, 10000, 10000};
int buyers_index[3] = {0, 1, 2};
pthread_mutex_t shop_mutexes[5];

int main() {
  int key;
  pthread_t buyer_threads[NUM_BUYER], loader_thread;
  struct winsize size;

  for (int i = 0; i < NUM_SHOPS; i++)
    pthread_mutex_init(&shop_mutexes[i], NULL);

  ioctl(fileno(stdout), TIOCGWINSZ, &size);

  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  noecho();

  win = newwin(size.ws_row - 3, 90, 1, 40);

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_GREEN);
  init_pair(2, COLOR_GREEN, COLOR_BLUE);
  wbkgd(win, COLOR_PAIR(1));
  wbkgd(stdscr, COLOR_PAIR(2));
  refresh();
  print_shops();
  print_buyers();
  wrefresh(win);

  srand(time(NULL));
  keypad(win, TRUE);
  srand(time(NULL));
  while (1) {
    switch (key = wgetch(win)) {
      case '\n':
        for (int i = 0; i < NUM_BUYER; i++)
          pthread_create(&buyer_threads[i], NULL, start_buy, &buyers_index[i]);
        pthread_create(&loader_thread, NULL, append_process, NULL);

        for (int i = 0; i < NUM_BUYER; i++)
          pthread_join(buyer_threads[i], NULL);
        pthread_join(loader_thread, NULL);
        break;
      default:
        break;
    }
    if (key == 'q') break;
  }

  for (int i = 0; i < NUM_SHOPS; i++) pthread_mutex_destroy(&shop_mutexes[i]);

  delwin(win);
  endwin();
  exit(EXIT_SUCCESS);
}

void sig_winch() {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

int is_end() { return buyers[0] + buyers[1] + buyers[2] + buyers[3]; }

void *start_buy(void *args) {
  int index = *((int *)args);

  while (buyers[index] > 0) {
    for (int i = 0; i < NUM_SHOPS; i++) {
      if (pthread_mutex_trylock(&shop_mutexes[i]) == 0) {
        if (shop_resource[i] >= 500) {
          buyers[index] -= 500;
          shop_resource[i] -= 500;
          sleep(2 + rand() % 3);
          pthread_mutex_unlock(&shop_mutexes[i]);
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
      if (pthread_mutex_trylock(&shop_mutexes[i]) == 0) {
        shop_resource[i] += 500;
        sleep(1);
        pthread_mutex_unlock(&shop_mutexes[i]);
      }
      wclear(win);
      print_buyers();
      print_shops();
    }
  }
  return NULL;
}

void print_buyers() {
  wmove(win, 6, 10);
  wprintw(win, "Buyer's need:");
  for (int i = 0; i < NUM_BUYER; i++) {
    wmove(win, 8 + i * 2, 10);
    wprintw(win, "Buyer %d : %d", i + 1, buyers[i]);
  }
  wrefresh(win);
}
void print_shops() {
  wmove(win, 6, 60);
  wprintw(win, "Shop resourse :");
  for (int i = 0; i < NUM_SHOPS; i++) {
    wmove(win, 8 + i * 2, 60);
    wprintw(win, "Shop %d : %d\n", i + 1, shop_resource[i]);
  }
  wrefresh(win);
}