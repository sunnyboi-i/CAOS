#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include "lca.h"

int GetPpid(int fd) {
    char buffer[PATH_MAX];
    char line[PATH_MAX];
    ssize_t bytesRead;
    size_t lineLength = 0;
    while ((bytesRead = read(fd, buffer, PATH_MAX)) > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            line[lineLength++] = buffer[i];

            if (buffer[i] == '\n') {
                line[lineLength] = '\0';
                if (strncmp(line, "PPid:", 5) == 0) {
                    return atoi(line + 6);
                }
                lineLength = 0;
            }
        }
    }
    return -1;
}


char* MakePath(pid_t x) {
    char* path = (char*)malloc(MAX_TREE_DEPTH);
    if (!path) {
        exit(EXIT_FAILURE);
    }
    snprintf(path, MAX_TREE_DEPTH, "/proc/%d/status", x);
    return path;
}

int* BuildParents(pid_t pid, int* counter) {
	int* anc = (int*)malloc(MAX_TREE_DEPTH);
	anc[*counter] = pid;

	char* curr_path;
	int fd;
	*counter += 1;
	
	
	while (pid > 0) {
		curr_path = MakePath(pid);
		fd = open(curr_path, O_RDONLY); 
		pid = GetPpid(fd);
		anc[(*counter)++] = pid;
		close(fd);
	}
	return anc;
}



pid_t find_lca(pid_t x, pid_t y) {
		if (x == y) {
			return x;
		}
		int x_c = 0; 
		int y_c = 0;
		int* parents_x = BuildParents(x, &x_c);
		int* parents_y = BuildParents(y, &y_c);

		x_c -= 1;
		y_c -= 1;
		
		int last_x;
		int last_y;

		while (x_c >= 0 && y_c >= 0) {
			last_x = parents_x[x_c];
			last_y = parents_y[y_c];
			if (last_x == last_y) {
				x_c -= 1;
				y_c -= 1;
			} else {
				return parents_x[x_c + 1];
			}
		}
		if (x_c >= 0 && y_c < 0) {
			return parents_y[y_c + 1];
		}
		if (x_c < 0 && y_c >= 0) {
			return parents_x[x_c + 1];
		}

    return -1;
}

