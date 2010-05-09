/*
 *  io.c
 *  ShorterWayGraphic
 *
 *  Created by Benjamin Loulier on 09/05/10.
 *  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

/*Function to allocate an empty matrix*/
/* function to create a table of predecessors */
int ** generate_empty_matrix(int nb_nodes) {
	int i;
	/*We create a structure to enter the list of predecessors with their weight*/
	int **predecessor = (int **) calloc(nb_nodes, sizeof(int *));
	/* For each row in row[0] we will store the weight and in row[1] we will store the predecessor */
	for (i=0; i<nb_nodes; i++) {
		predecessor[i] = (int *) calloc(nb_nodes, sizeof(int));
	}
	
	return predecessor;
}
/*Function to convert the string into the apropriate value*/
int fill_matrix(char * string) {
	if (!strcmp(string,"--")) {
		return INT_MAX;
	}
	else {
		return atoi(string);
	}
}

int read_nb_nodes(const char * path) {
	int nb_nodes;
	FILE * file;
	file = fopen(path, "r");
	if (file==NULL) {
		return 0;
	}
	else {
		fscanf(file, "nb_nodes: %d\n",&nb_nodes);
	}
	fclose(file);
	return nb_nodes;
}

int ** read_matrix(const char * path, int nb_nodes) {
	
	int i,j;
	int ** matrix;
	char buff[100];
	FILE * file;
	file = fopen(path, "r");
	
	if (file==NULL) {
		return NULL;
	}
	else {
		int temp;
		fscanf(file, "nb_nodes: %d\n", &temp);
		matrix = generate_empty_matrix(nb_nodes);
		for (i=0; i<nb_nodes; i++) {
			for (j=0; j<nb_nodes; j++) {
				if(j!=nb_nodes-1) {
					fscanf(file, "%s ", buff);
				}
				else {
					fscanf(file, "%s \n", buff);
				}
				matrix[i][j]=fill_matrix(buff);
			}
		}
	}
	fclose(file);
	
	return matrix;
}

/*Fonction to save a matrix generated*/
int write_matrix(const char * path, int ** matrix, int nb_nodes) {
	int i,j;
	FILE * file;
	file = fopen(path, "a+");
	if (file==NULL) {
		return 1;
	}
	else {
		fprintf(file,"nb_nodes: %d\n",nb_nodes);
		for(i=0;i<nb_nodes;i++) {
			for(j=0;j<nb_nodes;j++) {
				if(matrix[i][j]!=INT_MAX) {
					fprintf(file,"%-2d ", matrix[i][j]);
				}
				else {
					fprintf(file, "-- ");
				}
				
			}
			fprintf(file,"\n");
		}
	}
	fclose(file);
	return 0;
}