#include <stdio.h>
#include <inttypes.h>

void print_hex(int number)
{
    for (int i = 3; i >= 0; i--) {
        uint8_t byte = number >> (8 * i);
        printf("%x ", byte);
    }    
    putchar('\n');
}

int main()
{
    int number = 0xddccbbaa, third_byte = 0xee;

    print_hex(number);
    number &= ~(0xff << 16);  // обнуляем 3 байт
    number |= third_byte << 16; // записываем в 3 байт заданный байт
    print_hex(number);
}