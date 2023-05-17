#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <math.h>

#define ARRAY_SIZE 1000000
#define MAX_RECURSION_DEPTH 16

struct TData {
    double* array;
    int size;
    double result;
};

double seq(double* t, int n) {
    double res = 1.0;
    for (int i = 0; i < n; i++) {
        res *= t[i];
    }
    return res;
}

double rec(double* array, int start, int end) {
    if (start == end) {
        return array[start];
    }
    int m = (start + end) / 2;
    double left = rec(array, start, m);
    double right = rec(array, m + 1, end);
    return left * right;
}

void* pthr(void* data){
    struct TData* d = (struct TData*)data;
    d->result = seq(d->array, d->size);
}


int main()
{
    srand(time(0));
    int n = 10;
    double* t = (double*)malloc(n * sizeof(double));
    for(int i = 0; i < n; i++){
        t[i] = (double)rand()/RAND_MAX*10.0-5.0;
        printf("%lf ", t[i]);
    }
    printf("\n");

    double seq_res = seq(t, n);
    printf("Seq: %lf\n", seq_res);

    double rec_res = rec(t, 0, n-1);
    printf("Rec: %lf\n", rec_res);

    int th_num = 4;

    pthread_t threads[th_num];
    struct TData data[th_num];
    double res[th_num];
    for (int i = 0; i < th_num; i++) {
        int start = floor(n/th_num) * i;
        data[i].size = floor(n/th_num);
        data[i].array = &t[start];
        data[i].result = 1.0;
        if(i == th_num-1){
            data[i].size = (int)floor(n/th_num) + (n % th_num);
        }

        pthread_create(&threads[i], NULL, pthr, &data[i]);
    }
    double pth_res = 1.0;
    for (int i = 0; i < th_num; i++) {
        pthread_join(threads[i], NULL);
        pth_res *= data[i].result;
    }
    printf("PThread: %lf\n", pth_res);

    double omp_res = 1.0;
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        #pragma omp critical
        omp_res *= t[i];
    }
    printf("Openmp: %lf\n", omp_res);

    double omp_res_red = 1.0;
    #pragma omp parallel for reduction(*:omp_res_red)
    for (int i = 0; i < n; i++) {
        omp_res_red *= t[i];
    }
    printf("Openmp red: %lf\n", omp_res_red);




    return 0;
}
