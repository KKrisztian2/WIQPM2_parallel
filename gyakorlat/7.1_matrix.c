#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int egyseg(int** matrix, int n) {

    int i, j;
    int e = 1;
    #pragma omp parallel for private(j) shared(e)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j && matrix[i][j] != 1) {
                e = 0;
            }
            if (i != j && matrix[i][j] != 0) {
                e = 0;
            }
        }
    }
    return e;
}

int diagonalis(int** matrix, int n) {
    int i, j;
    int d = 1;
    #pragma omp parallel for private(j)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j && matrix[i][j] != 0) {
                d = 0;
            }
        }
    }
    return d;
}

int szimmetrikus(int** matrix, int n) {
    int i, j;
    int sz = 1;
    #pragma omp parallel for private(j)
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                sz = 0;
            }
        }
    }
    return sz;
}


int main()
{
    srand(time(0));

    omp_set_num_threads(4);
    int n = 10;
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }

    int e = egyseg(matrix, n);
    if (e == 1) {
        printf("A matrix egyseg matrix.\n");
    } else {
        printf("A matrix nem egyseg matrix.\n");
    }
    int d = diagonalis(matrix, n);
    if (d == 1) {
        printf("A matrix diagonalis.\n");
    } else {
        printf("A matrix nem diagonalis.\n");
    }

    int sz = szimmetrikus(matrix, n);
    if (sz == 1) {
        printf("A matrix szimmetrikus.\n");
    } else {
        printf("A matrix nem szimmetrikus.\n");
    }

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
