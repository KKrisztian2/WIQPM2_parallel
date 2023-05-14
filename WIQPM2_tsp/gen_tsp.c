#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <windows.h>

struct route{
    int* r;
    int fitness;
};

void generate_distances(int** dist, int n) {
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
        for (int j = i+1; j < n; j++) {
            dist[i][j] = rand() % 1000 + 1;
            dist[j][i] = dist[i][j];
        }
    }
}

void generate_population(int** dist, int n, int pop_size, struct route population[]){
    for (int i = 0; i < pop_size; i++) {
        for (int j = 0; j < n; j++) {
            population[i].r[j] = j;
        }
        for (int j = 1; j < n; j++) {
            int k = rand() % (n-1) + 1;
            int temp = population[i].r[j];
            population[i].r[j] = population[i].r[k];
            population[i].r[k] = temp;
        }
    }
}

void evaluate_fitness(struct route population[], int** dist, int pop_size, int n){
    for (int i = 0; i < pop_size; i++) {
        population[i].fitness = 0;
        for (int j = 0; j < n - 1; j++) {
            population[i].fitness += dist[population[i].r[j]][population[i].r[j+1]];
        }
    }
}

void sort_population(struct route population[], int pop_size) {
    for (int i = 0; i < pop_size - 1; i++) {
        for (int j = i + 1; j < pop_size; j++) {
            if (population[j].fitness < population[i].fitness) {
                struct route temp = population[i];
                population[i] = population[j];
                population[j] = temp;
            }
        }
    }
}

void select_parents(struct route population[], int n, int pop_size, int* best_index1, int* best_index2){
    int t_size = pop_size/10 + 2;
    int parent_ind[t_size];
    for (int i = 0; i < t_size; i++) {
        parent_ind[i] = rand() % pop_size;
    }
    *best_index1 = parent_ind[0];
    *best_index2 = parent_ind[1];
    int best_fitness1 = population[parent_ind[0]].fitness;
    int best_fitness2 = population[parent_ind[1]].fitness;

    for (int i = 1; i < t_size; i++) {
        int index = parent_ind[i];
        int fitness = population[index].fitness;
        if (fitness < best_fitness1) {
            int temp1 = *best_index1;
            int temp2 = best_fitness1;
            *best_index1 = index;
            best_fitness1 = fitness;
            *best_index2 = temp1;
            best_fitness2 = temp2;
        }
        else if(fitness < best_fitness2){
            *best_index2 = index;
            best_fitness2 = fitness;
        }
    }
    free(parent_ind);
}

struct route crossover(struct route parent1, struct route parent2, int n, int** dist){
    struct route child;
    #pragma omp critical
    {
        child.r = (int *)malloc(n * sizeof(int));
    }
    int start = rand() % n;
    int end = rand() % n;
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    child.r[0] = 0;
    for (int i = 1; i < n; i++) {
        child.r[i] = -1;
    }
    for (int i = start; i <= end; i++) {
        child.r[i] = parent1.r[i];
    }

    int j = 0, b = 0;
    for(int i = 0; i < n; i++){
        while(child.r[i] == -1){
            for(int k = 0; k < n; k++){
                if(child.r[k] == parent2.r[j]){
                    b = 1;
                    break;
                }
            }
            if(b == 1){
                j++;
                b = 0;
            }
            else{
                child.r[i] = parent2.r[j];
                b = 0;
            }
        }
    }

    child.fitness = 0;
    for (int i = 0; i < n - 1; i++) {
        child.fitness += dist[child.r[i]][child.r[i+1]];
    }

    return(child);
}

struct route mutate(struct route rt, int n, int** dist){
    struct route ch;
    #pragma omp critical
    {
        ch.r = (int *)malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        ch.r[i] = rt.r[i];
    }
    ch.fitness = rt.fitness;

    double mutation_rate = 0.1;

    for (int i = 1; i < n; i++) {
        if (((double)rand()/(double)RAND_MAX) < mutation_rate) {
            int j = rand() % (n-1) + 1;
            int temp = ch.r[i];
            ch.r[i] = ch.r[j];
            ch.r[j] = temp;
        }
    }

    ch.fitness = 0;
    for (int i = 0; i < n - 1; i++) {
        ch.fitness += dist[ch.r[i]][ch.r[i+1]];
    }

    return ch;
}

void print_population(struct route population[], int n, int pop_size) {
    int i, j;
    for (i = 0; i < pop_size; i++) {
        printf("%d: [ ", i);
        for (j = 0; j < n; j++) {
            printf("%d ", population[i].r[j]);
        }
        printf("] fitness = %d\n", population[i].fitness);
    }
}

int main()
{
    srand(time(0));

    omp_set_num_threads(4);

    int n;
    printf("Number of cities: ");
    scanf("%d", &n);
    int pop_size = 5*n;

    double start;
    double end;
    start = omp_get_wtime();

    int** dist = (int **)malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++){
        dist[i] = (int *)malloc(n * sizeof(int));
    }
    struct route population[pop_size];
    for(int i = 0; i < pop_size; i++){
        population[i].r = (int *)malloc(n * sizeof(int));
    }

    generate_distances(dist, n);
    /*for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/


    /*n = 400;
    FILE* f;
    f = fopen("tsp.txt", "r");
    for(size_t i = 0; i < n; ++i)
    {
        for(size_t j = 0; j < n; ++j)
            fscanf(f, "%d", dist[i] + j);
    }*/

    /*for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    generate_population(dist, n, pop_size, population);
    sort_population(population, pop_size);
    evaluate_fitness(population, dist, pop_size, n);
    for(int i = 0; i < 100; i++){
        #pragma omp parallel
        {
        #pragma omp for
        for(int j = 0; j < pop_size/2; j++){
            int best_index1, best_index2;
            struct route parent1, parent2;
            select_parents(population, n, pop_size, &best_index1, &best_index2);
            #pragma omp critical
            {
                parent1.r = (int *)malloc(n * sizeof(int));
                parent2.r = (int *)malloc(n * sizeof(int));
            }
            for(int k = 0; k < n ; k++){
                parent1.r[k] = population[best_index1].r[k];
                parent2.r[k] = population[best_index2].r[k];
            }
            parent1.fitness = population[best_index1].fitness;
            parent2.fitness = population[best_index2].fitness;
            struct route child1 = crossover(parent1, parent2, n, dist);
            struct route child2 = crossover(parent2, parent1, n, dist);
            free(parent1.r);
            free(parent2.r);
            struct route m_child1 = mutate(child1, n, dist);
            struct route m_child2 = mutate(child1, n, dist);
            int in_pop = 0, count = 0;
            if(m_child1.fitness < population[pop_size-1].fitness){
                for(int k = 0; k < pop_size ; k++){
                    if(population[k].fitness == m_child1.fitness){
                        for(int l = 0; l < n; l++){
                            if(population[k].r[l] == m_child1.r[l]){
                                count++;
                            }
                        }
                        if(count == n){
                            in_pop = 1;
                            count = 0;
                            break;
                        }
                        else if(population[k].fitness > m_child1.fitness){
                            break;
                        }
                        else{
                            count = 0;
                        }
                    }
                }
                if(in_pop == 0){
                    #pragma omp critical
                    {
                        for(int k = 0; k < n ; k++){
                            population[pop_size-1].r[k] = m_child1.r[k];
                        }
                        population[pop_size-1].fitness = m_child1.fitness;
                        sort_population(population, pop_size);
                    }
                }
            }
            in_pop = 0;
            if(m_child2.fitness < population[pop_size-1].fitness){
                for(int k = 0; k < pop_size ; k++){
                    if(population[k].fitness == m_child2.fitness){
                        for(int l = 0; l < n; l++){
                            if(population[k].r[l] == m_child2.r[l]){
                                count++;
                            }
                        }
                        if(count == n){
                            in_pop = 1;
                            count = 0;
                            break;
                        }
                        else if(population[k].fitness > m_child2.fitness){
                            break;
                        }
                        else{
                            count = 0;
                        }
                    }
                }
                if(in_pop == 0){
                    #pragma omp critical
                    {
                        for(int k = 0; k < n ; k++){
                            population[pop_size-1].r[k] = m_child2.r[k];
                        }
                        population[pop_size-1].fitness = m_child2.fitness;
                        sort_population(population, pop_size);
                    }
                }
            }

            free(child1.r);
            free(m_child1.r);
            free(child2.r);
            free(m_child2.r);
        }
    }
    }

    sort_population(population, pop_size);
    printf("\nBest solution: ");
    for(int i = 0; i < n; i++){
       printf("%d ", population[0].r[i]);
    }
    printf("fitness = %d\n", population[0].fitness);


    for (int i = 0; i < n; i++){
        free(dist[i]);
    }
    for (int i = 0; i < pop_size; i++){
        free(population[i].r);
    }
    free(dist);

    end = clock();
    end = omp_get_wtime();
    printf("\nWork time: %f seconds\n", end - start);

    return 0;
}
