#include <stdio.h>

void print_int_bytes(int *number)
{
    unsigned char *byte = (unsigned char *)number;

    for (int i = 0; i < 4; i++) {
        printf("%d ", byte[i]);
    }
    putchar('\n');
}

int main()
{
    int number = 0x01020304;

    print_int_bytes(&number);

    unsigned char *byte = (unsigned char *)&number;
    byte[1] = 0x05;
    
    print_int_bytes(&number);

    return 0;
}