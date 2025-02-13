#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

enum {
    MAX_ARGS_COUNT = 256,
    MAX_CHAIN_LINKS_COUNT = 256
};

typedef struct {
    char* command;
    uint64_t argc;
    char* argv[MAX_ARGS_COUNT];
} chain_link_t;

typedef struct {
    uint64_t chain_links_count;
    chain_link_t chain_links[MAX_CHAIN_LINKS_COUNT];
} chain_t;

char* RemoveSpace(char* str) {
    while (isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == '\0') {
        return str;
    }
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return str;
}

void ParseArgs(char* args, char** arg_res) {
    uint64_t sz = strlen(args);
    uint64_t start = 0;
    uint64_t counter = 0;

    for (uint64_t i = 0; i <= sz; ++i) {
        if (args[i] == '|' || args[i] == '\0') {
            uint64_t len = i - start;
            if (len > 0) {
                char* cmd_arg = (char*)malloc(len + 1);
                strncpy(cmd_arg, args + start, len);
                cmd_arg[len] = '\0';

                char* trm_cmd = RemoveSpace(cmd_arg);
                uint64_t trim_len = strlen(trm_cmd);

                arg_res[counter] = (char*)malloc(trim_len + 1);
                strcpy(arg_res[counter], trm_cmd);

                free(cmd_arg);
                counter++;
            }
            start = i + 1;
        }
    }
    arg_res[counter] = NULL;
}



void create_chain(char* command, chain_t* chain) {
    char* parsed_args[MAX_CHAIN_LINKS_COUNT];
    ParseArgs(command, parsed_args);

    chain->chain_links_count = 0;

    for (uint64_t i = 0; parsed_args[i] != NULL; i++) {
        uint64_t cmd_len = strlen(parsed_args[i]);
        chain->chain_links[i].command = (char*)malloc(cmd_len + 1);
        strcpy(chain->chain_links[i].command, parsed_args[i]);
        chain->chain_links[i].argc = 0;

        char* curr = parsed_args[i];
        uint64_t arg_id = 0;

        while (*curr != '\0') {

            while (isspace((unsigned char)*curr)) {
                curr++;
            }

            if (*curr == '\0') {
                break;
            }

            char* start = curr;
            while (*curr != '\0' && !isspace((unsigned char)*curr)) {
                curr++;
            }

            uint64_t arg_len = curr - start;

            chain->chain_links[i].argv[arg_id] = (char*)malloc(arg_len + 1);
            strncpy(chain->chain_links[i].argv[arg_id], start, arg_len);
            chain->chain_links[i].argv[arg_id][arg_len] = '\0';

            arg_id++;
        }

        chain->chain_links[i].argv[arg_id] = NULL;
        chain->chain_links[i].argc = arg_id;
        chain->chain_links_count++;
    }
}


void run_chain(chain_t* chain) {
    int pipefd[2];
    int p_fd = -1;

    for (uint64_t i = 0; i < chain->chain_links_count; ++i) {
        if (i < chain->chain_links_count - 1) {
            pipe(pipefd);
        }
        pid_t pid = fork();
        if (pid == 0) {
            if (p_fd != -1) {
                dup2(p_fd, STDIN_FILENO);
                close(p_fd);
            }
            if (i < chain->chain_links_count - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            execvp(chain->chain_links[i].argv[0], chain->chain_links[i].argv);
            exit(1);
        } else {
            if (p_fd != -1) {
                close(p_fd);
            }
            if (i < chain->chain_links_count - 1) {
                close(pipefd[1]);
            }
            p_fd = pipefd[0];
        }
    }
    for (uint64_t i = 0; i < chain->chain_links_count; ++i) {
        wait(NULL);
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "%s\n", argv[0]);
        exit(1);
    }

    chain_t chain;
    create_chain(argv[1], &chain);
    run_chain(&chain);
    return 0;
}
