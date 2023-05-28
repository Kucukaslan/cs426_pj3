
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // for int32_t

// @Kucukaslan's additions

// read_vector function
// reads a vector from a file
// returns 1 if successful, 0 otherwise
// the vector is stored in the vector parameter

int read_vector(char *filename, int *numVertices, double **vector)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		return 0;
	}
	else
	{
		// read the number of vertices
		fscanf(file, "%d", numVertices);
		printf("numVertices = %d\n", *numVertices);
		// allocate memory for the vector
		*vector = (double *)malloc(*numVertices * sizeof(double));
		printf("reading the vector of %d elements\n", *numVertices);

		for (int i = 0; i < *numVertices; i++)
		{
			fscanf(file, "%lf", &((*vector)[i]));
			// printf("%3.20lf\n", (*vector)[i]);
		}
		fclose(file);
		return 1;
	}
}

/*
 * reads a matrix of doubleing numbers in CSR format, each integer number separated by line,
 * first line defines the number of rows (n), the next lines is Number of Non-Zero (nnz) elements,
 * next n+1 lines define the offsets array, the next nnz lines define the corresponding edges, and next nnz weights array,
 */
int read_matrix(char *filename, int *numRows, int *numEdges, int **offsets, int **edges, double **weights)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		return 0;
	}
	else
	{
		// read the number of vertices
		fscanf(file, "%d", numRows);
		fscanf(file, "%d", numEdges);
		printf("numRows = %d, numEdges = %d\n", *numRows, *numEdges);
		// allocate memory for the vector
		*offsets = (int *)malloc((*numRows + 1) * sizeof(int));
		*edges = (int *)malloc((*numEdges) * sizeof(int));
		*weights = (double *)malloc((*numEdges) * sizeof(double));

		for (int i = 0; i < *numRows + 1; i++)
		{
			fscanf(file, "%d", &((*offsets)[i]));
			// printf("%d\n", (*offsets)[i]);
		}
		for (int i = 0; i < *numEdges; i++)
		{
			fscanf(file, "%d", &((*edges)[i]));
			// printf("%d\n", (*edges)[i]);
		}
		for (int i = 0; i < *numEdges; i++)
		{
			fscanf(file, "%lf", &((*weights)[i]));
			// printf("%3.20lf %d/%d\n", (*weights)[i], i, *numEdges);
		}

		fclose(file);
		return 1;
	}
}

// writes a vector to a file
// returns 1 if successful, 0 otherwise
int write_vector(char *filename, int numVertices, double *vector)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
	{
		return 0;
	}
	else
	{
		// write the number of vertices
		fprintf(file, "%d\n", numVertices);
		// write the vector
		for (int i = 0; i < numVertices; i++)
		{
			fprintf(file, "%3.20lf\n", vector[i]);
		}
		fclose(file);
		return 1;
	}
}
