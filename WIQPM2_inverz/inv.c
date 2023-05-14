#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


double determinant(double **matrix, int n) {
    double det = 0;
    if (n == 1) {
        det = matrix[0][0];
    } else {
        for (int k = 0; k < n; k++) {
            double** submatrix = (double **)malloc((n-1) * sizeof(double *));
            for (int i = 0; i < n-1; i++) {
                submatrix[i] = (double *)malloc((n-1) * sizeof(double));
            }
            for (int i = 1; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (j < k) {
                        submatrix[i-1][j] = matrix[i][j];
                    } else if (j > k) {
                        submatrix[i-1][j-1] = matrix[i][j];
                    }
                }
            }
            det += matrix[0][k] * pow(-1, k) * determinant(submatrix, n-1);
            for (int i = 0; i < n-1; i++) {
                free(submatrix[i]);
            }
            free(submatrix);
        }
    }
    return det;
}

void inv(double **matrix, double **inv_matrix, int size, double d) {
    if (size == 1) {
        inv_matrix[0][0] = 1 / matrix[0][0];
        } else {
            double** submatrix = (double **)malloc((size - 1) * sizeof(double *));
        for (int i = 0; i < size - 1; i++) {
            submatrix[i] = (double *)malloc((size - 1) * sizeof(double));
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size - 1; k++) {
                    for (int l = 0; l < size - 1; l++) {
                        if (k < i && l < j) {
                            submatrix[k][l] = matrix[k][l];
                        } else if (k < i && l >= j) {
                            submatrix[k][l] = matrix[k][l + 1];
                        } else if (k >= i && l < j) {
                            submatrix[k][l] = matrix[k + 1][l];
                        } else if (k >= i && l >= j) {
                            submatrix[k][l] = matrix[k + 1][l + 1];
                        }
                    }
                }

                inv_matrix[j][i] = pow(-1, i + j) * determinant(submatrix, size - 1) / d;
            }
        }

        for (int i = 0; i < size - 1; i++) {
            free(submatrix[i]);
        }
        free(submatrix);
    }
}

int main() {
    srand(time(0));
    int size;
    printf("Enter the size of the matrix: ");
    scanf("%d", &size);
    clock_t start = clock();
    double **matrix = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (double *)malloc(size * sizeof(double));
    }

    double **inv_matrix = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        inv_matrix[i] = (double *)malloc(size * sizeof(double));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (double)rand()/RAND_MAX*20.0-10.0;
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }

    double d = determinant(matrix, size);
    printf("Determinant: %lf\n", d);

    if (d != 0){
        inv(matrix, inv_matrix, size, d);
        printf("\n");
        printf("The inverse of the matrix is:\n");

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                printf("%lf ", inv_matrix[i][j]);
            }
            printf("\n");
        }

        double test[size][size];
         for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                test[i][j] = 0;
            }
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    test[i][j] += matrix[i][k] * inv_matrix[k][j];
                }
            }
        }
        printf("\nTest matrix is:\n");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                printf("%lf ", test[i][j]);
            }
            printf("\n");
        }
    }
    else
        printf("Determinant = 0");


    for (int i = 0; i < size; i++) {
        free(matrix[i]);
        free(inv_matrix[i]);
    }
    free(matrix);
    free(inv_matrix);

    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nCPU ido (Maximum - manualis): %f\n", time);

    return 0;
}
