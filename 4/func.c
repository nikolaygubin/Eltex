#include "func.h"

const char menu_text[] =
    "-Add user : press 'a'\n-Delete user : press d\n-Find user : press "
    "f\n-Print users : press p\n-Quit : press q\n";

void print_menu() { write(1, menu_text, strlen(menu_text)); }

void clear_scrin() { write(1, "\E[H\E[J", strlen("\E[H\E[J")); }

void add_user(Person *phone_book) {
  set_term_regime(0, 2, 20, 1, 0);

  int free_index = 0;
  for (; free_index < SIZE; free_index++) {
    if (!strcmp(phone_book[free_index].first_name, "\0") ||
        !strcmp(phone_book[free_index].last_name, "\0") ||
        !strcmp(phone_book[free_index].number, "\0"))
      break;
  }
  if (free_index == SIZE) {
    clear_scrin();
    write(1, "Phone book is overflow!", strlen("Phone book is overflow!"));
    sleep(1);
    return;
  }

  write(1, "Enter first name : ", strlen("Enter first name : "));
  scanf("%s", phone_book[free_index].first_name);

  write(1, "Enter last name : ", strlen("Enter last name : "));
  scanf("%s", phone_book[free_index].last_name);

  write(1, "Enter number : ", strlen("Enter number : "));
  scanf("%s", phone_book[free_index].number);

  write(1, "Contact succefully add!\nPress any to back to menu",
        strlen("Contact succefully add!\nPress any to back to menu"));

  set_term_regime(0, 0, 1, 0, 0);

  char simbol;
  read(1, &simbol, 1);
}

void remove_user(Person *phone_book) {
  for (int i = 0; i < SIZE; i++) {
    if (phone_book[i].first_name[0] != '\0' ||
        phone_book[i].last_name[0] != '\0' || phone_book[i].number[0] != '\0') {
      char user[150];
      sprintf(user, "%d) First name : %s\nSecond name : %s\nNumber : %s\n\n",
              i + 1, phone_book[i].first_name, phone_book[i].last_name,
              phone_book[i].number);
      write(1, user, strlen(user));
    }
  }
  write(1, "Enter number identificator to you want delete : ",
        strlen("Enter number identificator to you want delete : "));

  char number;
  read(1, &number, 1);
  if (isdigit(number) && number > '0' && number - '0' <= SIZE) {
    phone_book[number - '0' - 1].first_name[0] = '\0';
    phone_book[number - '0' - 1].last_name[0] = '\0';
    phone_book[number - '0' - 1].number[0] = '\0';
    write(1, "\nContact succefully remove!\nPress any to back to menu",
          strlen("\nContact succefully remove!\nPress any to back to menu"));
    read(1, &number, 1);
    return;
  } else {
    write(
        1,
        "\nCan't delete contact! Check identificator!\nPress a to try again or "
        "press any to back menu!\n",
        strlen("\nCan't delete contact! Check identificator!\nPress a to try "
               "again or "
               "press any to back menu!\n"));
  }
  char command;
  read(1, &command, 1);
  if (command == 'a') {
    clear_scrin();
    remove_user(phone_book);
  }
  return;
}

void find_user(Person *phone_book) {
  set_term_regime(0, 2, 20, 1, 0);
  
  write(1, "Enter first name to find : ", strlen("Enter first name to find : "));
  char first_name[20];
  scanf("%s", first_name);

  for (int i = 0; i < SIZE; i++) {
    if (!strcmp(phone_book[i].first_name, first_name)) {
      char user[150];
      sprintf(user, "%d) First name : %s\nSecond name : %s\nNumber : %s\n\n",
              i + 1, phone_book[i].first_name, phone_book[i].last_name,
              phone_book[i].number);
      write(1, user, strlen(user));
    }
  }

  set_term_regime(0, 0, 1, 0, 0);

  write(1, "Press any to back menu!", strlen("Press any to back menu!"));
  char simbol;
  read(1, &simbol, 1);
}

void print_users(Person *phone_book) {
  for (int i = 0; i < SIZE; i++) {
    if (phone_book[i].first_name[0] != '\0' ||
        phone_book[i].last_name[0] != '\0' || phone_book[i].number[0] != '\0') {
      char user[150];
      sprintf(user, "%d) First name : %s\nSecond name : %s\nNumber : %s\n\n",
              i + 1, phone_book[i].first_name, phone_book[i].last_name,
              phone_book[i].number);
      write(1, user, strlen(user));
    }
  }
  write(1, "Press any to back menu!", strlen("Press any to back menu!"));
  char simbol;
  read(1, &simbol, 1);
}

int on_clicked_process(Person *phone_book) {
  char key;

  read(1, &key, 1);

  switch (key) {
    case 'a':
      clear_scrin();
      add_user(phone_book);
      clear_scrin();
      break;

    case 'd':
      clear_scrin();
      remove_user(phone_book);
      clear_scrin();
      break;

    case 'f':
      clear_scrin();
      find_user(phone_book);
      clear_scrin();
      break;

    case 'p':
      clear_scrin();
      print_users(phone_book);
      clear_scrin();
      break;

    case 'q':
      clear_scrin();
      return 1;

    default:
      clear_scrin();
  }
  return 0;
}

int set_term_regime(int regime, int vtime, int vmin, int echo, int sigint) {
  struct termios options;
  if (tcgetattr(1, &options)) {
    return -1;
  }

  if (regime)
    options.c_lflag |= ICANON;
  else {
    options.c_lflag &= ~ICANON;
    options.c_cc[VTIME] = vtime;
    options.c_cc[VMIN] = vmin;
    if (echo) {
      options.c_lflag |= ECHO;
    } else {
      options.c_lflag &= ~ECHO;
    }
    if (sigint) {
      options.c_lflag |= ISIG;
    } else {
      options.c_lflag &= ~ISIG;
    }
  }
  tcsetattr(1, TCSAFLUSH, &options);

  return 0;
}