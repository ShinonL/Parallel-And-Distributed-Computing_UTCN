#include <stdio.h>
#include <stdlib.h>

int main() {
    int histogram[256] = { 0 };
    
    for (int i = 0 ; i < 5; i++) {
        char filePath[100];
        sprintf(filePath, "histogram_%d.out.txt", i);
        FILE* inputFile = fopen(filePath, "r+");

        for (int i = 0 ; i < 256; i++) {
            int number;
            fscanf(inputFile, "%d", &number);
            histogram[i] += number;
        }
        fclose(inputFile);
    }

    for(int i = 0 ; i < 256; i++) {
        printf("h[%d] = %d\n", i, histogram[i]);
    }

    return 0;
}