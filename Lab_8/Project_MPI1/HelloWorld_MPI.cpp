// HelloWorld_MPI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// wait here for all
	MPI_Barrier(MPI_COMM_WORLD);

	int M = 10;
	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

	const int k = 5;
	int local[k];
	if (rank != 0) {
		for (int i = 0; i < k; i++) {
			local[i] = i + M * rank;
		}
	}

	int result[k] = { 0 };
	MPI_Reduce(local, result, k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		int sum = 0;
		for (int i = 0; i < k; i++)
		{
			sum += result[i];
		}

		printf("Sum: %d", sum);
	}

    MPI_Finalize();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
