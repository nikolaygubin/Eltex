#include <stdio.h>

int main() {
    FILE *file = fopen("./test.b", "wb");

    int mas[7] = {0, 0, 0, 0, 0, 0x555551eb, 0x00005555};

    fwrite(mas, 4, 7, file);

    return 0;
}