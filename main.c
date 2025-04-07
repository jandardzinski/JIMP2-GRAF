#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "graf.h"

int main(int argc, char ** argv)
{
	FILE *in = fopen("graf.csrrg", "r");
	FILE *out = fopen("wynik.txt", "w");
	int number_of_nodes = 0;
	node_t *graph = create_graph(in, out, &number_of_nodes);
	int **matrix = create_matrix(in, out, number_of_nodes);

	//printf("Liczba wierzcholkow grafu: %d\n", number_of_nodes);

	int *partition = malloc(number_of_nodes * sizeof(int));
        for (int i = 0; i < number_of_nodes; i++)
        {
                partition[i] = -1;
        }
	int k;
        printf("Podaj na ile czesci chcesz podzielic graf:\n");
        scanf("%d", &k);
	bfs_partition(matrix, number_of_nodes, k, partition);
	partition_list(matrix, k, number_of_nodes, partition);
	free(partition);

	return 0;
}
