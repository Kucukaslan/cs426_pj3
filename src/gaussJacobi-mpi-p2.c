#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "util.h"
#include <stdbool.h>
#include <math.h>

#include <unistd.h> // for getpid()

// mpi
#include <mpi.h>

int min_int(int a, int b) { return a < b ? a : b; };
int max_int(int a, int b) { return a > b ? a : b; };

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

    // printf("Will initialize MPI\n");
    // initialize MPI and print errors if any

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get current process id
    MPI_Comm_size(MPI_COMM_WORLD, &size); // get number of processes

    // printf("Initialized MPI\n");

    // start the timer
    double start = MPI_Wtime();

    int numVertices, numEdges, *offsets, *columns;
    double *weights;

    int numVerticesB;
    double *b;

    // my pid
    printf("%d: My pid is %d\n", rank, getpid());
    // printf("%d: Number of processes: %d\n", rank, size);

    if (rank == 0)
    {
        printf("Number of processes: %d\n", size);
        printf("%d: time is %lf\n", rank, MPI_Wtime() - start);

        // offsets is the rows
        // why did they named it like that? To confuse us?

        // read the input file
        int success = read_matrix(argv[1], &numVertices, &numEdges, &offsets, &columns, &weights);
        if (!success)
        {
            printf("Error reading file %s\n", argv[1]);
            return 1;
        }
        else
        {
            // // print the matrix A
            // printf("With csr notation:\n");
            // for (int i = 0; i < numVertices; i++)
            // {
            //     printf("%10d ", i);
            // }
            // printf("\n");
            // printf("offsets:\n");
            // for (int i = 0; i < numVertices + 1; i++)
            // {
            //     printf("%10d ", offsets[i]);
            // }
            // printf("\n");

            // printf("columns:\n");
            // for (int i = 0; i < numEdges; i++)
            // {
            //     printf("%10d ", columns[i]);
            // }
            // printf("\n");

            // printf("weights:\n");
            // for (int i = 0; i < numEdges; i++)
            // {
            //     printf("%3.20lf ", weights[i]);
            // }
            // printf("\n");
        }

        // print the matrix A'
        // printf("A':\n");
        // for (int i = 0; i < numVertices; i++)
        // {
        //     for (int j = 0; j < numVertices; j++)
        //     {
        //         bool found = false;
        //         for (int k = offsets[i]; k < offsets[i + 1]; k++)
        //         {
        //             if (columns[k] == j)
        //             {
        //                 printf("%3.20lf ", weights[k]);
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
        // // now print it to a file A.txt
        // FILE *fp;
        // fp = fopen("A_csc.txt", "w");

        // for (int i = 0; i < numVertices; i++)
        // {
        //     for (int j = 0; j < numVertices; j++)
        //     {
        //         bool found = false;
        //         for (int k = offsets[i]; k < offsets[i + 1]; k++)
        //         {
        //             if (columns[k] == j)
        //             {
        //                 fprintf(fp, "%30.20lf ", weights[k]);
        //                 found = true;
        //                 break;
        //             }
        //         }
        //         if (!found)
        //         {
        //             fprintf(fp, "%30d ", 0);
        //         }
        //     }
        //     fprintf(fp, "\n");
        // }

        // read the vector b from the input file
        printf("Reading the vector b...\n");
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
    }
    // end of rank 0

    // broadcast the number of vertices, number of edges,
    // printf("%d: Broadcasting numVertices and numEdges...\n", rank);
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numEdges, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numVerticesB, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // printf("%d: numVertices: %d, numEdges: %d, numVerticesB: %d\n", rank, numVertices, numEdges, numVerticesB);
    // MPI_Barrier(MPI_COMM_WORLD);

    // prepare displs and sendcounts for scatterv of weights and columns
    int *sendcounts = (int *)malloc(size * sizeof(int));
    int *displs = (int *)malloc(size * sizeof(int));
    int div = numVertices / size;
    int remainder = numVertices % size;
    int index = 0;
    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            // we want to find how many edges each process will have
            sendcounts[i] = offsets[min_int(index + div, numVertices)] - offsets[index];
            displs[i] = offsets[index];
            index = index + div;
            // printf("%d: sendcount[%d]= %d, displs[%d] = %d\n", rank, rank, sendcounts[rank], rank, displs[rank]);
            // printf("%d: sendcount[%d], displs[%d] = %d, %d\n", rank, i, i, sendcounts[i], displs[i]);
        }
    }
    // broadcast the sendcounts and displs
    MPI_Bcast(sendcounts, size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displs, size, MPI_INT, 0, MPI_COMM_WORLD);

    // my sendcount and displs for weights and columns
    // printf("%d: sendcount[%d]= %d, displs[%d] = %d\n", rank, rank, sendcounts[rank], rank, displs[rank]);

    // printf("%d: sendcount[%d], displs[%d] = %d, %d\n", rank, rank, rank, sendcounts[rank], displs[rank]);
    // printf("\n");

    // MPI_Barrier(MPI_COMM_WORLD);

    // scatter the  columns and weights
    int *columns_local = (int *)malloc(sendcounts[rank] * sizeof(int));
    double *weights_local = (double *)malloc(sendcounts[rank] * sizeof(double));
    MPI_Scatterv(columns, sendcounts, displs, MPI_INT, columns_local, sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(weights, sendcounts, displs, MPI_DOUBLE, weights_local, sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // printf("%d: I received %d columns and weights starting from %d\n", rank, sendcounts[rank], displs[rank]);
    // if (sendcounts[rank] > 0)
    // {
    //     printf("%d: columns_local[0] = %d, weights_local[0] = %3.20lf\n", rank, columns_local[0], weights_local[0]);
    // }

    // scatter the offsets each process will have numVertices/size +1 elements starting from displs[rank]
    int *offset_sendcounts = (int *)malloc(size * sizeof(int));
    int *offset_displs = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        offset_sendcounts[i] = div + 1;
        offset_displs[i] = div * i;
    }
    int *offsets_local = (int *)malloc(offset_sendcounts[rank] * sizeof(int));
    MPI_Scatterv(offsets, offset_sendcounts, offset_displs, MPI_INT, offsets_local, offset_sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    // printf("%d: I received %d offsets starting from %d\n", rank, offset_sendcounts[rank], offset_displs[rank]);
    // if (offset_sendcounts[rank] > 0)
    // {
    //     // printf("%d: offsets_local[0] = %d\n", rank, offsets_local[0]);
    // }

    // MPI_Barrier(MPI_COMM_WORLD);

    // scatter the vector b to all processes (each process will have numVertices/size elements)
    double *b_local = (double *)malloc(div * sizeof(double));
    // printf("%d: b_local has size %d, will scatter %d elements\n", rank, div, numVerticesB);
    // scatter the vector b to all processes(each process will have numVertices / size elements)
    int *b_sendcounts = (int *)malloc(size * sizeof(int));
    int *b_displs = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        b_sendcounts[i] = div;
        b_displs[i] = i * div;
    }
    MPI_Scatterv(b, b_sendcounts, b_displs, MPI_DOUBLE, b_local, div, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    printf("%d: I received %d elements of b starting from %d\n", rank, b_sendcounts[rank], b_displs[rank]);

    // allocate memory for the vector x
    double *x = (double *)malloc(numVertices * sizeof(double));
    double *x_new = (double *)malloc(numVertices * sizeof(double));

    // Xold = X // Initial solution vector (n x 1 vector filled with 0 values)
    for (int i = 0; i < numVertices; i++)
    {
        x[i] = 0;
    }

    // apply the Gauss-Jacobi algorithm
    printf("%d: Applying the Gauss-Jacobi algorithm for CSC...\n", rank);
    double error = INT_MAX;
    int iteration = 0;
    while (error > EPSILON)
    {
        for (int i = 0; i < numVertices; i++)
        {
            x_new[i] = 0;
        }

        // multiply A' with x and store the result in x_new

        int l, u;
        // for each column assigned to the process x_new[j+col_displs] +=
        // so weights[j - offsets_local[0]] * x[columns[j-offsets_local[0]]]
        int col_displs = div * rank; // TODO
        for (int i = 0; i < div; i++)
        {
            // offsets for the current column
            l = offsets_local[i];
            u = offsets_local[i + 1];
            // printf("%d:%d column %d has offsets %d, %d\n", rank, iteration, i, l, u);
            for (int j = l; j < u; j++)
            {
                // printf("%d:%d i=%d, j=%d, offsets_local[0]=%d, col_displs=%d, columns_local[-] = %d \n", rank, iteration, i, j, offsets_local[0], col_displs, columns_local[j - offsets_local[0]]);
                x_new[columns_local[j - offsets_local[0]]] += weights_local[j - offsets_local[0]] * x[i + col_displs];
                // printf("%d:%d: x_new[%d] = %3.20lf =(x_new+ %3.20lf * %3.20f\n", rank, iteration, columns_local[j - offsets_local[0]], x_new[columns_local[j - offsets_local[0]]], weights_local[j - offsets_local[0]], x[i + col_displs]);
            }
        }

        for (int i = 0; i < div; i++)
        {
            // printf("%d: x_new[%d] +=b_local[%d]= %3.20lf\n", rank, i + col_displs, i, b_local[i]);
            x_new[i + col_displs] += b_local[i];
        }

        // local x_new
        // for (int i = 0; i < numVertices; i++)
        // {
        //     printf("%d:%d x_new[%d] = %3.20lf\n", rank, iteration, i, x_new[i]);
        // }

        // make vector sum of x_new from all processes
        MPI_Allreduce(MPI_IN_PLACE, x_new, numVertices, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // at rank 0 compute the error and broadcast it to all processes
        if (rank == 0)
        {
            error = 0;
            for (int i = 0; i < numVertices; i++)
            {
                error += (x_new[i] - x[i]) * (x_new[i] - x[i]);
            }
            error = sqrt(error);
        }

        // broadcast the error
        MPI_Bcast(&error, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (iteration % 100 == 0)
        {
            if (rank == 0)
            {
                printf("%d:%d: total error = %3.20lf\n", rank, iteration, error);
            }
        }

        // broadcast the vector x_new to all processes's x
        MPI_Bcast(x_new, numVertices, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // copy x_new to x
        for (int i = 0; i < numVertices; i++)
        {
            x[i] = x_new[i];
        }

        // char *x_f = (char *)malloc(100);
        // sprintf(x_f, "./x%d_%d", rank, iteration);
        // // char *xn_f = (char *)malloc(100);
        // // sprintf(xn_f, "./xn%d_%d", rank, iteration);
        // write_vector(x_f, numVertices, x);
        // // write_vector(xn_f, numVertices, x_new);

        iteration++;

        // if (iteration > 9)
        // {
        //     break;
        //     // MPI_Finalize();
        //     // exit(0);
        // }
    }

    // write the vector x to the output file
    if (rank == 0)
    {
        printf("Writing the vector x to the output file %s...\n", argv[3]);
        int success = write_vector(argv[3], numVertices, x);
        if (!success)
        {
            printf("Error writing file %s\n", argv[3]);
            return 1;
        }

        // end the timer
        double end = MPI_Wtime();
        printf("Time elapsed: %lf seconds\n", end - start);
    }

    MPI_Finalize();
}
