
#ifndef UTIL_H_
#define UTIL_H_

#define EPSILON 1e-10 // it might be too small for double

/*
 *		reads a vector file in CSC format, each integer number separated by line,
 *		First line defines the number of vertices, the next lines define the vector.
 *
 *       @params:
 *		file_name: name of the file
 *		vector: vector array
 *
 *       Returns 0 if there is a file format or a reading problem, 1 if successful
 *
 *       Usage Example:
 *       int *numVertices = malloc(sizeof(int));
 *       double **vector;
 *      int success = read_vector(argv[1], num  &vector);
 *      if(success){
 *     	//Do the computation
 *     }
 **/
int read_vector(char *filename, int *numVertices, double **vector);

/*
 * reads a matrix of doubleing numbers in either of CSR or CSC format, each integer number separated by line,
 * first line defines the number of rows (n), the next lines is Number of Non-Zero (nnz) elements,
 * next n+1 lines define the offsets array, the next nnz lines define the corresponding edges/columns, and next nnz weights array,
 */
int read_matrix(char *filename, int *numRows, int *numEdges, int **offsets, int **edges, double **weights);

int write_vector(char *filename, int numVertices, double *vector);

#endif /* UTIL_H_ */
