#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double func(double x) {
    return 3*pow(x,2) + 4*x + 2;
}

double integral(double a, double b, int steps) {
    double s_size = (b - a) / steps;
    double sum = 0.0;
    int i;

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < steps; i++) {
        double x1 = a + i * s_size;
        double x2 = a + (i+1) * s_size;
        double f1 = func(x1);
        double f2 = func(x2);
        sum += ((x2-x1)/2) * (f1 + f2);
    }
    return sum;
}


int main() {
    double a = 0.0;
    double b = 10.0;
    int steps = 1000000;

    double result = integral(a, b, steps);
    printf("Az integral erteke: %f\n", result);

    return 0;
}
