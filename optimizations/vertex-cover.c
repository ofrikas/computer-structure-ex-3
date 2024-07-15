#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

bool isVertexCover(int graph[MAX_VERTICES * MAX_VERTICES], int n, int cover[], int k) {
    bool edges_covered[MAX_VERTICES * MAX_VERTICES] = {false};

    int* endCover = &cover[k];
    while (cover < endCover)
    {
        int v = *cover++;
        for (int j = 0; j < n; j++) {
            if (graph[v * MAX_VERTICES + j]) {
                edges_covered[v * MAX_VERTICES+ j] = true;
                edges_covered[j * MAX_VERTICES + v] = true;
            }
        }
    }

    // here we did optimization by 
    int startrow = 0;
    int i = 0;
    n--;
    while (i<n)
    {
        i++;
        int* endgraph = &graph[startrow + n];
        int* graphLoc = &graph[startrow + i];
        bool* edgesLoc = &edges_covered[startrow + i];
        while (graphLoc <= endgraph)
        {
            if (*graphLoc && !*edgesLoc) {
                return false;
            }
            graphLoc++;
            edgesLoc++;
        }
        startrow += MAX_VERTICES;
    }
    return true;
}

void generateCombinations(int n, int k, int cover[], int start, int currentSize,
                          int graph[MAX_VERTICES * MAX_VERTICES], int* minCover, int* minSize) {
    if (currentSize == k) {
        if (isVertexCover(graph, n, cover, k)&& k < *minSize) {
                *minSize = k;
                int* minCoverEnd = &minCover[k];
                while (minCover < minCoverEnd)
                {
                    *minCover = *cover;
                    minCover++;
                    cover++;
                }
        }
        return;
    }

    int i = start;
    while (i < n)
    {
        cover[currentSize] = i;
        i++;
        generateCombinations(n, k, cover, i , currentSize + 1, graph, minCover, minSize);
        if (*minSize <= k) return; // Early termination if we've found a solution of size k or smaller
    }
}

void findMinVertexCover(int graph[MAX_VERTICES * MAX_VERTICES], int n) {
    int minCover[MAX_VERTICES];
    int cover[MAX_VERTICES];
    int minSize = n + 1;

    for (int k = 1; k <= n && minSize > k; k++) {
        generateCombinations(n, k, cover, 0, 0, graph, minCover, &minSize);
    }

    printf("Minimum Vertex Cover: ");
    for (int i = 0; i < minSize; i++) {
        printf("%d ", minCover[i]);
    }
    printf("\nSize: %d\n", minSize);

}