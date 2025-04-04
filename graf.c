
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node
{
	int x;
	int y;
        //int* edges;
	//int edge_counter;
        //struct Node* next;
} *node_t, node_o;
node_t * create_graph()
{
        FILE *in = fopen("graf.csrrg", "r");
	FILE *out = fopen("wynik.txt", "w");
        if(in == NULL)
        {
                printf("Blad podczas wczytywania pliku.\n");
        }
        int max_number;
        fscanf(in, "%d", &max_number);
        printf("Maksymalna liczba wierzcholkow w wierszu wynosi: %d\n", max_number);
        int number_of_nodes = 0;
        char c = '_';
	int buf[8192];
        while(c != '\n' && fscanf(in, "%d%c", &buf[number_of_nodes], &c) == 2)
        { 
                number_of_nodes++;	
        }
        printf("Liczba wierzcholkow w grafie wynosi: %d\n", number_of_nodes);
		
	node_t *new_graph = malloc(number_of_nodes * sizeof(node_t));
	int first;
	int second;
	int row_counter = 0;
	fscanf(in, "%d%c%d%c", &first, &c, &second, &c);
	int ** new_matrix = malloc(max_number * sizeof(int*));
	for(int i = 0; i < max_number; i++)
	{
		new_matrix[i] = calloc(max_number, sizeof(int));
	}
	for(int i = first; i < second; i++)
	{
		new_graph[i] = malloc(sizeof(node_o));
		new_graph[i]->x = 0;
		new_graph[i]->y = buf[i];
		new_matrix[0][buf[i]] = 1;
	}
	
	//printf("FIRST:%dx%d\n", first, second);
	for(int i = 1; i < max_number; i++)
		{
			first = second;
			fscanf(in, "%d%c", &second, &c);
			//printf("second: %dx%d\n", second, i);
		for(int j = first; j < second; j++)
		{
			new_graph[j] = malloc(sizeof(node_o));
			new_graph[j]->x = i;	
			new_graph[j]->y = buf[j];
			new_matrix[i][buf[j]] = 1;
			//printf("%dx%d\n", new_graph[j]->x , new_graph[j]->y);
		}
	}

	for(int i = 0; i < max_number; i++)
	{
		fprintf(out, "[");
		for(int j = 0; j < max_number-1; j++)
			fprintf(out,"%d. ", new_matrix[i][j]);
			
		fprintf(out, "%d.]", new_matrix[i][max_number-1]);
		fprintf(out, "\n");			
	}

	for(int i = 0; i < max_number; i++)
		free(new_matrix[i]);
	
	free(new_matrix);

	for(int i = 0; i < number_of_nodes; i++)
	{
		//printf("%dx%d\n", new_graph[i]->x , new_graph[i]->y);
	}

	int **matrix = malloc(number_of_nodes * sizeof(int*));
	for(int i = 0; i < number_of_nodes; i++)
	{
		matrix[i] = calloc(number_of_nodes, sizeof(int));
	}
	int counter = 0;
	c = '_';
	while(c != '\n' && fscanf(in, "%d%c", &buf[counter], &c) == 2)
	{
		//printf("BUFIKSIOR[%d] : %d\n", counter, buf[counter]);
		counter++;	
	}
	
	fscanf(in, "%d%c%d%c", &first, &c, &second, &c);
	for(int i = first + 1; i < second; i++)
	{
		int start = buf[first];
		int end = buf[i];
		matrix[start][end] = 1;
		matrix[end][start] = 1;
		fprintf(out, "Krawedz %d - %d\n", start, end);
	}
	c = '_';
	first = second;
	while(fscanf(in, "%d%c", &second, &c) != EOF)
	{
		int start = buf[first];
		for(int i = first + 1; i < second; i++)
		{
			int end = buf[i];
			matrix[start][end] = 1;
			matrix[end][start] = 1;
			fprintf(out, "Krawedz %d - %d\n", start, end);
		}
		first = second;
	}
	int start = buf[first];
	for (int i = first + 1; i < counter; i++)
	{
    		int end = buf[i];
    		matrix[start][end] = 1;
    		matrix[end][start] = 1;
    		fprintf(out, "Krawedz: %d - %d\n", start, end);
	}
	/*
	int def = 0;
	int counter_y = 0;
	fscanf(in, "%d%c%d%c", &first, &c, &second, &c);	
	for(int i = first + 1; i < second; i++)
	{
		new_graph[buf[first]]->edges = malloc((second - first - 1) *sizeof(int));
		new_graph[buf[first]]->edges[counter_y] = buf[i];
		printf("Krawedzie wierzcholka %d: %d\n", buf[first], new_graph[buf[first]]->edges[counter_y]);
		counter_y++;
	}
	c = '_';
	first = second;
	counter_y = 0;
	while(fscanf(in, "%d%c", &second, &c) != EOF)	
	{
			int nr_node = buf[first];
			def = second - first - 1;
			new_graph[nr_node]->edges = malloc(def * sizeof(int));
			for(int i = first + 1; i < second; i++)
			{
				new_graph[nr_node]->edges[counter_y] = buf[i];
	       			printf("Krawedzie[%d] : %d\n", nr_node, new_graph[nr_node]->edges[counter_y]);
				counter_y++;
			}
		counter_y = 0;
		first = second;		
	}
		int nr_node = buf[first];
                def = counter - first - 1;
                new_graph[nr_node]->edges = malloc(def * sizeof(int));
                for(int i = first + 1; i < counter; i++)
                {
                	new_graph[nr_node]->edges[counter_y] = buf[i];
                        printf("Krawedzie[%d] : %d\n", nr_node, new_graph[nr_node]->edges[counter_y]);
                        counter_y++;
		}*/
	fclose(in);
}

int main()
{
	
	create_graph();
	return 0;
}


