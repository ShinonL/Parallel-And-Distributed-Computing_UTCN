#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>

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

void solve(float** mat, int n) {
    bool done = false;
    while(!done) {
        float diff = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                float temp = mat[i][j];
                mat[i][j] = 0.2 * (mat[i][j] + mat[i][j - 1] + mat[i - 1][j] + mat[i][j + 1] + mat[i + 1][j]);
                diff += abs(mat[i][j] - temp);
            }
        }

        if (diff / (n * n) < TOL) {
            done = true;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << ".\\sequential <number_n>";
        return -1;
    }

    int n = atoi(argv[1]);

    float **A = generate_random_matrix(n + 2);
    print_matrix(A, n + 2, "initial.txt");

    double startTime = clock();
    solve(A, n);
    double endTime = clock();
    
    print_matrix(A, n + 2, "result.txt");

    double execTime = ((double)(endTime - startTime)  * 1.0 / CLOCKS_PER_SEC) * 1000;
    printf("Execution time: %lf ms\n", execTime);

    return 0;
}