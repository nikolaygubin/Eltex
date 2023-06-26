#include "func.h"

int main() {
  Person *phone_book = malloc(sizeof(Person) * book_capacity);

  clear_scrin();
  set_term_regime(0, 0, 1, 0, 0);

  while (1) {
    print_menu();
    if (on_clicked_process(&phone_book)) break;
  }
  set_term_regime(0, 0, 0, 1, 1);
  set_term_regime(1, 0, 0, 0, 0);

  free(phone_book);

  return 0;
}