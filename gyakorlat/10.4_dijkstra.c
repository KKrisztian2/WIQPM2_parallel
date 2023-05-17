#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define V 5

int minDistance(int dist[], int visited[])
{
    int min = INT_MAX, min_index;

    #pragma omp parallel for
    for (int v = 0; v < V; v++)
    {
        if (visited[v] == 0 && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void dijkstra(int graph[V][V], int source)
{
    int dist[V];
    int visited[V];

    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[source] = 0;

    #pragma omp parallel
    {
        for (int count = 0; count < V - 1; count++)
        {
            int u = minDistance(dist, visited);
            visited[u] = 1;

            for (int v = 0; v < V; v++)
            {
                if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                {
                        dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    printf("Vertex\tDistance from Source\n");
    for (int i = 0; i < V; i++)
    {
        printf("%d\t%d\n", i, dist[i]);
    }
}

int main()
{
    omp_set_num_threads(4);
    int graph[V][V] = {
        {0, 4, 0, 0, 0},
        {4, 0, 8, 0, 0},
        {0, 8, 0, 7, 0},
        {0, 0, 7, 0, 9},
        {0, 0, 0, 9, 0}
    };

    dijkstra(graph, 0);

    return 0;
}
