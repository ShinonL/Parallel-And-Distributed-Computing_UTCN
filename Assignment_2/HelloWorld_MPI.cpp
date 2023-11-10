#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include <fstream>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int size = 0;
    int* numbers = NULL;
    if (rank == 0) {
        std::ifstream read;
        read.open(argv[1]);

        if (read.is_open()){
            int i;
            while (read >> i) {
                size++;
            }
            numbers = (int*)malloc(sizeof(int) * size);

            read.clear();
            read.seekg(0);

            for (int i = 0; i < size; i++) {
                read >> numbers[i];
            }
            read.close();
        }
    }

    int subsetSize = size / world_size;
    int* numbersSubset = (int*)malloc(sizeof(int) * size);
    MPI_Scatter(numbers, size, MPI_INT, numbersSubset, size, MPI_INT, 0, MPI_COMM_WORLD);

    int histogram[256] = { 0 };
    for (int i = 0; i < size; i++) {
        histogram[numbersSubset[i]]++;
    }

    int* receivedHistogram = NULL;
    if (rank == 0) {
        receivedHistogram = (int*)malloc(sizeof(int) * 256);
    }

    MPI_Reduce(histogram, receivedHistogram, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::ofstream write;
        write.open("../../result.txt", std::fstream::app);
        for (int i = 0; i < 256; i++) {
            write << "histogram[" << i << "] = " << receivedHistogram[i] << "\n";
        }

        write.close();
    }

    MPI_Finalize();
    return 0;
}