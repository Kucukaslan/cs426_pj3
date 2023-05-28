
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "util.h"
#include <stdbool.h>
#include <math.h>

// an example call is
// %my_program input_matrix_A input_matrix_b output_matrix_X
int main(int argc, char *argv[])
{
    // check if the number of arguments is correct
    if (argc != 4)
    {
        printf("Usage: %s <input_file_A> <input_file_b> <output_file>\n", argv[0]);
        return 1;
    }

    // offsets is the rows
    // edges is the columns
    // why did they named it like that? To confuse us?

    // read the input file
    int numVertices, numEdges, *offsets, *edges;
    float *weights;
    int success = read_matrix_csr(argv[1], &numVertices, &numEdges, &offsets, &edges, &weights);
    if (!success)
    {
        printf("Error reading file %s\n", argv[1]);
        return 1;
    }
    // else
    // {
    //     // print the matrix A
    //     printf("With csr notation:\n");
    //     for (int i = 0; i < numVertices; i++)
    //     {
    //         printf("%10d ", i);
    //     }
    //     printf("\n");

    //     for (int i = 0; i < numVertices + 1; i++)
    //     {
    //         printf("%10d ", offsets[i]);
    //     }
    //     printf("\n");

    //     for (int i = 0; i < numEdges; i++)
    //     {
    //         printf("%10d ", edges[i]);
    //     }
    //     printf("\n");

    //     for (int i = 0; i < numEdges; i++)
    //     {
    //         printf("%10f ", weights[i]);
    //     }
    //     printf("\n");
    // }

    // // print the matrix A'
    // printf("A':\n");
    // for (int i = 0; i < numVertices; i++)
    // {
    //     for (int j = 0; j < numVertices; j++)
    //     {
    //         bool found = false;
    //         for (int k = offsets[i]; k < offsets[i + 1]; k++)
    //         {
    //             if (edges[k] == j)
    //             {
    //                 printf("%10f ", weights[k]);
    //                 found = true;
    //                 break;
    //             }
    //         }
    //         if (!found)
    //         {
    //             printf("%10d ", 0);
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // read the vector b from the input file
    printf("Reading the vector b...\n");
    int numVerticesB;
    float *b;
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
        //     printf("%3f ", b[i]);
        // }
        // printf("\n");
    }

    // allocate memory for the vector x
    float *x = (float *)malloc(numVertices * sizeof(float));
    float *x_new = (float *)malloc(numVertices * sizeof(float));

    // Xold = X // Initial solution vector (n x 1 vector filled with 0 values)
    for (int i = 0; i < numVertices; i++)
    {
        x[i] = 0;
    }

    // apply the Gauss-Jacobi algorithm
    float error = INT_MAX;
    int iteration = 0;
    while (error > EPSILON_e5)
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
                x_new[i] += weights[j] * x[edges[j]];
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

        // printf("Iteration %d: error = %f\n", iteration, error);
        // printf("x:\n");
        // for (int i = 0; i < numVertices; i++)
        // {
        //     printf("%3f ", x[i]);
        // }
        // printf("\n------------------\n");
        iteration++;
    }

    // print the vector x
    printf("x:\n");
    for (int i = 0; i < numVertices; i++)
    {
        printf("%3f ", x[i]);
    }
    printf("\n");

    // write the vector x to the output file
    printf("Writing the vector x to the output file %s...\n", argv[3]);
    success = write_vector(argv[3], numVertices, x);
    if (!success)
    {
        printf("Error writing file %s\n", argv[3]);
        return 1;
    }
}
