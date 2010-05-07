#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "core.h"

#define MAX_LEN 20

/* simple data structure : chained-list containing the number of the nodes */
struct chained_list {
	int number;
	int value;
	struct chained_list * next;
};

/* function to create a table of predecessors */
int ** genereate_predecessor(int nb_nodes, int depart) {
	int i;
	/*We create a structure to enter the list of predecessors with their weight*/
	int **predecessor = (int **) calloc(nb_nodes, sizeof(int *));
	/* For each row in row[0] we will store the weight and in row[1] we will store the predecessor */
	for (i=0; i<nb_nodes; i++) {
		predecessor[i] = (int *) calloc(2, sizeof(int));
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
	
	int i, j;
	
	int **tableau = (int **) calloc(nb_nodes, sizeof(int *));
	
	/* just to have a different configuration each time  */
    srand(time((time_t *)0));
	
	/* allocate space for our little matrix */
	for (i=0; i<nb_nodes; i++) {
		tableau[i] = (int *) calloc(nb_nodes, sizeof(int));
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
	int i, j;
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
	int i;
	
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
	int i;
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
				/*We add this to prevent an int overflow and evaluate only the part where an hedge exists*/
				if (predecessor[i][0] != INT_MAX && j!=i && matrix[i][j] != INT_MAX) {
					/*If the lenght to reach a node is superior to the length to reach one of his
					 predecessor plus the length of the edge to go from the predecessor to the node
					 then it is a shorter way so we change the predecessor of the node and the value
					 to reach the node.*/
					int new_length;
					new_length = predecessor[i][0]+matrix[i][j];
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

int*** dantzig_init_matrix(int nb_nodes) {
	int i,j;
	
	/* we need to initialise the matrix which will be given as output */
	int*** output_matrix = (int ***) calloc(nb_nodes, sizeof(int **));
	/* the output_matrix stores in a three dimension graph the predecessor of the node j to
	 * go to the node i, with the distance between i and j */
	for (i=0; i<nb_nodes; i++) {
		output_matrix[i] = (int **) calloc(nb_nodes, sizeof(int *));
		for (j=0; j<nb_nodes; j++) {
			/* For each row in row[0] we will store the weight and in row[1] we will store 
			 * the predecessor */
			output_matrix[i][j] = (int *) calloc(2, sizeof(int *));
			if (j!=i) {
				output_matrix[i][j][1] = 0;
				output_matrix[i][j][0] = INT_MAX; /* we didn't evaluate the cost yet */
			}
			/* on the diagonal */
			else {
				output_matrix[i][j][1] = i;
				output_matrix[i][j][0] = 0; /* no cost to go from i to i */
			}
		}
	}
	return output_matrix;
}

/* function to find the shorter way between all nodes in a graph using dantzig */
int*** dantzig_matrix(int ** matrix, int nb_nodes) {
	int i,j,k;
	int circuit, length;
	int *** output_matrix;
	k = 0;
	circuit = 0;
	
	FILE * file;
	file = open_log();
	fprintf(file, "starting dantzig\n");
	
	output_matrix = dantzig_init_matrix(nb_nodes);
	
	/* evaluation of the distances between the nodes : we grow the matrix starting from a 
	 * small graph to finish with the whole graph */
	while (k < nb_nodes - 1 && circuit == 0) { /* counter to increment the graph */
		fprintf(file, "k : %d circuit : %d\n", k, circuit);
		i = 0;
		while (i <= k && circuit == 0) { /* counter to increment the initial node */
			for (j=0; j<=k; j++) { /* counter to increment the destination node */
				/* calculation of the length between node i and node k+1 : we know the distance
				 * between all the nodes at the rank k, and we want to visit the k+1 node :
				 * the algorithm tests all the nodes of arrival until k, and try to minimize
				 * the distance between i and j */
				fprintf(file,"i : %d, j : %d\n", i, j);
				/* test to avoid an int overflow */
				if (output_matrix[i][j][0] != INT_MAX && matrix[j][k+1] !=INT_MAX) {
					length = output_matrix[i][j][0] + matrix[j][k+1];
				}
				else {
					length = INT_MAX;
				}
				fprintf(file,"length i -> k+1 : %d\n", length);
				if (length < output_matrix[i][k+1][0]) {
					output_matrix[i][k+1][0] = length;
					output_matrix[i][k+1][1] = j;
				}
				/*  calculation in the other direction */
				/* test to avoid an int overflow */
				if (output_matrix[j][i][0] != INT_MAX && matrix[k+1][j] != INT_MAX) {
					length = output_matrix[j][i][0] + matrix[k+1][j];
				}
				else {
					length = INT_MAX;
				}
				fprintf(file,"length k+1 -> i : %d\n", length);
	 			if (length < output_matrix[k+1][i][0]) {
					output_matrix[k+1][i][0] = length;
					if (i == j) {
						output_matrix[k+1][i][1] =  k+1;
					}
					else {
						output_matrix[k+1][i][1] = output_matrix[j][i][1];
					}
				}
			}
			/* if the distance between k+1 and i plus the one in the other direction is < 0 */
			if (output_matrix[k+1][i][0] + output_matrix[i][k+1][0] < 0 && output_matrix[k+1][i][0] != INT_MAX && output_matrix[i][k+1][0] != INT_MAX) {
				/* we have a circuit */
				fprintf(file,"failed : circuit\n");
				circuit = 1;
			}
			i++;
		}
		
		/* optimization of the matrix */
		if (circuit == 0) {
			for (i=0;i<=k;i++) {
				for (j=0;j<=k;j++) {
					fprintf(file,"opt : i : %d, j : %d\n", i, j);
					/* we calculate again the distance, to find a new minimum if possible */
					/* test to avoid an int overflow */
					if (output_matrix[i][k+1][0] != INT_MAX && output_matrix[k+1][j][0] != INT_MAX) {
						length = output_matrix[i][k+1][0] + output_matrix[k+1][j][0];
					}
					else {
						length = INT_MAX;
					}
					fprintf(file,"length i -> k+1 : %d\n", length);
					if (length < output_matrix[i][j][1]) {
						/* if so, we have to replace the corresponding cell */
						fprintf(file, "looks like we found an optimization !");
						output_matrix[i][j][0] = length;
						output_matrix[i][j][1] = output_matrix[k+1][j][1];
					}
				}
			}
		}
		k++;
		fprintf(file,"===========\n");
	}
	fprintf(file, "###########\n");
	fprintf(file, "\n");
	fclose(file);
	return output_matrix;
}

/* function written for debugging purpose : it redirects stderr to a log file */
FILE* open_log() {
	FILE* fp;
    fp = fopen("shorter_way_graphic.log", "a");
    time_t todaytime;
    todaytime = time(NULL);
    fprintf(fp, ctime(&todaytime));
    fprintf(fp, "starting shorter way\n");
	return fp;
}