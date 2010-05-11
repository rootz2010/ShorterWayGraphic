/* simple data structure : chained-list containing the id of the node, the value, and a pointer
 * to the next node */
struct chained_list {
	int number;
	int value;
	struct chained_list * next;
};


/* Function to generate a random graph, this graph has a matrix form
 * if you want to have a chained list form use the convert_matrix function
 */
int ** random_graph(int nb_nodes, float completeness, float negative_probability);
/* function to convert a matrix into a chained list */
struct chained_list ** convert_matrix(int ** tab, int nb_nodes);
/*function to free a list*/
void free_list(struct chained_list * list);
/*function to free a table of chained list*/
void free_table_list(struct chained_list ** list, int nb_nodes);
/*function to free a 2 dimensions matrix*/
void free_2Dmatrix(int ** matrix, int row);
/* function to free a 3 dimensions matrix */
void free_3Dmatrix(int *** matrix, int row, int col);

/* function to find the shorter way using dantzig */
int*** dantzig_matrix(int ** matrix, int nb_nodes);
int*** dantzig_init_matrix(int nb_nodes);
int ** dantzig_extract_predecessor(int*** matrix, int nb_nodes);

/*bellman ford algorithm, this function returns a predecessor matrix*/
int** bellman_ford_matrix(int ** matrix,int nb_nodes, int depart);

/*Dikjstra matrix, this function returns a predecessor matrix*/
int** dijkstra_matrix(int ** matrix, int nb_nodes, int depart);

/* function to find the shorter way from the predecessor matrix */
struct chained_list* find_short(int** matrix, int departure, int arrival);