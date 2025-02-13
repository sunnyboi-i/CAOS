#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pcre.h>

void find_reg(char* pathname, pcre* reg_ex, pcre_extra* reg_ex_extra) {

    int pre_count;
    pcre_fullinfo(reg_ex, reg_ex_extra, PCRE_INFO_CAPTURECOUNT, &pre_count);
    int ovecsize = 2 * (pre_count + 1);
    int ovector[ovecsize];
    int fd = open(pathname, O_RDONLY);
    struct stat st;
    if (fstat(fd, &st) < 0) {
        printf("Error while getting file info: %s\n", pathname);
        return;
    }
    
    if (!S_ISREG(st.st_mode)) {
        close(fd);
        return;
    }
    
    void* region = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (region == MAP_FAILED) {
        printf("Error while mapping file: %s\n", pathname);
        close(fd);
        return;
    }

    char* begin = (char*)region;
    char* end = begin + st.st_size;
    int i = 1;
    for (char* ptr = begin; ptr < end; ++ptr) {
        if (*ptr == '\n') {
            size_t line_length = ptr - begin;
            char* buffer = (char*)malloc(line_length + 1);
            if (buffer == NULL) {
                printf("Failed to allocate memory");
                exit(0);
            }

            memcpy(buffer, begin, line_length);
            buffer[line_length] = '\0';

            int rc = pcre_exec(reg_ex, reg_ex_extra, buffer, strlen(buffer), 0, 0, ovector, ovecsize);

            if (rc >= 0) {
                printf("%s:%d: %s\n", pathname, i, buffer);     
            }
            free(buffer);
            i += 1;
            begin = ptr + 1;
        }
    }
    munmap(region, st.st_size);
    close(fd);
}

void scan_dir(const char* dirpath, pcre* reg_ex, pcre_extra* reg_ex_extra) {
  
    DIR* dr = opendir(dirpath);
    if (dr == NULL) {
        printf("Error opening directory: %s\n", dirpath);
        exit(1);
    }

    struct dirent* d_entry = readdir(dr);


    while (d_entry != NULL) {
        if (strcmp(d_entry->d_name, ".") == 0 || strcmp(d_entry->d_name, "..") == 0) {
            d_entry = readdir(dr);
            continue;
        }

        char fullpath[PATH_MAX];
        snprintf(fullpath, PATH_MAX, "%s/%s", dirpath, d_entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) < 0) {
            printf("Error accessing file: %s\n", fullpath);
            d_entry = readdir(dr);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            scan_dir(fullpath, reg_ex, reg_ex_extra);
        } else if (S_ISREG(st.st_mode)) {
            find_reg(fullpath, reg_ex, reg_ex_extra);
        }
        d_entry = readdir(dr);
    }

    closedir(dr);
}



int main(int argc, char* argv[]) {
    if (argc != 3) {
        exit(1);
    }

    const char* regex = argv[1];
    const char* dir = argv[2];

    const char* err;
    int err_off;
    pcre* reg_ex = pcre_compile(regex, 0, &err, &err_off, NULL); 
    if (reg_ex == NULL) {
        fprintf(stderr, "%s\n", err);
        exit(1);
    }

    pcre_extra* reg_ex_extra = pcre_study(reg_ex, 0, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
        pcre_free(reg_ex);
        exit(1);
    }

    scan_dir(dir, reg_ex, reg_ex_extra);

    pcre_free_study(reg_ex_extra);
    pcre_free(reg_ex);
    return 0;
}
