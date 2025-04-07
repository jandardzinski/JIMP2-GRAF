#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "graf.h"

int main(int argc, char ** argv)
{
	FILE *in = fopen("graf.csrrg", "r");
	int max_number;
	fscanf(in, "%d\n", &max_number); 
	FILE *out = fopen("wynik.txt", "w");
	FILE *wynik = fopen("podzial.txt", "w");
	int number_of_nodes = 0;
	node_t *graph = create_graph(in, out, &number_of_nodes, max_number);
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
	print_matrix(graph, wynik, number_of_nodes, max_number);
	partition_list(matrix, k, number_of_nodes, partition, wynik);
	free(partition);
	fclose(wynik);

	return 0; 
}
