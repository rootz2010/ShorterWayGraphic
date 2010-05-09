/*
 *  io.h
 *  ShorterWayGraphic
 *
 *  Created by Benjamin Loulier on 09/05/10.
 *  Copyright 2010 Ecole nationale supèrieure des Mines de Saint-Etienne. All rights reserved.
 *
 */

int write_matrix(const char * path, int ** matrix, int nb_nodes);
int read_nb_nodes(const char * path);
int** read_matrix(const char * path, int nb_nodes);