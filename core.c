#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "core.h"

#define MAX_LEN 20

/* simple data structure : chained-list containing the number of the nodes */
struct chained_list {
	unsigned long int number;
	int value;
	struct chained_list * next;
};

/* function to create a table of predecessors */
int ** genereate_predecessor(int nb_nodes, int depart) {
	int i;
	/*We create a structure to enter the list of predecessors with their weight*/
	int **predecessor = (int **) calloc(nb_nodes, sizeof(unsigned long int *));
	/* For each row in row[0] we will store the weight and in row[1] we will store the predecessor */
	for (i=0; i<nb_nodes; i++) {
		predecessor[i] = (int *) calloc(2, sizeof(unsigned long int));
		if(i==depart) {
			predecessor[i][0] = 0;
			predecessor[i][1] = INT_MAX;
		}
		else {
			predecessor[i][0] = INT_MAX;
			predecessor[i][1] = INT_MAX;
		}

	}
	
	return predecessor;
}

/* function to generate a random graph */
int ** random_graph(int nb_nodes, float completeness) {
	
	unsigned long int i, j;
	
	int **tableau = (int **) calloc(nb_nodes, sizeof(unsigned long int *));
	
	/* just to have a different configuration each time  */
    srand(time((time_t *)0));
	
	/* allocate space for our little matrix */
	for (i=0; i<nb_nodes; i++) {
		tableau[i] = (int *) calloc(nb_nodes, sizeof(unsigned long int));
	}
	
	/* generate random arc with its value */
	for (i=0;i<nb_nodes;i++){
		/* generate the cost of the arc */
		for (j=0; j<nb_nodes; j++) {
			if (j != i) {
				/* we fill the value only if we are under the percentage of completeness */
				if (rand()/(RAND_MAX + 1.) < completeness) {
					tableau[i][j] = (int) (1 + round(rand()/(RAND_MAX + 1.) * (MAX_LEN -1 )));
				}
				else {
					tableau[i][j] = INT_MAX;
				}
				
			}
			else {
				tableau[i][i] = 0;
			}
		}
	}
	
	return tableau;
}

/* function to convert a matrix into a chained list */
struct chained_list ** convert_matrix(int ** tab, int nb_nodes) {
	unsigned long int i, j;
	struct chained_list * pointer;
	
	/* we allocate nb_nodes cells for the chained list tab */
	struct chained_list **list = (struct chained_list **) calloc(nb_nodes, sizeof(struct chained_list *));
	
	for (i=0; i<nb_nodes; i++) {
		list[i]= NULL;
		for (j=0; j<nb_nodes; j++) {
			/* to convert the matrix we want to take only the cells set */
			if (j != i && tab[i][j] != INT_MAX) {
				/* if list[i] is NULL, it means we have to allocate a struct in it */
				if (list[i] == NULL) {
					list[i] = (struct chained_list *) malloc(sizeof(struct chained_list));
					/* we copy the address of the new struct in the pointer */
					pointer = list[i];
				}
				/* else it means we already filled the cell with at least one value, and we have
				 * created a pointer */
				else {
					pointer->next = (struct chained_list *) malloc(sizeof(struct chained_list));
					pointer = pointer->next;
				}
				/* storing time ! */
				pointer->number = j;
				pointer->value = tab[i][j];
			}
		}
		pointer->next = NULL;
	}
	
	return &list[0];
}

/* function to free a chained_list */
void free_list(struct chained_list ** list, int nb_nodes) {
	unsigned long int i;
	
	struct chained_list * pointer;
	struct chained_list * tmp;
	
	for (i=0; i<nb_nodes; i++) {
		pointer = list[i];
		/* to suppress the case of an empty row */
		if (pointer != NULL) {
			/* then we free as we go further in the graph */
			while (pointer->next != NULL) {
				tmp = pointer->next;
				free(pointer);
				pointer = tmp;
			}
		}
	}
	free(list);
	printf("list memory freeed\n");
}

/* function to free a matrix, not so complicated */
void free_matrix(int ** matrix, int nb_nodes) {
	unsigned long int i;
	for (i=0; i<nb_nodes; i++) {
		free(matrix[i]);
	}
	free(matrix);
	printf("matrix memory free\n");
}

/* function to find the shorter way in a graph using bellman-ford */
int** bellman_ford_matrix(int ** matrix,int nb_nodes, int depart) {
	int i,j;
	int ** predecessor;
	int compteur;
	/*Initialization of the variables*/
	compteur = INT_MAX;
	predecessor = genereate_predecessor(nb_nodes, depart);
	/*While the table of predecessors is not modified we run the code*/
	while(compteur) {
		compteur = 0;
		/*We go through the matrix by column to get the list of successors*/
		for (j=0; j<nb_nodes; j++) {
			for (i=0; i<nb_nodes; i++) {
				/*We add this to prevent an int overflow*/
				if (predecessor[i][0] != INT_MAX) {
					/*If the lenght to reach a node is superior to the length to reach one of his
					 predecessor plus the length of the edge to go from the predecessor to the node
					 then it is a shorter way so we change the predecessor of the node and the value
					 to reach the node.*/
					int new_length;
					new_length = predecessor[i][0]+matrix[i][j];
					if (predecessor[j][0] >= new_length) {
						predecessor[j][0] = new_length;
						predecessor[j][1] = i;
						compteur++;
					}
				}
			}
		} 
	}
	return predecessor;
}