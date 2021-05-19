#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int printPredecessors(int n, int pre[], int j)
{
    if (j < 0)
        return 0;

    printPredecessors(n, pre, pre[j]);
    printf("%d ", j);
}

int main()
{
    int n, m;
    printf("Enter n & m:");
    scanf("%d %d", &n, &m);

    int edges[2 * m][3];
    for (int i = 0; i < 2 * m; i += 2)
    {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        edges[i][0] = u;
        edges[i][1] = v;
        edges[i][2] = w;

        edges[i + 1][0] = v;
        edges[i + 1][1] = u;
        edges[i + 1][2] = w;
    }

    for (int i = 0; i < n; i++)
    {
        int dis[n];
        for (int j = 0; j < n; j++)
            dis[j] = INT_MAX;
        dis[i] = 0;

        int pre[n];
        for (int j = 0; j < n; j++)
            pre[j] = -1;

        for (int j = 0; j < n - 1; j++)
        {
            for (int k = 0; k < 2 * m; k++)
            {
                int u = edges[k][0], v = edges[k][1], w = edges[k][2];
                if (dis[v] > dis[u] + w && dis[u] != INT_MAX)
                {
                    dis[v] = dis[u] + w;
                    pre[v] = u;
                }
            }
        }

        printf("Routing Table of node-%d\n", i);

        for (int j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            else if (dis[j] == INT_MAX)
                printf("dest- %d\t distance- INF\t path-> NOT_AVL\n", j);
            else
            {
                printf("dest- %d\t distance- %d\t path-> ", j, dis[j]);
                printPredecessors(n, pre, j);
                printf("\n");
            }
        }
        // for(int k=0;k<n;k++)
        // printf("%d ",pre[k]);
        printf("\n");
    }
}