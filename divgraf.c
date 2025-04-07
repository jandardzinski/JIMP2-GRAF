#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "graf.h"

/*
void partition(node_t *graph, int number_of_nodes, int *k_value)
{
	printf("Podaj na ile czesci chcesz podzielic graf:\n");
	scanf("%d", k_value);
	int k = *k_value;

	for(int i = 0; i < number_of_nodes; i++)
	{
		graph[i]->part = i % k + 1;
		printf("Partycja wierzcholka %d: %d\n", i, graph[i]->part);
	}
	
	int deleted_edges = 0;
	for(int i = 0; i < number_of_edges; i++)
	{
		for(int j = 0; j < number_of_nodes; j++)
		{
			if(graph[i][j] == 1 && graph[i]->part != graph[j]->part)
			deleted_edges++;
		}
	}
	printf("Liczba krawedzi do usuniecia wynosi: %d\n", deleted_edges);
}
*/
/*
void calculate_weights(node_t * graph, int number_of_nodes, int max_number, int **matrix)
{
	int weight_counter = 0;
	for(int i = 0; i < number_of_nodes; i++)
	{
		graph[i]->weight = 0;
		for(int j = 0; j < number_of_nodes; j++)
		{
			if(matrix[i][j] == 1)
			{
				graph[i]->weight++; 
			}	
		}
		if(graph[i]->weight >= 5)
		{	
			weight_counter++;
		}
		printf("Waga wierzcholka %d: %d\n", i, graph[i]->weight);

	}
	printf("%d\n", number_of_nodes);
	printf("Liczba >= 4 %d\n", weight_counter);
}*/

void bfs_partition(int **matrix, int n, int k, int* partition)
{
	/*int k;
	printf("Podaj na ile czesci chcesz podzielic graf:\n");
	scanf("%d", &k);
	printf("Czesci: %d", k);
	*/

	bool *visited = calloc(n, sizeof(bool));
	int max_size = (n + k -1) / k;
	int current_partition = 0;
	int partition_counter[k];
	int max_allowed = max_size * 1.15; 

	for(int i = 0; i < k; i++)
	{
		partition_counter[i] = 0;
	}

	for(int i = 0; i < n; i++)
	{
		if(visited[i])
		continue;
		
		int queue[n];
		int start = 0;
		int next = 0;

		queue[next++] = i;
		visited[i] = true;

		while(start < next)
		{
			int current_node; 
			current_node = queue[start++];
			// Znajdź najmniejszą możliwą partycję, która jeszcze ma miejsce
			
			int min_index = -1;
			int min_count = max_size + 1;
			for(int p = 0; p < k; p++)
			/*

			partition[current_node] = current_partition;
			partition_counter[current_partition]++;
			
			if(partition_counter[current_partition] >= max_size)
			{
				if(partition_counter[p] < max_size && partition_counter[p] < min_count)
				{
					min_index = p;
					min_count = partition_counter[p];
				}
			}
			
			// Jeśli wszystkie pełne, to daj do najmniejszej
			if(min_index == -1)
			{
				min_index = 0;
				for(int p = 1; p < k; p++)
				{
					if(partition_counter[p] < partition_counter[min_index])
					{
						min_index = p;
					}
				}
			}

			partition[current_node] = min_index;
			partition_counter[min_index]++;
			
			for(int j = 0; j < n; j++)
			{
				if(matrix[current_node][j] && !visited[j])
				{
					queue[next++] = j;
					visited[j] = true;
				}
			}
		}
	}
	free(visited);
	/*
	for (int i = 0; i < n; i++) 
	{
        	printf( "Wierzcholek %d -> Partycja %d\n", i, partition[i]);
    	}
	*/	
	int deleted_counter = 0;	
	//printf("Usuniete krawedzie miedzy partycjami:\n");
	for( int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			if(partition[i] != partition[j] && matrix[i][j] == 1)
			{
				//printf("Usuwam krawedz: %d - %d\n", i, j);
				//deleted_counter++;
				matrix[i][j] = 0;
				matrix[j][i] = 0;

				bool isolated_i = true;
				bool isolated_j = true;

				for( int k = 0; k < n; k++)
				{
					if(matrix[i][k] == 1)
					{
						isolated_i = false;
					}
					if(matrix[j][k] == 1)
					{
						isolated_j = false;
					}

				}
				if(isolated_i || isolated_j)
				{
					matrix[i][j] = 1;
					matrix[j][i] = 1;

					if(isolated_i)
					{
						//int old_partition = partition[i];
						for(int k = 0; k < n; k++)
						{
							if(matrix[i][k] == 1 && partition[i] != partition[k])
							{
								int old_partition = partition[i];
								partition[i] = partition[k];

								bool connection = false;
								for(int x = 0; x < n; x++)
								{
									if(partition[x] == partition[i] && matrix[i][x] == 1)
									{
										connection = true;
										break;
									}
								}

								if(connection)
								{
									partition_counter[old_partition]--;
									partition_counter[partition[i]]++;
									break;
								}
								else
								{
									partition[i] = old_partition;
								}
								//int target_partition = partition[k];
								/*if(partition_counter[target_partition] < max_allowed)
								{
									partition[i] = target_partition;
									partition_counter[old_partition]--;
									partition_counter[target_partition]++;
									break;
								}*/
							}
						}
					}

					if(isolated_j)
					{
						//int old_partition = partition[j];
						for(int k = 0; k < n; k++)
						{
							if(matrix[j][k] == 1 && partition[j] != partition[k])
							{
								int old_partition = partition[j];
                                                                partition[j] = partition[k];
								bool connection = false;
								for(int x = 0; x < n; x++)
								{
									if(partition[x] == partition[j] && matrix[j][x] == 1)
									{
										connection = true;
										break;
									}
								}
								if(connection)
								{
								 	partition_counter[old_partition]--;
                                                        	 	partition_counter[partition[j]]++;
									break;
								}
								else
								{
									partition[j] = old_partition;
								}
								
							}
						}
					}
				}
					
				else
				{
					//printf("Usuwam krawedz: %d - %d\n", i, j);
                                	deleted_counter++;
				}

			}
		}
	}
	printf("Licznik usunietych krawedzi: %d\n", deleted_counter);
	printf("Ilosc wierzcholkow w kazdej partycji:\n");
	for(int i = 0; i < k; i++)
	{
		printf("Partycja %d, %dwierzcholkow\n", i, partition_counter[i]);
	}

}

void partition_list(int **matrix, int k, int n, int *partition)
{
	for(int i = 0; i < k; i++)
	{
		printf("Lista sasiedztwa dla partycji %d:\n", i);
		for(int j = 0; j < n; j++)
		{
			if(partition[j] == i)
			{
				for(int x = 0; x < n; x++)
				{
					if(matrix[j][x] == 1 && partition[x] == i)
					{
						printf("Krawedz %d - %d\n", j, x);
					}
				}
			}
		}
	}
}

void print_matrix(node_t * graph, int number_of_nodes, FILE *wynik)
{
	for(int i = 0; i < number_of_nodes; i++)
	{

	}
}
