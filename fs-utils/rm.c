#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <unistd.h>

void RemoveFile(char* pathname) {
  int res = unlink(pathname);
  if (res != 0) {
    printf("ERROR DELETING FILE");
    exit(1);
  }
}

void RmDir(char* pathname) {
  DIR* dr = opendir(pathname);
  if (dr == NULL) {
    printf("ERROR OPENING DIR");
    exit(1);
  }
  struct dirent* d_entry;
  char path[1024];
  while ((d_entry = readdir(dr)) != NULL) {
    if (strcmp(d_entry->d_name, ".") == 0 ||
        strcmp(d_entry->d_name, "..") == 0) {
      continue;
    }
    snprintf(path, sizeof(path), "%s/%s", pathname, d_entry->d_name);
    struct stat tmp;
    int res_tmp = stat(path, &tmp);
    if (res_tmp != 0) {
      printf("ERROR\n");
      exit(1);
    }
    if (S_ISDIR(tmp.st_mode)) {
      RmDir(path);
    } else {
      RemoveFile(path);
    }
  }
  closedir(dr);
  int f_res = rmdir(pathname);
  if (f_res != 0) {
    printf("ERROR\n");
    exit(1);
  }
}

int main(int argc, char* argv[]) {
	printf("BEING TESTED");
  int flag = 0;
  int opt;
  while ((opt = getopt(argc, argv, "r")) != -1) {
    switch (opt) {
      case 'r':
        flag = 1;
        break;
      default:
        printf("ERROR");
        exit(1);
    }
  }
  for (int i = optind; i < argc; i++) {
    struct stat statbuf;
    if (lstat(argv[i], &statbuf) != 0) {
      perror(argv[i]);
      continue;
    }

    if (S_ISDIR(statbuf.st_mode)) {
      if (flag) {
        RmDir(argv[i]);
      } else {
        printf("ERROR");
        exit(1);
      }
    } else {
      RemoveFile(argv[i]);
    }
  }
  return 0;
}

