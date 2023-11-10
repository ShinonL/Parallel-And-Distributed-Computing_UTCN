#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char** argv) {
	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	int no_processes = atoi(argv[3]);

	for (int i = 0; i < no_processes; i++) {
        int start_i = start + (end - start) * i / no_processes;
        int end_i = start + (end - start) * (i + 1) / no_processes - 1;

        char file_path[100];
        sprintf(file_path, "process_%d.in.txt", i);
        FILE *fd = fopen(file_path, "w+");
        fprintf(fd, "%d %d", start_i, end_i);

        fclose(fd);
	}
	return 0;
}