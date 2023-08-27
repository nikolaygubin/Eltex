#include <stdio.h>

#define N 15

void print_matrix(int matrix[N][N])
{
    for (int rows = 0; rows < N; rows++) {
        for (int cols = 0; cols < N; cols++)
            printf("%3d ", matrix[rows][cols]);
        putchar('\n');
    }
}

void print_array(int *array)
{
    for (int i = 0; i < N; i++) 
        printf("%d ", array[i]);
    putchar('\n');
}

int main()
{
    #if 0 // first task
        int matrix[N][N];

        for (int rows = 0; rows < N; rows++)
            for (int cols = 0; cols < N; cols++)
                matrix[rows][cols] = rows * N + cols + 1;
        print_matrix(matrix);
    #endif

    #if 0 // second task
        int array[N];

        for (int i = 0; i < N; i++) {
            array[i] = i + 1;
        }
        print_array(array);
        
        for (int i = 0; i < N / 2; i++) {
            int temp = array[i];
            array[i] = array[N - i - 1]; 
            array[N - i - 1] = temp;
        }
        print_array(array);
        
    #endif

    #if 0 // third task
        int matrix[N][N];

        for (int rows = 0; rows < N; rows++)
            for (int cols = 0; cols < N; cols++)
                matrix[rows][cols] = (cols < N - rows - 1) ? 0 : 1;
        print_matrix(matrix);
    #endif

    #if 1 // fourth task
        int matrix[N][N], rows = 0, cols = 0, number = 1, layer = 0;

        while (number - 1 != N * N) {
            while (cols < N - layer)
                matrix[rows][cols++] = number++;
            cols--, rows++;

            while (rows < N - layer)
                matrix[rows++][cols] = number++;
            rows--, cols--;

            while (cols > layer)
                matrix[rows][cols--] = number++;

            while (rows > layer)
                matrix[rows--][cols] = number++;

            rows = cols = ++layer;
        }
        print_matrix(matrix);  
    #endif

    return 0;
}
