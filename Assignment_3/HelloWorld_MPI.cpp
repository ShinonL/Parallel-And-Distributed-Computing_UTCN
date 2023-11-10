// HelloWorld_MPI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
using namespace std;

int* readMatrix(const char* filePath, int* matSize) {
	ifstream read(filePath);

	int size;
	read >> size;

	*matSize = size;
	int* mat = (int*)malloc(size * size * sizeof(int));

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			int aux;
			read >> aux;
			mat[i * size + j] = aux;
		}
	}

	read.close();

	return mat;
}

void multiplyMatrix(int size, int* a, int* b, int* res) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				res[i * size + j] += a[i * size + k] * b[k * size + j];
			}
		}
	}
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

	int noProcessesSqrt = (int)sqrt(worldSize);
	if (noProcessesSqrt - sqrt(worldSize) != 0) {
		if (worldRank == 0) {
			cout << "noProcessesSqrt - sqrt(worldSize) = !0\n";
		}

		MPI_Finalize();
		return -1;
	}

	int* matrixA = NULL;
	int* matrixB = NULL;
	int* result = NULL;
	int matSize = 0;
	if (worldRank == 0) {
		matrixA = readMatrix(argv[1], &matSize);
		matrixB = readMatrix(argv[2], &matSize);
		result = (int*)calloc(matSize * matSize, sizeof(int));;

		if (matSize % noProcessesSqrt != 0) {
			if (worldRank == 0) {
				cout << "matSize % noProcessesSqrt != 0\n";
			}

			MPI_Finalize();
			return -2;
		}
	}
	MPI_Bcast(&matSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Datatype type, subarrayType;
	int sizes[] = { matSize, matSize };
	int subSize = int(matSize / sqrt(worldSize));
	int subSizes[] = { subSize, subSize };
	int starts[] = { 0, 0 };
	MPI_Type_create_subarray(2, sizes, subSizes, starts, MPI_ORDER_C, MPI_INT, &type);
	MPI_Type_create_resized(type, 0, subSize * sizeof(int), &subarrayType);
	MPI_Type_commit(&subarrayType);

	int* ptrA = NULL;
	int* ptrB = NULL;
	int* ptrResult = NULL;
	if (worldRank == 0) {
		ptrA = &(matrixA[0]);
		ptrB = &(matrixB[0]);
		ptrResult = &(result[0]);
	}

	int* counts = (int*)malloc(sizeof(int) * worldSize);
	int* displacements = (int*)malloc(sizeof(int) * worldSize);
	if (worldRank == 0) {
		for (int i = 0; i < worldSize; i++) {
			counts[i] = 1;
		}

		int disp = 0;
		for (int i = 0; i < noProcessesSqrt; i++) {
			for (int j = 0; j < noProcessesSqrt; j++) {
				displacements[i * noProcessesSqrt + j] = disp;
				disp++;
			}
			disp += (subSize - 1) * noProcessesSqrt;
		}
	}

	int* subA = (int*)calloc(subSize * subSize, sizeof(int));;
	int* subB = (int*)calloc(subSize * subSize, sizeof(int));
	int* subResult = (int*)calloc(subSize * subSize, sizeof(int));
	MPI_Scatterv(ptrA, counts, displacements, subarrayType, &(subA[0]), int(matSize * matSize / (worldSize)), MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(ptrB, counts, displacements, subarrayType, &(subB[0]), int(matSize* matSize / (worldSize)), MPI_INT, 0, MPI_COMM_WORLD);

	int periods[] = { 1, 1 };
	int dims[] = { noProcessesSqrt, noProcessesSqrt };
	MPI_Comm commCart;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &commCart);
	int coord[2];
	MPI_Cart_coords(commCart, worldRank, 2, coord);

	// 1st movement
	int left, right, up, down;
	MPI_Cart_shift(commCart, 1, coord[0], &left, &right);
	MPI_Sendrecv_replace(&(subA[0]), subSize * subSize, MPI_INT, left, 1, right, 1, commCart, MPI_STATUS_IGNORE);

	MPI_Cart_shift(commCart, 0, coord[1], &up, &down);
	MPI_Sendrecv_replace(&(subB[0]), subSize * subSize, MPI_INT, up, 1, down, 1, commCart, MPI_STATUS_IGNORE);

	for (int i = 0; i < noProcessesSqrt; i++) {
		multiplyMatrix(subSize, subA, subB, subResult);

		// next movement
		MPI_Cart_shift(commCart, 1, 1, &left, &right);
		MPI_Cart_shift(commCart, 0, 1, &up, &down);

		MPI_Sendrecv_replace(&(subA[0]), subSize * subSize, MPI_INT, left, 1, right, 1, commCart, MPI_STATUS_IGNORE);
		MPI_Sendrecv_replace(&(subB[0]), subSize * subSize, MPI_INT, up, 1, down, 1, commCart, MPI_STATUS_IGNORE);
	}

	MPI_Gatherv(&(subResult[0]), matSize * matSize / worldSize, MPI_INT, ptrResult, counts, displacements, subarrayType, 0, MPI_COMM_WORLD);

	if (worldRank == 0) {
		for (int i = 0; i < matSize; i++) {
			for (int j = 0; j < matSize; j++) {
				cout << result[i * matSize + j] << "\t";
			}
			cout << "\n";
		}
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
