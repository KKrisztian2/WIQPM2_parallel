#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

struct Params{
    int* a;
    int s;
    int even;
    int odd;
    int null_a;
    int pos;
    int neg;
    int interval;
};

void* attr(void* data)
{
    pthread_t thread;
    thread = pthread_self();
    struct Params *p = (struct Params*)data;
    for(int i = 0; i < p->s; i++){
        if(p->a[i] % 2 == 0){
            p->even = p->even + 1;
        }else{
            p->odd = p->odd + 1;
        }
        if(p->a[i] == 0){
            p->null_a = p->null_a + 1;
        }
        if(p->a[i] < 0){
            p->neg = p->neg + 1;
        }else{
            p->pos = p->pos + 1;
        }
        if(p->a[i] >= 100 && p->a[i] <= 200){
            p->interval = p->interval + 1;
        }
    }
    pthread_exit(p);
}

int main()
{
    srand(time(0));
    int size;
    printf("Size of array: ");
    scanf("%d", &size);
    int* array = (int*)malloc(size * sizeof(int));
    int n;
    printf("Number of threads: ");
    scanf("%d", &n);
    double time_spent = 0.0;
    clock_t begin = clock();
    for(int i = 0; i < size; i++){
        array[i] = rand() % 500 - 250;
    }

    pthread_t threads[n];
    struct Params p[n];
    for (int i = 0; i < n; i++){
        int st = floor(size/n)*i;
        p[i].a = &array[st];
        p[i].s = (int)floor(size/n);
        p[i].even = 0;
        p[i].odd = 0;
        p[i].null_a = 0;
        p[i].pos = 0;
        p[i].neg = 0;
        p[i].interval = 0;
        if(i == n-1){
            p[i].s = (int)floor(size/n) + (size % n);
        }
        //printf("%d ", st);
        pthread_create(&threads[i], NULL, attr, &p[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], &p);
    }
    int even = 0, odd = 0, null_a = 0, pos = 0, neg = 0, interval = 0;
    for (int i = 0; i < n; i++) {
        even = even + p[i].even;
        odd = odd + p[i].odd;
        null_a = null_a + p[i].null_a;
        pos = pos + p[i].pos;
        neg = neg + p[i].neg;
        interval = interval + p[i].interval;
    }

    printf("Even: %d, odd: %d\n", even, odd);
    printf("Null: %d \n", null_a);
    printf("Positive: %d, negative: %d \n", pos, neg);
    printf("100-200 interval: %d \n", interval);

    clock_t end = clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time: %f\n", time_spent);

    free(array);
    return 0;
}
