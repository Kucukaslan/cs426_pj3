
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "util.h"
#include <stdbool.h>
#include <math.h>
#include <time.h>

// an example call is
// %my_program input_matrix_A input_matrix_b output_matrix_X
int main(int argc, char *argv[])
{
    // start the  monotonic timer
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // check if the number of arguments is correct
    if (argc != 4)
    {
        printf("Usage: %s <input_file_A> <input_file_b> <output_file>\n", argv[0]);
        return 1;
    }

    // offsets is the rows
    // why did they named it like that? To confuse us?

    // read the input file
    int numVertices, numEdges, *offsets, *columns;
    double *weights;
    int success = read_matrix(argv[1], &numVertices, &numEdges, &offsets, &columns, &weights);
    if (!success)
    {
        printf("Error reading file %s\n", argv[1]);
        return 1;
    }

    // read the vector b from the input file
    printf("Reading the vector b...\n");
    int numVerticesB;
    double *b;
    success = read_vector(argv[2], &numVerticesB, &b);
    if (!success)
    {
        printf("Error reading file %s\n", argv[2]);
        return 1;
    }
    else
    {
        // // print the vector b
        // printf("b:\n");
        // for (int i = 0; i < numVertices; i++)
        // {
        //     printf("%3.20lf ", b[i]);
        // }
        // printf("\n");
    }

    // allocate memory for the vector x
    double *x = (double *)malloc(numVertices * sizeof(double));
    double *x_new = (double *)malloc(numVertices * sizeof(double));

    // Xold = X // Initial solution vector (n x 1 vector filled with 0 values)
    for (int i = 0; i < numVertices; i++)
    {
        x[i] = 0;
    }

    // apply the Gauss-Jacobi algorithm
    double error = INT_MAX;
    int iteration = 0;
    while (error > EPSILON)
    {
        for (int i = 0; i < numVertices; i++)
        {
            x_new[i] = 0;
        }
        // multiply A with x and store the result in x_new and add b
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = offsets[i]; j < offsets[i + 1]; j++)
            {
                x_new[i] += weights[j] * x[columns[j]];
            }
            x_new[i] += b[i];
        }

        error = 0;
        for (int i = 0; i < numVertices; i++)
        {
            error += (x_new[i] - x[i]) * (x_new[i] - x[i]);
        }
        error = sqrtf(error);

        // copy x_new to x
        for (int i = 0; i < numVertices; i++)
        {
            x[i] = x_new[i];
        }

        if (iteration % 100 == 0)
        {
            printf("Iteration %d: error = %3.20lf\n", iteration, error);
        }

        iteration++;
    }

    // write the vector x to the output file
    printf("Writing the vector x to the output file %s...\n", argv[3]);
    success = write_vector(argv[3], numVertices, x);
    if (!success)
    {
        printf("Error writing file %s\n", argv[3]);
        return 1;
    }

    // end the timer
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    // print the elapsed with nanoseconds precision
    printf("Elapsed time: %.9lf seconds\n", elapsed);
}
