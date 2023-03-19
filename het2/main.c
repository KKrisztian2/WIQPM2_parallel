#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void egyedi(int [], const int);
int main()
{
    srand(time(0));
    int n=500;
    int t[n];
    int e;
    int i=0;
    while(i!=n-1){
        e=1;
        t[i]=rand()%1000+1;
        for(int j=0;j<i;j++){
            if(t[i]==t[j]){
                e=0;
                break;
            }
        }
        if(e==1) i++;
    }

    for(int i=0;i<3;i++){
        int ran=rand()%2;
        if(ran==1){
            t[2*i]=t[2*i+1];
        }
    }
    egyedi(t, n);

    return 0;
}
void egyedi(int t[], const int n){
    int e=1;
    for(int i=0;i<n;i++){
        for(int j=(i+1);j<n;j++){
           if(t[i]==t[j]){
                e=0;
                break;
           }
        }
        if(e==0){
            printf("Nem egyedi");
            break;
        }
    }
    if(e==1) printf("Egyedi"); //O(n^2)
}
