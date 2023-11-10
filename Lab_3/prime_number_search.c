#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool isPrime(int number) {
    for (int divisor = 3; divisor <= floor(sqrt(number)); divisor += 2) {
        if (number % divisor == 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Expected 2 parameters: A B.\n");
        exit(1);
    }

    int A = atoi(argv[1]);
    int B = atoi(argv[2]);

    if (A >= B) {
        printf("Expected A to be less than B.\n");
        exit(2);
    }

    printf("Prime numbers between %d and %d: \n", A, B);
    if (A < 3) {
        printf("2 ");
        A = 3;
    } 

    for (int X = A + 1 - (A % 2); X <= B; X += 2) {
        if (isPrime(X)) {
            printf("%d ", X);
        }
    }

    printf("\n");
    return 0;
}
