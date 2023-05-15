#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_THREADS 8
#define NUM_POINTS 10000000

typedef struct {
    int thread_num;
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double* sum;
} ThreadData;

double f(double x) {
    return (4*pow(x,3) - 2*pow(x,2) + 13*x - 23);
}

DWORD WINAPI MonteCarlo(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    double sum = 0;
    srand((unsigned int)time(NULL) + data->thread_num);
    for (int i = 0; i < NUM_POINTS; i++) {
        double x = ((double)rand() / (double)RAND_MAX) * (data->x_max - data->x_min) + data->x_min;
        double y = ((double)rand() / (double)RAND_MAX) * (data->y_max - data->y_min) + data->y_min;
        if (f(x) >= 0 && y <= f(x) && y >= 0) {
            sum += 1;
        }
        else if(f(x) < 0 && y >= f(x) && y < 0){
            sum -= 1;
        }
    }
    *(data->sum) += sum;
    return 0;
}

int main() {
    clock_t start = clock();
    HANDLE threads[NUM_THREADS];
    ThreadData data[NUM_THREADS];
    double sum = 0.0;
    double integral_estimate;
    double x1, x2, y1, y2;
    x1 = -1.0;
    x2 = 4.0;
    y1 = f(x1);
    if(y1 > 0)
        y1 = 0;
    y2 = f(x2);

    for (int i = 0; i < NUM_THREADS; i++) {
        data[i].thread_num = i;
        data[i].x_min = x1;
        data[i].x_max = x2;
        data[i].y_min = y1;
        data[i].y_max = y2;
        data[i].sum = &sum;
        threads[i] = CreateThread(NULL, 0, MonteCarlo, &data[i], 0, NULL);
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
    }
    integral_estimate = (sum / (NUM_POINTS * NUM_THREADS)) * (y2 - y1) * (x2 - x1);
    printf("Estimated value of the integral: %f\n", integral_estimate);

    clock_t end = clock();
    double time = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nIdo: %f\n", time);

    return 0;
}
