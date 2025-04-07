#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graf.h"
#define MAX_NODES 1000

// ===== STRUKTURY DO LISTY SĄSIEDZTWA =====
typedef struct AdjNode {
    int neighbor;
    struct AdjNode* next;
} AdjNode;

AdjNode** create_adjacency_list(int** matrix, int n) {
    AdjNode** list = malloc(n * sizeof(AdjNode*));
    for (int i = 0; i < n; i++) list[i] = NULL;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j]) {
                AdjNode* new_node = malloc(sizeof(AdjNode));
                new_node->neighbor = j;
                new_node->next = list[i];
                list[i] = new_node;
            }
    return list;
}

// ===== PODZIAŁ NA WOJEWÓDZTWA =====
void dziel_na_wojewodztwa(AdjNode** list, int n, int x, FILE* out) {
    int* assigned = calloc(n, sizeof(int));
    int* sizes = calloc(x, sizeof(int));
    int cel = n / x;
    bool* visited = calloc(n, sizeof(bool));
    int region = 1;

    for (int i = 0; i < n && region <= x; i++) {
        if (assigned[i]) continue;

        int queue[1024], front = 0, rear = 0;
        queue[rear++] = i;
        visited[i] = true;

        while (front < rear && region <= x) {
            int v = queue[front++];

            if (!assigned[v] && sizes[region - 1] < cel) {
                assigned[v] = region;
                sizes[region - 1]++;
            }

            AdjNode* neighbor = list[v];
            while (neighbor) {
                if (!visited[neighbor->neighbor]) {
                    queue[rear++] = neighbor->neighbor;
                    visited[neighbor->neighbor] = true;
                }
                neighbor = neighbor->next;
            }

            if (sizes[region - 1] >= cel) region++;
        }
    }

    fprintf(out, "\nPodział na %d województw:\n", x);
    for (int w = 1; w <= x; w++) {
        fprintf(out, "Województwo %d: ", w);
        for (int i = 0; i < n; i++)
            if (assigned[i] == w) fprintf(out, "%d ", i);
        fprintf(out, "\n");
    }

    free(assigned);
    free(sizes);
    free(visited);
}

// ===== GŁÓWNA FUNKCJA ODCZYTU I PODZIAŁU =====
void przetwarzaj_plik(const char* nazwa) {
    FILE* in = fopen(nazwa, "r");
    FILE* out = fopen("wynik.txt", "w");
    if (!in) {
        printf("Błąd wczytywania %s\n", nazwa);
        return;
    }

    int max_nodes;
    fscanf(in, "%d", &max_nodes);

    int nodes[MAX_NODES], node_count = 0;
    char c = '_';
    while (c != '\n' && fscanf(in, "%d%c", &nodes[node_count], &c) == 2)
        node_count++;

    int index_pointers[MAX_NODES], pointer_count = 0;
    c = '_';
    while (c != '\n' && fscanf(in, "%d%c", &index_pointers[pointer_count], &c) == 2)
        pointer_count++;

    int** matrix = malloc(node_count * sizeof(int*));
    for (int i = 0; i < node_count; i++)
        matrix[i] = calloc(node_count, sizeof(int));

    int edge_groups[MAX_NODES][MAX_NODES];
    int group_sizes[MAX_NODES];
    int edge_ptrs[MAX_NODES];
    int edge_total = 0, group_index = 0;

    while (!feof(in)) {
        int line[MAX_NODES], count = 0;
        c = '_';
        while (fscanf(in, "%d%c", &line[count], &c) == 2 && c != '\n')
            count++;

        if (count == 0) break;

        for (int i = 0; i < count - 1; i++) {
            int a = line[0], b = line[i + 1];
            matrix[a][b] = 1;
            matrix[b][a] = 1;
            fprintf(out, "Krawędź: %d - %d\n", a, b);
        }
        group_sizes[group_index] = count;
        for (int i = 0; i < count; i++)
            edge_groups[group_index][i] = line[i];
        group_index++;
        edge_total += count;
    }

    // Lista sąsiedztwa
    AdjNode** adj = create_adjacency_list(matrix, node_count);

    // Podział na województwa
    dziel_na_wojewodztwa(adj, node_count, 3, out);  // <- tu ustaw liczbę województw

    // Czyszczenie
    for (int i = 0; i < node_count; i++) {
        AdjNode* p = adj[i];
        while (p) {
            AdjNode* next = p->next;
            free(p);
            p = next;
        }
        free(matrix[i]);
    }
    free(matrix);
    free(adj);
    fclose(in);
    fclose(out);
}

// ===== MAIN =====
int main() {
    przetwarzaj_plik("graf.csrrg");
    return 0;
}
