#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node
{
        int x;
        int y;
	int part;
        //int* edges;
        //int weight;
        //struct Node* next;
} *node_t, node_o;

node_t * create_graph(FILE *in, FILE *out, int *ptr_number_of_nodes, int max_number);
int **create_matrix(FILE *in, FILE *out, int number_of_nodes);
//void partition(node_t *graph, int number_of_nodes, int *k_value);
//void calculate_weights(node_t * graph, int number_of_nodes, int max_number, int **matrix);
void bfs_partition(int **matrix, int n, int k, int* partition);
void partition_list(int **matrix, int k, int n, int *partition, FILE* out);
void print_matrix(node_t * graph, FILE *out, int number_of_nodes, int max_number);


