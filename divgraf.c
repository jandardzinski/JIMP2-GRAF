#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "graf.h"

void assign_partitions(int **matrix, int n, int k, int* partition)
{
    bool *visited = calloc(n, sizeof(bool));
    int partition_counter[k];
    int max_size = (n + k - 1) / k;
    int max_allowed = max_size * 1.1;

    for (int i = 0; i < k; i++) partition_counter[i] = 0;

    for (int i = 0; i < n; i++)
    {
        if (visited[i])
            continue;

        int queue[n];
        int start = 0, next = 0;
        queue[next++] = i;
        visited[i] = true;

        while (start < next)
        {
            int current_node = queue[start++];

            int min_index = -1;
            for (int j = 0; j < n; j++)
            {
                if (matrix[current_node][j] == 1 && visited[j])
                {
                    if (partition_counter[partition[j]] < max_allowed)
                    {
                        min_index = partition[j];
                        break;
                    }
                }
            }

            if (min_index == -1)
            {
                min_index = 0;
                for (int p = 1; p < k; p++)
                {
                    if (partition_counter[p] < partition_counter[min_index])
                        min_index = p;
                }
            }

            partition[current_node] = min_index;
            partition_counter[min_index]++;

            for (int j = 0; j < n; j++)
            {
                if (matrix[current_node][j] == 1 && !visited[j])
                {
                    queue[next++] = j;
                    visited[j] = true;
                }
            }
        }
    }
    free(visited);
    // Sprawdzamy, czy są puste partycje i przypisujemy do nich wierzchołki
        /*
        for (int i = 0; i < n; i++)
        {
                printf( "Wierzcholek %d -> Partycja %d\n", i, partition[i]);
        }
        */

}


void fix_edges(int **matrix, int n, int* partition, int *partition_counter, int lonely, bool *moved)
{
    bool found = false;
    for (int k = 0; k < n; k++)
    {
        if (matrix[lonely][k] == 1 && partition[k] != partition[lonely])
        {
            if (partition_counter[partition[k]] > 1)
            {
                int old_partition_neighbor = partition[k];
                partition[k] = partition[lonely];

                partition_counter[partition[lonely]]++;
                partition_counter[old_partition_neighbor]--;

                moved[k] = true;
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        for (int k = 0; k < n; k++)
        {
            if (matrix[lonely][k] == 1 && partition[lonely] != partition[k])
            {
                int old_partition = partition[lonely];
                partition[lonely] = partition[k];

                bool connection = false;
                for (int x = 0; x < n; x++)
                {
                    if (partition[x] == partition[lonely] && matrix[lonely][x] == 1)
                    {
                        connection = true;
                        break;
                    }
                }

                if (connection)
                {
                    partition_counter[old_partition]--;
                    partition_counter[partition[lonely]]++;
                }
                else
                {
                    partition[lonely] = old_partition;
                }
            }
        }
    }
}

void remove_edges(int **matrix, int n, int* partition, int k)
{
    int partition_counter[k];
    for (int i = 0; i < k; i++) 
	partition_counter[i] = 0;
    for (int i = 0; i < n; i++) 
	partition_counter[partition[i]]++;

    bool moved[n];
    for (int i = 0; i < n; i++) moved[i] = false;

    int deleted_counter = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (partition[i] != partition[j] && matrix[i][j] == 1)
            {
                matrix[i][j] = 0;
                matrix[j][i] = 0;

                bool isolated_i = true, isolated_j = true;

                for (int k = 0; k < n; k++)
                {
                    if (matrix[i][k] == 1) 
			isolated_i = false;
                    if (matrix[j][k] == 1) 
			isolated_j = false;
                }

                if (isolated_i || isolated_j)
                {
                    matrix[i][j] = 1;
                    matrix[j][i] = 1;

                    if (isolated_i)
                        fix_edges(matrix, n, partition, partition_counter, i, moved);

                    if (isolated_j)
                        fix_edges(matrix, n, partition, partition_counter, j, moved);
                }
                else
                {
                    deleted_counter++;
                }
            }
        }
    }

    printf("Licznik usunietych krawedzi: %d\n", deleted_counter);
    printf("Ilosc wierzcholkow  w kazdej partycji:\n");
	for(int i = 0; i < k; i++)
	{
		printf("Partycja %d, %dwierzcholkow\n", i, partition_counter[i]);
	}
}

void partition_list(int **matrix, int k, int n, int *partition, FILE *out)
{
	for(int i = 0; i < k; i++)
	{
		fprintf(out, "Lista sasiedztwa dla partycji %d:\n", i);
		for(int j = 0; j < n; j++)
		{
			if(partition[j] == i)
			{
				for(int x = 0; x < n; x++)
				{
					if(matrix[j][x] == 1 && partition[x] == i)
					{
						fprintf(out," %d - %d\n", j, x);
					}
				}
			}
		}
	}
}

void print_matrix(node_t * graph, FILE *out, int number_of_nodes, int max_number)
{
	int **matrix = malloc(max_number * sizeof(int*));
		for(int i = 0; i < max_number; i++)
		{
			matrix[i] = calloc(max_number, sizeof(int));
		}
	for(int i = 0; i < number_of_nodes; i++)
	{
		matrix[graph[i]->x][graph[i]->y] = 1;
	}

	for(int i = 0; i < max_number; i++)
	{
		fprintf(out, "[ ");
		for(int j = 0; j < max_number; j++)
		{
			fprintf(out, "%d ", matrix[i][j]);
		}
		fprintf(out, "]\n");
	}
	for(int i = 0; i < max_number; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

