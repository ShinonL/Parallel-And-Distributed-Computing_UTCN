#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int noElements = atoi(argv[1]);

    int* numbers = (int*)malloc(sizeof(int) * 100);
    if (rank == 0) {
        for (int i = 0; i < 100; i++) {
            numbers[i] = rand();
            printf("numbers[%d] = %d\n", i, numbers[i]);
        }
    }

    int* numbersSubset = (int*)malloc(sizeof(int) * noElements);
    MPI_Scatter(numbers, noElements, MPI_INT, numbersSubset, noElements, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < noElements; i++) {
        numbersSubset[i]++;
    }

    int* receivedNumbers = NULL;
    if (rank == 0) {
        receivedNumbers = (int*)malloc(sizeof(int) * 100);
    }

    MPI_Gather(numbersSubset, noElements, MPI_INT, receivedNumbers, noElements, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < 100; i++) {
            printf("receivedNumbers[%d] = %d\n", i, receivedNumbers[i]);
        }
    }

    MPI_Finalize();
    return 0;
}