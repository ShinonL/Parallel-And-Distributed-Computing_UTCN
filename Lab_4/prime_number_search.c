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

int main() {
    int A, B;    
    scanf("%d", &A);
    scanf("%d", &B);

    if (A > B) {
        printf("Expected %d to be less than %d.\n", A, B);
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
