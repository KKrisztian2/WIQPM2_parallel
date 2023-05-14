#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

typedef struct {
    int row;
    int size;
    double det;
    double **matrix;
} ThreadData;

typedef struct {
    double *m;
} Result;


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

void *inv(void *arg){
    ThreadData *data = (ThreadData*) arg;
    Result *res = (Result *) malloc(sizeof(Result));
    res->m = (double *) malloc(data->size * sizeof(double));

    for (int k = 0; k < data->size; k++) {
        double** submatrix = (double **)malloc(((data->size)-1) * sizeof(double *));
        for (int i = 0; i < (data->size)-1; i++) {
            submatrix[i] = (double *)malloc(((data->size)-1) * sizeof(double));
        }

        for (int i = 1+(data->row); i < data->size+(data->row); i++) {
            for (int j = 0; j < data->size; j++) {
                if (j < k) {
                    submatrix[i-(data->row)-1][j] = data->matrix[i%data->size][j];
                } else if (j > k) {
                    submatrix[i-(data->row)-1][j-1] = data->matrix[i%data->size][j];
                }
            }
        }

        double A = determinant(submatrix, (data->size)-1);
        if(data->size%2 == 0){
            if(data->row%2==0)
                res->m[k] = A * pow(-1, k + 1) / data->det * (-1);
            else
                res->m[k] = A * pow(-1, k + 1) / data->det;
        }
        else{
            res->m[k] = A * pow(-1, k) / data->det;
        }
        for (int i = 0; i < (data->size)-1; i++) {
            free(submatrix[i]);
        }
        free(submatrix);
    }

    pthread_exit(res->m);
}

int main() {
    srand(time(0));
    int size;
    printf("Enter the size of the matrix: ");
    scanf("%d", &size);
    clock_t start = clock();
    double **matrix=(double **)malloc(size*sizeof(double *));
    for (int i = 0; i < size; i++) {
        matrix[i]=(double *)malloc(size*sizeof(double));
    }

    double **inv_matrix=(double **)malloc(size*sizeof(double *));
    for (int i = 0; i < size; i++) {
        inv_matrix[i]=(double *)malloc(size*sizeof(double));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            //matrix[i][j] = rand()%10-5;
            matrix[i][j] = (double)rand()/RAND_MAX*20.0-10.0;
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
    double d = determinant(matrix, size);
    printf("Determinant: %lf\n", d);

    if(d != 0 && size > 1){
        pthread_t threads[size];
        ThreadData data[size];
        Result res[size];
        for (int i = 0; i < size; i++) {
            data[i].row = i;
            data[i].size = size;
            data[i].matrix = matrix;
            data[i].det = d;

            pthread_create(&threads[i], NULL, inv, &data[i]);
        }
        for (int i = 0; i < size; i++) {
            pthread_join(threads[i], &res[i]);
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                inv_matrix[j][i] = res[i].m[j];
            }
        }
    }else if(d != 0 && size == 1){
        inv_matrix[0][0] = 1/matrix[0][0];
    }
    else{
        printf("Determinant = 0");
    }

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

    for (int i = 0; i < size; i++) {
        free(matrix[i]);
        free(inv_matrix[i]);
    }
    free(matrix);
    free(inv_matrix);

    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nIdo: %f\n", time);


    return 0;
}
