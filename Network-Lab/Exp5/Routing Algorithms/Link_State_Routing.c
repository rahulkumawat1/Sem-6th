#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

//Considering numbering of router as 0th-index

int n, m;

int minDistance(int dist[], int inSet[])
{

    int min = INT_MAX, min_index = -1;

    for (int v = 0; v < n; v++)
        if (inSet[v] == 0 &&
            dist[v] < min)
            min = dist[v], min_index = v;

    return min_index;
}

void printPath(int parent[], int j)
{

    if (parent[j] == -1)
        return;

    printPath(parent, parent[j]);

    printf("%d ", j);
}

int printSolution(int dist[], int parent[], int src)
{
    for (int i = 0; i < n; i++)
    {
        if (i != src)
        {
            if (dist[i] == INT_MAX)
            {
                printf("dest- %d\t distance- INF\t path- NOT_AVL\n", i);
                continue;
            }

            printf("dest- %d\t distance- %d\t path-  %d ", i, dist[i], src);
            printPath(parent, i);
            printf("\n");
        }
    }
}

void dijkstra(int graph[n][n], int src)
{

    int dist[n];
    int inSet[n];
    int parent[n];

    parent[src] = -1;
    for (int i = 0; i < n; i++)
    {
        dist[i] = INT_MAX;
        inSet[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++)
    {
        int u = minDistance(dist, inSet);
        if (u == -1)
            continue;

        inSet[u] = 1;

        for (int v = 0; v < n; v++)
            if (!inSet[v] && graph[u][v] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    printSolution(dist, parent, src);
}

int main()
{
    scanf("%d", &n);
    scanf("%d", &m);

    int src, dest, weigth;

    int graph[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = INT_MAX;

    for (int i = 0; i < m; i++)
    {
        scanf("%d %d %d", &src, &dest, &weigth);
        graph[src][dest] = weigth;
        graph[dest][src] = weigth;
    }

    for (int i = 0; i < n; i++)
    {
        printf("src %d =>\n", i);
        dijkstra(graph, i);
        printf("\n");
    }
    return 0;
}
