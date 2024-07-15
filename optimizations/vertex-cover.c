#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

bool isVertexCover(int graph[MAX_VERTICES][MAX_VERTICES], int n, int cover[], int k) {
    bool edges_covered[MAX_VERTICES][MAX_VERTICES] = {false}; // A matrix to keep track of which edges are covered

    // For each vertex in the cover, mark all edges incident to it as covered
    for (int i = 0; i < k; i++) {
        int v = cover[i]; // The vertex we're considering
        for (int j = i+1; j < n; j++) {
            if (graph[v][j]) {
                edges_covered[v][j] = edges_covered[j][v] = true;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (graph[i][j] && !edges_covered[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void generateCombinations(int n, int k, int cover[], int start, int currentSize,
                          int graph[MAX_VERTICES][MAX_VERTICES], int* minCover, int* minSize) {
    if (currentSize == k) {
        if (isVertexCover(graph, n, cover, k)) {
            if (k < *minSize) {
                *minSize = k;
                for (int i = 0; i < k; i++) {
                    minCover[i] = cover[i];
                }
            }
        }
        return;
    }

    for (int i = start; i < n; i++) {
        cover[currentSize] = i;
        generateCombinations(n, k, cover, i + 1, currentSize + 1, graph, minCover, minSize);
        if (*minSize <= k) return; // Early termination if we've found a solution of size k or smaller
    }
}

void findMinVertexCover(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    int* minCover = (int*)malloc(n * sizeof(int)); // The maximum vertex cover can't be larger than n/2
    // cover is an array that stores the current combination of vertices we're considering as a vertex cover
    int* cover = (int*)malloc(n * sizeof(int));
    int minSize = n + 1; // n/2 +1 

    for (int k = 1; k <= n && minSize > k; k++) {
        generateCombinations(n, k, cover, 0, 0, graph, minCover, &minSize);
    }

    printf("Minimum Vertex Cover: ");
    for (int i = 0; i < minSize; i++) {
        printf("%d ", minCover[i]);
    }
    printf("\nSize: %d\n", minSize);

    free(minCover);
    free(cover);
}