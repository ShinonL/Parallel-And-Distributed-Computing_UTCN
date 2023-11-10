#include <stdlib.h>
#include <stdio.h>

void read_from_file(int i, FILE *writeFile) {
	char file_path[100];
	sprintf(file_path, "process_%d.in.txt", i);
	FILE *readFile = fopen(file_path, "r+");

    int readResult = 0, number;
	while ((readResult = fscanf(readFile, "%d", &number)) != EOF) {
		fprintf(writeFile, "%d ", number);
	}

	fclose(readFile);
}

int main() {
	FILE *fd = fopen("gather_results.out.txt", "w+");
	
	for (int i = 0; i < 4; i++) {
		read_from_file(i, fd);
	}

	fclose(fd);

	return 0;
}