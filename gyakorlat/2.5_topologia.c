#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int n = 50;
    FILE *f;
    f = fopen("gr.txt", "w");
    for(int i = 1; i <= n; i++){
        int node = i * i;
        int edge = 2*(i-1)*i;
        float ratio = (float)edge/(float)node;
        printf("%d x %d meretu: %d csomopont, %d el, aranyuk(el/csp): %f\n", i, i, node, edge, ratio);
        fprintf(f, "%d %d %f\n", node, edge, ratio);
    }
    fclose(f);

    f = fopen("hiszt.txt", "w");
    for(int x = 1; x <= 12; x++){
        for(int y = 1; y <= 12; y++){
            float dist = 0;
            for(int i = 1; i <= 12; i++){
                for(int j = 1; j <= 12; j++){
                    dist += abs(x-i) + abs(y-j);
                }
            }
            dist = dist / (12*12);
            printf("%f\n", dist);
            fprintf(f, "%f\n", dist);
        }
    }
    fclose(f);

    int n2 = 24;
    for(int i = 1; i <= n2; i++){
        int node = i * i * i;
        int edge = 2*(i-1)*(i*i) + (i-1)*(i*i);
        int node2d = i * i;
        int edge2d = 2*(i-1)*i;
        float ratio = (float)edge/(float)edge2d;
        printf("%d x %d meretu: %d csomopont, %d el, elek aranya a sikbelivel: %f\n", i, i, node, edge, ratio);
    }

    for(int x = 1; x <= 5; x++){
        for(int y = 1; y <= 5; y++){
            for(int z = 1; z <= 5; z++){
                float dist = 0;
                for(int i = 1; i <= 15; i++){
                    for(int j = 1; j <= 5; j++){
                        for(int k = 1; k <= 5; k++){
                            dist += abs(x-i) + abs(y-j) - abs(z-k);
                        }
                    }
                }
                dist = dist / (5*5*5);
                printf("%f\n", dist);
            }
        }
    }


    return 0;
}
