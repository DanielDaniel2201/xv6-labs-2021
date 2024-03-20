#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define INPUT_FD 0

char buf[512]; // buffer for all arguments including effective arguments, blank spaces and newline symbol

int new_line_helper(char *base) {
    char *p = base;
    while (*p++ != '\n')
        ;
    return p - base;
}

int blank_space_helper(char* base) {
    char *p = base;
    while (*p != '\n' && *p != ' ') {
        p++;
    }
    p++;
    return p - base;
}


int main(int argc, char *argv[]) {
    char *new_args[MAXARG];
    char tmp;
    for (int i = 0; (read(INPUT_FD, &tmp, sizeof(char))) == sizeof(char); i++) {
        buf[i] = tmp;
    }
    char *ptr = buf;
    char tmp_line[512];

    while (ptr - buf != strlen(buf)) {
        int line_len = new_line_helper(ptr);
        memset(tmp_line, 0, 512);
        memmove(tmp_line, ptr, line_len);
        ptr += line_len;


        char *ptr1 = tmp_line;
        char tmp_arg[512];
        int i = argc - 1;
        while (ptr1 - tmp_line != strlen(tmp_line)) {
            int space_len = blank_space_helper(ptr1);
            memset(tmp_arg, 0, 512);
            memmove(tmp_arg, ptr1, space_len - 1);
            ptr1 += space_len;
            new_args[i] = malloc(strlen(tmp_arg) + 1);
            strcpy(new_args[i], tmp_arg);
            i++;
        }
        new_args[i] = 0;
        for (int i = 1; i < argc; i++) {
            new_args[i - 1] = malloc(strlen(argv[i]) + 1);
            strcpy(new_args[i - 1], argv[i]);
        }
        if (fork() == 0) {
            exec(new_args[0], new_args);
            fprintf(2, "exec: failed");
            exit(1);
        } else {
            wait(0);
        }
    }
    exit(0);
}