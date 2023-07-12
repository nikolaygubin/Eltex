#include <curses.h>
#include <dirent.h>
#include <fcntl.h>
#include <locale.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define MAX_PATH 256
#define BEGIN_FILES 4

WINDOW *left_win, *right_win;
char *left_path, *right_path;
struct dirent left_files[10], right_files[10];

void sig_winch();
void init_child_wins();
int print_window(WINDOW *win, char **path, struct dirent *files);
void delete_last_file(char **path);

int main() {
  setlocale(LC_ALL, "Russian");

  WINDOW *cur_win;
  struct dirent *cur_files;
  char *cur_path;
  struct winsize size;
  int length_left = 0, length_right = 0, cur_length = 0;

  left_path = getenv("PWD");
  right_path = malloc(strlen(left_path));
  strcpy(right_path, left_path);

  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  noecho();

  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);

  init_child_wins();
  length_left = print_window(left_win, &left_path, left_files);
  length_right = print_window(right_win, &right_path, right_files);

  wrefresh(left_win);
  wrefresh(right_win);

  keypad(left_win, TRUE);
  keypad(right_win, TRUE);

  cur_win = left_win;
  cur_path = left_path;
  cur_files = left_files;
  cur_length = length_left;
  wmove(cur_win, 4, 37);
  while (1) {
    int key = wgetch(cur_win);
    if (key == 'q') break;
    switch (key) {
      case '\t':
        if (cur_win == left_win) {
          cur_win = right_win;
          cur_path = right_path;
          cur_files = right_files;
          cur_length = print_window(cur_win, &cur_path, cur_files);
        } else {
          cur_win = left_win;
          cur_path = left_path;
          cur_files = left_files;
          cur_length = print_window(cur_win, &cur_path, cur_files);
        }
        wmove(cur_win, 4, 37);
        break;
      case '\n':
        int index = getcury(cur_win) - 4;
        if (cur_files[index].d_type == DT_DIR) {
          if (!strcmp(cur_files[index].d_name, "..")) {
            delete_last_file(&cur_path);
            wclear(cur_win);
            cur_length = print_window(cur_win, &cur_path, cur_files);
            wmove(cur_win, 4, 37);
          } else {
            strcat(cur_path, "/");
            strcat(cur_path, cur_files[index].d_name);
            wclear(cur_win);
            cur_length = print_window(cur_win, &cur_path, cur_files);
            wmove(cur_win, 4, 37);
          }
        } else {
          pid_t child_pid;
          int status;
          child_pid = fork();
          if (child_pid == 0) {
            char *path_to_file = malloc(256);
            strcat(path_to_file, cur_path);
            strcat(path_to_file, "/");
            strcat(path_to_file, cur_files[index].d_name);
            execl("./../Ex7/editor", "./editor", path_to_file, NULL);
            free(path_to_file);
            exit(5);
          } else {
            wait(&status);
            wclear(left_win);
            wclear(right_win);
            keypad(left_win, TRUE);
            keypad(right_win, TRUE);
            print_window(right_win, &right_path, right_files);
            cur_length = print_window(left_win, &left_path, left_files);
          }
        }

        break;
      case KEY_UP:
        if (BEGIN_FILES != getcury(cur_win))
          wmove(cur_win, getcury(cur_win) - 1, 37);
        // init_pair(3, COLOR_WHITE, COLOR_BLACK);
        // wattron(left_win, COLOR_PAIR(3));
        // wprintw(cur_win, "%s", cur_files[getcury(cur_win) - 4].d_name);
        // wattroff(left_win, COLOR_PAIR(3));
        break;
      case KEY_DOWN:
        if ((BEGIN_FILES + (cur_length - 1)) != getcury(cur_win))
          wmove(cur_win, getcury(cur_win) + 1, 37);
        // wattron(right_win, COLOR_PAIR(3));
        // wprintw(cur_win, "%s", cur_files[getcury(cur_win) - 4].d_name);
        // wattroff(right_win, COLOR_PAIR(3));
        break;
      default:
        break;
    }
    wrefresh(cur_win);
  }

  wclear(left_win);
  wclear(right_win);
  wrefresh(left_win);
  wrefresh(right_win);
  delwin(left_win);
  delwin(right_win);
  endwin();
  exit(EXIT_SUCCESS);
}

void sig_winch() {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
  init_child_wins();
  print_window(right_win, &right_path, right_files);
  print_window(left_win, &left_path, left_files);
  wrefresh(left_win);
  wrefresh(right_win);
}

void init_child_wins() {
  wclear(stdscr);
  wrefresh(stdscr);
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);

  int middle = size.ws_col / 2;
  right_win = newwin(size.ws_row - 2, size.ws_col - 2, 1, middle);
  left_win = newwin(size.ws_row - 2, middle - 2, 1, 1);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);

  wbkgd(left_win, COLOR_PAIR(1));
  wbkgd(right_win, COLOR_PAIR(1));
}

int print_window(WINDOW *win, char **path, struct dirent *files) {
  wmove(win, 1, 2);
  wattron(win, COLOR_PAIR(2));
  wprintw(win, "Путь: %s", *path);
  wattroff(win, COLOR_PAIR(2));
  wbkgd(win, COLOR_PAIR(1));
  wmove(win, 3, 37);
  wprintw(win, "Filename");

  DIR *cur_dir = opendir(*path);

  struct dirent *cur_file;

  wmove(win, 4, 37);
  int length = 0;
  while ((cur_file = readdir(cur_dir))) {
    if (!strcmp(cur_file->d_name, ".")) continue;
    files[length] = *cur_file;
    wprintw(win, "%s", cur_file->d_name);
    wmove(win, getcury(win) + 1, 37);
    length++;
  }

  return length;
}

void delete_last_file(char **path) {
  int i = strlen(*path) - 1;
  while ((*path)[i] != '/') --i;
  (*path)[i] = '\0';
}
