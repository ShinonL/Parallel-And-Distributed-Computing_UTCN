#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h" 
#include <iostream>

#define TOL 0.01

using namespace std;

float** generate_random_matrix(int n, int m) {   
    time_t t;
    srand((unsigned)time(&t));

    float* tempMat = (float*)malloc(n * m * sizeof(float*));

    float** mat = (float**)malloc(n * sizeof(float*));
    for (int i = 0 ; i < n; i ++) {
        mat[i] = &(tempMat[i * n]);
        for(int j = 0 ; j < m ; j++) {
            mat[i][j] = rand() % 1000;
        }
    }

    return mat;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {   
        cout << "mpiexec -n <noProcesses> .\\msg_pass.exe <grid_size>\n";
        MPI_Finalize();
        return 1;
    }

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int n = atoi(argv[1]);
    int blockSize = n / worldSize;
    if (n % worldSize != 0 ) {
        blockSize++;
    }

    float** mat = generate_random_matrix(blockSize + 2, n + 2);

    double start;
    start = MPI_Wtime();

    int i, j, tempdiff = 0, diff = 0;
    bool done = false;
    float temp;
    while (!done) {
        if (rank != 0) {
            MPI_Send(&mat[1][0], n, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD);
        }

        if (rank != worldSize - 1) {
            MPI_Send(&mat[blockSize][0], n, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD);
        }

        if (rank != 0) {
            MPI_Recv(&mat[0][0], n, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank != worldSize - 1) {
            MPI_Recv(&mat[blockSize + 1][0], n, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        diff = 0;
        tempdiff = 0;
        for (i = 1 ; i <= blockSize; i++) {
            for (j = 1; j <= n ; j++) {
                temp = mat[i][j];
                mat[i][j] = 0.2 * (mat[i][j] + mat[i][j - 1] + mat[i - 1][j] + mat[i][j + 1] + mat[i + 1][j]);
                diff += abs(mat[i][j] - temp);
            }
        }
        
        MPI_Allreduce(&diff, &tempdiff, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

        if (tempdiff / (n * n) < TOL) {
            done = true;
        }
    }

    double end;
    end = MPI_Wtime();
    
    if (rank == 0) {
       printf("Execution time: %lf ms\n", (end - start) * 1000);
    }

    MPI_Finalize();
    return 0;
}