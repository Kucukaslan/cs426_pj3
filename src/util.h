
#ifndef UTIL_H_
#define UTIL_H_

#define EPSILON_e5 1e-5
// #define EPSILON_e10 = 1e-10; do not use this, it is too small for float

/*
 *		reads a a graph file in CSC format, each integer number separated by line,
 *		First (numRows+1) lines define offsets array, the next (numColumns) lines define edges array,
 *      and the next (numColumns) lines define the weights array.
 *
 *
 *       This is used for testing on regular pc
 *
 *       @params:
 *		file_name: name of the file
 *		numRows: number of rows(vertices) in the file
 *		numColumns: number of columns (edges) in the file
 *      offsets: offsets(vertices) array
 *
 *       Returns 0 if there is a file format or a reading problem, 1 if successful
 *
 *       Usage Example:
 *       int numVertices,numEdges,*offsets,*edges, *weights;
 *       int success = read_file(argv[1],&numVertices,&numEdges,&offsets,&edges);
 *       if(success){
 *       	edges = &(offsets[numVertices + 1]);
 *       	weights = &(offsets[numVertices + 1 + numEdges]);
 *           //Do the computation
 *       }
 **/

int read_file(char *file_name, int *numRows, int *numColumns, int **offsets);

// prints the results to a file
void printResults(char *fileName, int *distances, int numVertices);

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
 *       float **vector;
 *      int success = read_vector(argv[1], num  &vector);
 *      if(success){
 *     	//Do the computation
 *     }
 **/
int read_vector(char *filename, int *numVertices, float **vector);

/*
 * reads a matrix of floating numbers in CSR format, each integer number separated by line,
 * first line defines the number of rows (n), the next lines is Number of Non-Zero (nnz) elements,
 * next n+1 lines define the offsets array, the next nnz lines define the corresponding edges, and next nnz weights array,
 */
int read_matrix_csr(char *filename, int *numRows, int *numEdges, int **offsets, int **edges, float **weights);

int write_vector(char *filename, int numVertices, float *vector);

#endif /* UTIL_H_ */
