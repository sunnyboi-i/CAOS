#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>

void Parse(char* pathname, mode_t md, int flag) {
  mode_t dflt = 0755;
  char* path[255];
  char tmp[255];
  int ln = strlen(pathname);
  int all = 0;
  int cnt = 0;
  int rel = 0;

  if (pathname[0] == '/') {
    path[all++] = strdup("/");
    rel += 1;
  } else if (pathname[0] == '.' && pathname[1] == '/') {
    rel += 1;
    path[all++] = strdup(".");
  } else if (pathname[0] == '.' && pathname[1] == '.' && pathname[2] == '/') {
    rel += 2;
    path[all++] = strdup("..");
  } else {
    path[all++] = strdup(".");
  }

  for (int i = rel; i <= ln; ++i) {
    if (pathname[i] == '/' || pathname[i] == '\0') {
      if (cnt > 0) {
        tmp[cnt] = '\0';
        path[all++] = strdup(tmp);
        cnt = 0;
      }
    } else {
      tmp[cnt++] = pathname[i];
    }
  }
	
  char* res = malloc(1024);  
  if (res == NULL) {
    exit(1);
  }

  strcpy(res, path[0]);
	for (int i = 0; i < all; ++i) {
	
	}
  for (int i = 1; i < all; ++i) {
    if (strcmp(res, ".") == 0 || strcmp(res, "..") == 0) {
      strcat(res, "/");
    }
    if (i + 1 < all) {
      strcat(res, path[i]);
      strcat(res, "/");
    } else {
      strcat(res, path[i]);
    }

		//char* tmp_tmp = malloc(strlen(res) + 1);
		//strcpy(tmp_tmp, res);
		
    DIR* tmp_o = opendir(res);
    if (tmp_o != NULL) {
			printf("Directory already exists: %s\n", res);
      closedir(tmp_o);
    } else {
			printf("Attempting to create directory: %s\n", res);
      if ((i + 1 == all && flag == 0) || (flag == 1 && i + 1 <= all)) {
        int m = mkdir(res, dflt);
        if (m != 0) {
					printf("MKDIR FAILED\n");
          break;
          exit(1);
        }
				printf("DIRECTORY %s, created\n", res);
        if (i + 1 == all) {
          int cm = chmod(res, md);
          if (cm != 0) {
            perror("chmod failed\n");
            exit(1);
          }				
				}

      } else  if (i + 1 < all && flag == 0) {
        printf("NO FLAG -p PROVIDED\n");
        exit(1);
      }
    }
  }

  printf("Final path: %s\n", res);
  for (int i = 0; i < all; ++i) {
  	free(path[i]);
 	}
  free(res);
}

int main(int argc, char* argv[]) {
  //printf("Mkdir is being test\n");
	int flag = 0;
  mode_t dflt = 0755;
  int c;
  struct option long_options[] = {
    {"mode", required_argument, NULL, 'm'},
    {"p", no_argument, &flag, 1},
    {0, 0, 0, 0}
  };

  while ((c = getopt_long(argc, argv, "p", long_options, NULL)) != -1) {
    switch (c) {
      case 'm':
        dflt = (mode_t)strtol(optarg, NULL, 8);
        break;
      case 'p':
        flag = 1;
        break;
      case 0:
        break;
      default:
        exit(1);
    }
  }

  if (optind >= argc) {
    exit(1);
  }

  for (int i = optind; i < argc; ++i) {
		printf("CURRENT PATH IS %s\n\n", argv[i]);
    Parse(argv[i], dflt, flag);
  }
  return 0;
}


