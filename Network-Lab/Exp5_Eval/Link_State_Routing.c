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

int dijkstra(int graph[n][n], int src)
{

    int dist[n];
    int inSet[n];

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
                dist[v] = dist[u] + graph[u][v];
            }
    }

    if (dist[n - 1] == INT_MAX)
        return -1;

    return dist[n - 1];
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

    int rp;
    scanf("%d", &rp);
    for (int i = 0; i < n; i++)
    {
        graph[i][rp] = INT_MAX;
        graph[rp][i] = INT_MAX;
    }

    int min_dist = dijkstra(graph, 0);
    printf("%d\n", min_dist);
    return 0;
}
