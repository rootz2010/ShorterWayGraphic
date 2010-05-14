/*
 *  io.h
 *  ShorterWayGraphic
 *
 *  Created by Benjamin Loulier on 09/05/10.
 *  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
 *
 */

/*Function to allocate an empty matrix*/
/* function to create a table of predecessors */
int ** generate_empty_matrix(int nb_nodes);
/*Function to convert the string into the apropriate value*/
int fill_matrix(char * string);

int write_matrix(const char * path, int ** matrix, int nb_nodes);
int read_nb_nodes(const char * path);
int** read_matrix(const char * path, int nb_nodes);