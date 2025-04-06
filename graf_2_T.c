#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ==================== STRUKTURY =====================
typedef struct Node {
    int x;
    int y;
} *node_t, node_o;

typedef struct AdjNode {
    int neighbor;
    struct AdjNode* next;
} AdjNode;

// ==================== LISTA SĄSIEDZTWA =====================
AdjNode** build_adjacency_list(int** matrix, int n) {
    AdjNode** list = malloc(n * sizeof(AdjNode*));
    for (int i = 0; i < n; i++) list[i] = NULL;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j]) {
                AdjNode* new_node = malloc(sizeof(AdjNode));
                new_node->neighbor = j;
                new_node->next = list[i];
                list[i] = new_node;
            }
        }
    }
    return list;
}

// ==================== PODZIAŁ NA WOJEWÓDZTWA =====================
void podziel_na_wojewodztwa(AdjNode** list, int liczba_miast, int liczba_woj, FILE* out) {
    int* przypisanie = calloc(liczba_miast, sizeof(int));
    int* rozmiary = calloc(liczba_woj, sizeof(int));
    int cel_rozmiar = liczba_miast / liczba_woj;
    bool* odwiedzony = calloc(liczba_miast, sizeof(bool));
    int woj_id = 1;

    for (int start = 0; start < liczba_miast; start++) {
        if (przypisanie[start] != 0) continue;

        int kolejka[1024], front = 0, tyl = 0;
        kolejka[tyl++] = start;
        odwiedzony[start] = true;

        while (front < tyl && woj_id <= liczba_woj) {
            int v = kolejka[front++];
            if (przypisanie[v] == 0 && rozmiary[woj_id - 1] < cel_rozmiar) {
                przypisanie[v] = woj_id;
                rozmiary[woj_id - 1]++;
            }

            AdjNode* sasiad = list[v];
            while (sasiad) {
                int u = sasiad->neighbor;
                if (!odwiedzony[u]) {
                    kolejka[tyl++] = u;
                    odwiedzony[u] = true;
                }
                sasiad = sasiad->next;
            }

            if (rozmiary[woj_id - 1] >= cel_rozmiar) woj_id++;
        }
    }

    fprintf(out, "\nPodział na województwa:\n");
    for (int w = 1; w <= liczba_woj; w++) {
        fprintf(out, "Województwo %d: ", w);
        for (int i = 0; i < liczba_miast; i++) {
            if (przypisanie[i] == w) {
                fprintf(out, "%d ", i);
            }
        }
        fprintf(out, "\n");
    }

    free(przypisanie);
    free(rozmiary);
    free(odwiedzony);
}

// ==================== GŁÓWNA FUNKCJA =====================
void create_graph() {
    FILE *in = fopen("graf.csrrg", "r");
    FILE *out = fopen("wynik.txt", "w");
    if (in == NULL) {
        printf("Błąd podczas wczytywania pliku.\n");
        return;
    }

    int max_number;
    fscanf(in, "%d", &max_number);
    int number_of_nodes = 0;
    char c = '_';
    int buf[8192];
    while (c != '\n' && fscanf(in, "%d%c", &buf[number_of_nodes], &c) == 2) {
        number_of_nodes++;
    }

    node_t *new_graph = malloc(number_of_nodes * sizeof(node_t));
    int first, second;
    fscanf(in, "%d%c%d%c", &first, &c, &second, &c);

    int **matrix = malloc(number_of_nodes * sizeof(int*));
    for (int i = 0; i < number_of_nodes; i++) {
        matrix[i] = calloc(number_of_nodes, sizeof(int));
    }

    for (int i = first + 1; i < second; i++) {
        int start = buf[first];
        int end = buf[i];
        matrix[start][end] = 1;
        matrix[end][start] = 1;
        fprintf(out, "Krawędź: %d - %d\n", start, end);
    }

    first = second;
    while (fscanf(in, "%d%c", &second, &c) != EOF) {
        int start = buf[first];
        for (int i = first + 1; i < second; i++) {
            int end = buf[i];
            matrix[start][end] = 1;
            matrix[end][start] = 1;
            fprintf(out, "Krawędź: %d - %d\n", start, end);
        }
        first = second;
    }

    int start = buf[first];
    for (int i = first + 1; i < number_of_nodes; i++) {
        int end = buf[i];
        matrix[start][end] = 1;
        matrix[end][start] = 1;
        fprintf(out, "Krawędź: %d - %d\n", start, end);
    }

    // ======================= LISTA I PODZIAŁ =======================
    AdjNode** lista = build_adjacency_list(matrix, number_of_nodes);
    int liczba_wojewodztw = 3; // Możesz zmienić na dowolną wartość
    podziel_na_wojewodztwa(lista, number_of_nodes, liczba_wojewodztw, out);

    // ======================= CZYSZCZENIE ===========================
    for (int i = 0; i < number_of_nodes; i++) {
        AdjNode* curr = lista[i];
        while (curr) {
            AdjNode* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
        free(matrix[i]);
    }
    free(lista);
    free(matrix);
    free(new_graph);
    fclose(in);
    fclose(out);
}

// ==================== MAIN =====================
int main() {
    create_graph();
    return 0;
}
