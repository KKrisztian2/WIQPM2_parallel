#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    omp_set_num_threads(1);
    int n = 7;
    int tomb[n];
    tomb[0] = 5; tomb[1] = 1; tomb[2] = 6; tomb[3] = 3; tomb[4] = 7; tomb[5] = 4; tomb[6] = 2;   //5, 1, 6, 3, 7, 4, 2

    int a[n];

    for(int i = 0; i < n - 1; i++){
        a[i] = 0;
    }
    for(int i = 0; i < n - 1; i++){
        a[tomb[i] - 1] = tomb[i+1];
    }

    int output[n];
    #pragma omp parallel for
    for(int i = 0; i < n; i++){
        if(a[i] != 0){
            output[i] = 1;
        }
        else{
            output[i] = 0;
        }
    }

    for(int j = 0; j < 3; j++){
        #pragma omp parallel for
        for(int i = 0; i < n; i++){
            if(a[i] != 0){
                output[i] = output[i] + output[a[i]-1];
                a[i] = a[a[i]-1];
            }
        }
    }

    for(int i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%d ", output[i]);
    }



    return 0;
}
