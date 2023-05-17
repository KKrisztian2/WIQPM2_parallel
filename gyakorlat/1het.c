#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

void nyolckar(int szam);
void randomgen();
void argint(int argc, int a, int b);
void osszeg();
void prim(int);
void filek(int *t, int n);

int main(int argc, char* argv[])
{
    srand(time(0));
    nyolckar(737694);

    sleep(1);

    printf("\n");
    randomgen();

    printf("\n");
    argint(argc , argv[1], argv[2]);

    printf("\n");
    osszeg();

    printf("\n");
    for(int i=0;i<20;i++){
        double time_spent = 0.0;
        clock_t begin = clock();
        prim((i+1)*1000);
        clock_t end = clock();
        time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
        printf("n=%d: %f\n", (i+1)*1000, time_spent);
    }

    printf("\n");
    int n = 5;
    int t[n];
    for(int i = 0; i < n; i++){
        t[i] = rand()%100 + 1;
    }
    filek(t, n);

    return 0;
}

void nyolckar(int szam){
    printf("%d \n", szam);
    for(int i = 7; i >= 0; i--){
        if(szam/pow(10, i) != 0){
            int j = (int)floor(szam/pow(10, i));
            printf("%d", j%10);
        }
        else{
            printf("0");
        }
    }
}

void randomgen(){
    int isz = rand()%500 + 500;
    printf("%d \n", isz);
    float fsz = ((float)rand()/(float)(RAND_MAX)) * 500 + 500;
    printf("%f \n", fsz);
    double dsz = ((double)rand()/(double)(RAND_MAX)) * 500 + 500;
    printf("%f \n", dsz);
}

void argint(int argc ,int a, int b){
    if(argc != 3){
        printf("Nem megfelelo argumentum szam \n");
    }else{
        int sz = rand()%(b-a) + a;
        printf("%d \n", sz);
    }
}

void osszeg(){
    int a,b;
    a=rand()%100+1;
    b=rand()%100+1;
    printf("%d + %d = ?\n", a, b);
    int helyes=0;
    int valasz=0;
    double time_spent = 0.0;
    clock_t begin = clock();
    while(helyes!=1){
        scanf("%d", &valasz);
        if(valasz==(a+b)){
            helyes=1;
        }else{
            printf("Helytelen\n");
        }
    }
    clock_t end = clock();
    time_spent+=(double)(end - begin) / CLOCKS_PER_SEC;
    printf("Szamolasi ido: %f", time_spent);
}

void prim(int n){
    int db=0;
    int p=0;
    if(n==2) p=1;
    if(n==3) p=2;
    else if(n>3){
        for(int i=3;i<(n+1);i++){
            for(int j=2;j<(i/2);j++){
                if(i%j==0){
                    db++;
                    break;
                }
            }
            if(db==0) p++;
            db=0;
        }
    }
}

void filek(int *t, int n){
    FILE *f;
    f = fopen("tomb.txt", "w");
    for(int i = 0; i < n; i++){
        fprintf(f, "%d ", t[i]);
    }
    float meret = ftell(f);
    printf("Meret: %f \n", meret);
    fclose(f);
    f = fopen("tomb.txt", "r");

    printf("\n");
    for(int i = 0; i < n; i++){
        fscanf(f, "%d", &t[i]);
        printf("%d ", t[i]);
    }
    fclose(f);
}
