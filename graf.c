
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node
{
	int x;
	int y;
        int* edges;
        struct Node* next;
} *node_t, node_o;
node_t * create_graph()
{
        FILE *in = fopen("graf.csrrg", "r");
        if(in == NULL)
        {
                printf("Blad podczas wczytywania pliku.\n");
        }
        int max_number;
        fscanf(in, "%d", &max_number);
        printf("Maksymalna liczba wierzcholkow w wierszu wynosi: %d\n", max_number);
        int number_of_nodes = 0;
        char c = '_';
        int x;
	int buf[8192];
        while(c != '\n' && fscanf(in, "%d%c", &buf[number_of_nodes], &c) == 2)
        { 
                number_of_nodes++;	
        }
        printf("Liczba wierzcholkow w grafie wynosi: %d\n", number_of_nodes);
		
	node_t *new_graph = malloc(number_of_nodes * sizeof(new_graph));
	int first;
	int second;
	int row_counter = 0;
	fscanf(in, "%d%c%d%c", &first, &c, &second, &c);
	printf("FIRST:%dx%d\n", first, second);
	for(int i = 0; i < max_number - 2; i++)
		{
			first = second;
			fscanf(in, "%d%c", &second, &c);
		for(int j = first; j < second; i++)
		{
			new_graph[j] = malloc(sizeof(node_o));
			new_graph[j]->x = i;	
			new_graph[j]->y = buf[j];
			printf("%dx%d\n", new_graph[j]->x , new_graph[j]->y);
		}
	}
	for(int i = 0; i < number_of_nodes; i++)
	{
		printf("%dx%d\n", new_graph[i]->x , new_graph[i]->y);
	}
	fclose(in);
}

int main()
{
	
	create_graph();
	return 0;
}


