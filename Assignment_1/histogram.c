#include <stdio.h>
#include <stdlib.h>

int main() {
    int histogram[256] = { 0 };

    int number;
    while(scanf("%d", &number) == 1) {
        histogram[number]++;
    }

    for (int i = 0 ; i < 256; i++) {
        printf("%d\n", histogram[i]);
    }

    return 0;
}