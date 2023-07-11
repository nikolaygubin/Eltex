#include <curses.h>
#include <fcntl.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#define ROWS 25
#define COLS 140

char* to_string(char* str_num, int num)
{
  int num_digit = 0, copy_num = num;
  while (copy_num > 0) {
    copy_num /= 10;
    num_digit++;
  }

  str_num[num_digit] = '\0';
  for (; num_digit > 0; num_digit--) {
    str_num[num_digit - 1] = num % 10 + '0';
    num /= 10;
  }

  return str_num;
}

void sig_winch() {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

int main(int argc, char **argv) {
  if (argc == 1) return 1;
  WINDOW *win_main;

  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  noecho();
  wclear(stdscr);
  wrefresh(stdscr);

  start_color();
  win_main = newwin(ROWS, COLS, 2, 2);
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  wattron(win_main, COLOR_PAIR(1));
  wbkgd(win_main, COLOR_PAIR(1));
  box(win_main, '|', '-');
  wmove(win_main, 1, 1);
  wprintw(win_main, "file : %s", argv[1]);
  wmove(win_main, 3, 4);

  int fd = open(argv[1], O_RDONLY);

  if (!fd) {
    delwin(win_main);
    endwin();
    exit(EXIT_SUCCESS);
  }

  struct stat file_stat_buf;
  fstat(fd, &file_stat_buf);
  int size = file_stat_buf.st_size;

  char *buf = malloc(size);
  size = read(fd, buf, size);
  if (size == 0) {
    buf[0] = '\0';
  }
  close(fd);

  char **text = malloc(sizeof(char **) * (ROWS - 4));
  for (int i = 0; i < ROWS - 4; i++) {
    text[i] = malloc(COLS - 2);
    text[i] = memset(text[i], ' ', COLS - 2);
    text[i][COLS - 4 - 1] = '\n';
  }

  int col = 0, row = 0;
  for (size_t i = 0; i < strlen(buf); i++) {
    if (buf[i] == '\n') {
      col = 0;
      row++;
      wmove(win_main, getcury(win_main) + 1, 4);
      if (row == ROWS - 6)  break;
      continue;
    }
    wprintw(win_main, "%c", buf[i]);
    text[row][col++] = buf[i];
  }
  for (int i = 1; i < ROWS - 4; i++) {
    wmove(win_main, i + 2, 1);
    char *num = malloc(4);
    wprintw(win_main, "%s", to_string(num, i));
  }
  wmove(win_main, 3, 4);

  free(buf);

  wrefresh(win_main);

  int symbol;
  keypad(win_main, TRUE);
  while (1) {
    symbol = wgetch(win_main);
    if (symbol == ':') {
      wmove(win_main, getmaxy(win_main) - 2, 1);
      wprintw(win_main, ":");
      symbol = wgetch(win_main);
      if (symbol == 'q') {
        wprintw(win_main, "q");
        wrefresh(win_main);
        sleep(1);
        break;
      } else if (symbol == 'r') {
        wprintw(win_main, "r");
        wrefresh(win_main);
        sleep(1);
        FILE *file = fopen(argv[1], "w");
        for (size_t i = 0; i < ROWS - 5; i++) {
          fwrite(text[i], 1, COLS - 4, file);
        }
        break;
      }
    }
    switch (symbol) {
      case KEY_UP:
        if (getcury(win_main) == 3)
          wmove(win_main, getmaxy(win_main) - 3, getcurx(win_main));
        else
          wmove(win_main, getcury(win_main) - 1, getcurx(win_main));
        break;

      case KEY_DOWN:
        if (getcury(win_main) == getmaxy(win_main) - 3)
          wmove(win_main, 3, getcurx(win_main));
        else
          wmove(win_main, getcury(win_main) + 1, getcurx(win_main));
        break;

      case KEY_LEFT:
        if (getcurx(win_main) == 4)
          wmove(win_main, getcury(win_main), getmaxx(win_main) - 2);
        else
          wmove(win_main, getcury(win_main), getcurx(win_main) - 1);
        break;

      case KEY_RIGHT:
        if (getcurx(win_main) == getmaxx(win_main) - 2)
          wmove(win_main, getcury(win_main), 4);
        else
          wmove(win_main, getcury(win_main), getcurx(win_main) + 1);
        break;
      case KEY_BACKSPACE:
        if (getcurx(win_main) == 4) {
          wmove(win_main, getcury(win_main) - 1, strlen(text[getcury(win_main) - 4]));
        }
        wmove(win_main, getcury(win_main), getcurx(win_main) - 1);
        text[getcury(win_main) - 3][getcurx(win_main) - 4] = ' '; 
        wprintw(win_main, " ");
        wmove(win_main, getcury(win_main), getcurx(win_main) - 1);
        break;
      default:
        if (symbol == '\n') {
          wmove(win_main, getcury(win_main) + 1, 4);
          break;
        }
        if (getcurx(win_main) == getmaxx(win_main) - 2)
          wmove(win_main, getcury(win_main) + 1, 4);
        text[getcury(win_main) - 3][getcurx(win_main) - 4] = symbol;
        wprintw(win_main, "%c", symbol);
        break;
    }
  }
  delwin(win_main);
  endwin();
  exit(EXIT_SUCCESS);
}