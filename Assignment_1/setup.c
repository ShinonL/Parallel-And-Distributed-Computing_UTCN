#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int noProcesses = atoi(argv[1]);

    FILE* inputFile = fopen("test_file.txt", "r+");

    int number, count = 0;
    while (fscanf(inputFile, "%d", &number) == 1) {
        count++;
    }

    fseek(inputFile, 0, SEEK_SET);

    int splitSize = count / noProcesses;
    if (count % noProcesses != 0) {
        splitSize++;
    }

    char filePath[100];
    for (int currentProcess = 0; currentProcess < noProcesses; currentProcess++) {
        sprintf(filePath, "./process_%d.in.txt", currentProcess);
        FILE* writeFile = fopen(filePath, "w+");

        for (int i = 0; i < splitSize; i++) {
            if (fscanf(inputFile, "%d", &number) != 1) {
                fclose(writeFile);
                fclose(inputFile);

                return 0;
            }
            fprintf(writeFile, "%d ", number);
        }
        
        fclose(writeFile);
    }

    fclose(inputFile);
    return 0;
}