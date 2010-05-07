/* simple data structure : chained-list containing the number of nodes */
struct chained_list;
/*
 * Function to generate a random graph, this graph has a matrix form
 * if you want to have achained list form use the convert_matrix function
 */
int ** random_graph(int nb_nodes, float completeness);
/* function to convert a matrix into a chained list */
struct chained_list ** convert_matrix(int ** tab, int nb_nodes);
/*function to free a list*/
void free_list(struct chained_list ** list, int nb_nodes);
/*function to free a matrix*/
void free_matrix(int ** matrix, int nb_nodes);

/* function to find the shorter way using dantzig */
int*** dantzig_matrix(int ** matrix, int nb_nodes);

/*bellman ford algorithm*/
int** bellman_ford_matrix(int ** matrix,int nb_nodes, int depart);
/*Dikjstra matrix*/
int ** dijkstra_matrix(int ** matrix, int nb_nodes, int depart);