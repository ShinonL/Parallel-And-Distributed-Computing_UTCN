#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int INTEGER_TO_SEND = 109;
        MPI_Send(&INTEGER_TO_SEND, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        printf("Sent Integer %d to process %d\n", INTEGER_TO_SEND, 1);

        int INCREASED_INTEGER;
        MPI_Recv(&INCREASED_INTEGER, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank 0 received integer %d from Rank 1\n", INCREASED_INTEGER);
    } 
    else if (rank == 1) {
        int RECEIVED_INTEGER;
        MPI_Recv(&RECEIVED_INTEGER, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Rank 1 received integer %d from Rank 0\n", RECEIVED_INTEGER);

        RECEIVED_INTEGER++;
        MPI_Send(&RECEIVED_INTEGER, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        printf("Sent Integer %d to process %d\n", RECEIVED_INTEGER, 0);
    }

    MPI_Finalize();
    return 0;
}