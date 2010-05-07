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
int ** generate_predecessor(int nb_nodes, int depart) {
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

/******************************************************************/
/**********************BELLMAN FORD********************************/
/******************************************************************/

/* function to find the shorter way in a graph using bellman-ford */
int** bellman_ford_matrix(int ** matrix,int nb_nodes, int depart) {
	int i,j;
	int ** predecessor;
	int compteur;
	/*Initialization of the variables*/
	compteur = INT_MAX;
	predecessor = generate_predecessor(nb_nodes, depart);
	/*While the table of predecessors is not modified we run the code*/
	while(compteur) {
		compteur = 0;
		/*We go through the matrix by column to get the list of successors*/
		for (j=0; j<nb_nodes; j++) {
			for (i=0; i<nb_nodes; i++) {
				int pred, edge_weight; 
				pred = predecessor[i][0];
				edge_weight = matrix[i][j];
				/*We add this to prevent an int overflow and evaluate only the part where an hedge exists*/
				if (pred != INT_MAX && j!=i && edge_weight != INT_MAX) {
					/*If the lenght to reach a node is superior to the length to reach one of his
					 predecessor plus the length of the edge to go from the predecessor to the node
					 then it is a shorter way so we change the predecessor of the node and the value
					 to reach the node.*/
					int new_length;
					new_length = pred+edge_weight;
					if (predecessor[j][0] > new_length) {
						//Add a check for negative cycles
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


/******************************************************************/
/**********************DIJKSTRA************************************/
/******************************************************************/

/*Function to generate a list of nodes to visit from a table of predecessors*/
struct chained_list * nodes_to_visit(int ** predecessor, int nb_nodes) {
	int i;
	struct chained_list * head;
	struct chained_list * cursor;
	head = (struct chained_list *) malloc(sizeof(struct chained_list));
	head->number = 0;
	head->value = predecessor[0][0];
	cursor = head;
	for(i=0; i<nb_nodes-1; i++) {
		cursor->next = (struct chained_list *) malloc(sizeof(struct chained_list));
		cursor->next->number = i+1;
		cursor->next->value = predecessor[i+1][0];
		cursor = cursor->next;
	}
	cursor->next = NULL;
	return head;
}

/*Function to find the node with the minimum weight int the to_visit list*/
struct chained_list * find_node_min(struct chained_list * head, int ** predecessor) {
	int min;
	struct chained_list * node_min;
	struct chained_list * cursor;
	min = INT_MAX;
	cursor = head;
	while (cursor) {
		int temp = predecessor[cursor->number][0];
		if (temp <= min) {
			min = temp;
			node_min = cursor;
		}
		cursor = cursor->next;
	}
	return node_min;
}

/*Function to remove a node from a list*/
struct chained_list * remove_node(struct chained_list * head, struct chained_list * node) {
	struct chained_list * new_head;
	struct chained_list * cursor;
	new_head = head;
	cursor = head;
	if (node == head) {
		new_head = head->next;
		free(node);
	}
	else {
		while(cursor) {
			if (cursor->next == node) {
				cursor->next = node->next;
				free(node);
			}
			cursor = cursor->next;
		}
	}
	return new_head;
}

/*Function to find the shorter way using dijkstra*/
int ** dijkstra_matrix(int ** matrix, int nb_nodes, int depart) {
	int i,j;
	int ** predecessor;
	int * visited;
	struct chained_list * to_visit;
	visited = (int *) calloc(nb_nodes, sizeof(int));
	predecessor = generate_predecessor(nb_nodes, depart);
	to_visit  = nodes_to_visit(predecessor, nb_nodes);
	while(to_visit) {
		/*We find the node witht the minimum value in the list and we visit it*/
		struct chained_list * node_min = find_node_min(to_visit, predecessor);
		/*If the node is connected to the graph we look at all the non visited children*/
		i=node_min->number;
		/*We mark the node as visited*/
		visited[i] = 1;
		if(predecessor[i][0]!=INT_MAX) {
			for(j=0; j<nb_nodes; j++) {
				if (!visited[j]) {
					if (matrix[i][j] != INT_MAX && i!=j && predecessor[i][0]+matrix[i][j]<predecessor[j][0]) {
						predecessor[j][0] = predecessor[i][0]+matrix[i][j];
						predecessor[j][1] = i;
					}
				}
			}
		}
		/*We remove the node_min from the to_visit list*/
		to_visit = remove_node(to_visit, node_min);
	}
	return predecessor;
}