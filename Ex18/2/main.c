#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
  if (argc != 2) {
    printf("Usage : ./main <num_clients>\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < atoi(argv[1]); i++) {
    printf("%d\n", i);
    system("./client");
  }

  exit(EXIT_SUCCESS);
}
