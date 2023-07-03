#ifndef FUNC_H
#define FUNC_H

#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  char first_name[20];
  char last_name[20];
  char number[20];
} Person;

extern size_t book_capacity;
extern size_t book_size;

int set_term_regime(int regime, int vtime, int vmin, int echo, int sigint);
void print_menu();
void clear_scrin();
int on_clicked_process(Person **phone_book);
void add_user(Person **phone_book);
void remove_user(Person *phone_book);
void find_user(Person *phone_book);
void print_users(Person *phone_book);

#endif