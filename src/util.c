
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // for int32_t

// TODO: refactor function to use float weights

int read_file(char *file_name, int *numVertices, int *numEdges, int **offsets)
{
	printf("Reading the file...\n");
	FILE *inputFile = fopen(file_name, "r");
	if (inputFile)
	{
		int success;

		success = fscanf(inputFile, "%d", &*numVertices);
		if (!success)
		{
			printf("Bad File format!\n");
			return 0;
		}
		success = fscanf(inputFile, "%d", &*numEdges);
		if (!success)
		{
			printf("Bad File format!\n");
			return 0;
		}

		// topologicalSize includes the edge weights
		int topologicalSize = (*numVertices + 1 + (*numEdges * 2)) * sizeof(int);

		printf("numVertices = %d, numEdges = %d\n", *numVertices, *numEdges);

		printf("Graph data footprint=%.3f KB ~ %.3f MB ~ %.3f GB\n", topologicalSize / 1024.0, topologicalSize / (1024.0 * 1024), topologicalSize / (1024.0 * 1024 * 1024));

		*offsets = (int *)malloc(topologicalSize);

		for (int i = 0; i < (*numVertices + 1 + (*numEdges) * 2); i++)
		{
			success = fscanf(inputFile, "%d", &((*offsets)[i]));
			if (success == EOF || success == 0)
			{
				printf("Bad File format!\n");
				return 0;
			}
		}
		fclose(inputFile);
		return 1;
	}
	printf("Could not open the file!\n");
	return 0;
}

void printResults(char *fileName, int *distances, int numVertices)
{
	FILE *inputFile = fopen(fileName, "w");
	if (inputFile)
	{
		// Output the final values
		for (int i = 0; i < numVertices; i++)
		{
			fprintf(inputFile, "%d\n", (int32_t)distances[i]);
		}
	}
	else
	{
		printf("Could not open the file!\n");
	}
}

// @Kucukaslan's additions

// read_vector function
// reads a vector from a file
// returns 1 if successful, 0 otherwise
// the vector is stored in the vector parameter

int read_vector(char *filename, int *numVertices, float **vector)
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
		*vector = (float *)malloc(*numVertices * sizeof(float));
		printf("initailized vector\n");

		for (int i = 0; i < *numVertices; i++)
		{
			fscanf(file, "%f", &((*vector)[i]));
			printf("%f\n", (*vector)[i]);
		}
		fclose(file);
		return 1;
	}
}

/*
 * reads a matrix of floating numbers in CSR format, each integer number separated by line,
 * first line defines the number of rows (n), the next lines is Number of Non-Zero (nnz) elements,
 * next n+1 lines define the offsets array, the next nnz lines define the corresponding edges, and next nnz weights array,
 */
int read_matrix_csr(char *filename, int *numRows, int *numEdges, int **offsets, int **edges, float **weights)
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
		*weights = (float *)malloc((*numRows) * sizeof(float));

		for (int i = 0; i < *numRows + 1; i++)
		{
			fscanf(file, "%d", &((*offsets)[i]));
			printf("%d\n", (*offsets)[i]);
		}
		for (int i = 0; i < *numEdges; i++)
		{
			fscanf(file, "%d", &((*edges)[i]));
			printf("%d\n", (*edges)[i]);
		}
		for (int i = 0; i < *numEdges; i++)
		{
			fscanf(file, "%f", &((*weights)[i]));
			printf("%f\n", (*weights)[i]);
		}

		fclose(file);
		return 1;
	}
}

// writes a vector to a file
// returns 1 if successful, 0 otherwise
int write_vector(char *filename, int numVertices, float *vector)
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
			fprintf(file, "%f\n", vector[i]);
		}
		fclose(file);
		return 1;
	}
}
