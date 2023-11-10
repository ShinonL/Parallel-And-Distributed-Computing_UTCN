#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
#include <omp.h>

#define TOL 0.01

using namespace std;

float** generate_random_matrix(int size) {
    time_t t;
    srand((unsigned)time(&t));

    float** mat = (float**)malloc(size * sizeof(float*));

    for (int i = 0; i < size; i++) {
        mat[i] = (float*)malloc(size * sizeof(float));

        for (int j = 0; j < size; j++) {
            mat[i][j] = rand() % 1000;
        }
    }

    return mat;
}

void print_matrix(float** mat, int size, const char* fileName) {
    ofstream write;
    write.open(fileName, fstream::app);
        
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            write << mat[i][j] << "\t";
        }
        write << "\n";
    }
    write.close();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << ".\\shared_mem <number_n> <number_of_threads>\n";
        return -1;
    }

    int n = atoi(argv[1]);
    int noThreads = atoi(argv[2]);
    omp_set_num_threads(noThreads);

    float **mat = generate_random_matrix(n + 2);
    print_matrix(mat, n + 2, "initial.txt");

    double startTime = omp_get_wtime();

    float diff;
    #pragma omp parallel
    {
        int pid = omp_get_thread_num();

        int min = 1 + (pid * n / noThreads);
        int max = min + n / noThreads - 1;

        bool done = false;
        while(!done) {
            diff = 0;
            float tempDiff = 0;

            for (int i = min; i <= max; i++) {
                for (int j = 1; j <= n; j++) {
                    float temp = mat[i][j];
                    mat[i][j] = 0.2 * (mat[i][j] + mat[i][j - 1] + mat[i - 1][j] + mat[i][j + 1] + mat[i + 1][j]);
                    diff += abs(mat[i][j] - temp);
                }
            }

            #pragma omp critical
            {
                diff += tempDiff;
            }
            #pragma omp barrier

            #pragma omp critical
            {
                if (diff / (n * n) < TOL) {
                    done = true;
                }
            }
            #pragma omp barrier
        }
    }
    double endTime = omp_get_wtime();
    
    print_matrix(mat, n + 2, "result.txt");

    double execTime = ((double)(endTime - startTime) * 1.0 / CLOCKS_PER_SEC) * 1000;
    printf("Execution time: %lf ms\n", execTime);

    return 0;
}