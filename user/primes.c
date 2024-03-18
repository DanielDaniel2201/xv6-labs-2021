#include "kernel/types.h"
#include "user/user.h"

void new_proc(int p[2]);

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        new_proc(p); // does not read from child proc
    } else {
        close(p[0]);
        for (int i = 2; i < 35; i++) {
            if (write(p[1], &i, sizeof(int)) != sizeof(int)) {
                fprintf(2, "first process failed to write ints to its child");
                exit(1);
            }
        }
        close(p[1]); // the write is done
        wait(0);
        exit(0);
    }
    exit(0);
}

void new_proc(int p[2]) {
    close(p[1]); // does not write to its parent proc
    int prime;
    int n;
    int new_p[2];
    pipe(new_p);
    // int flag; // indicates whether there are still numbers left
    if (read(p[0], &prime, sizeof(int)) != 4) {
        fprintf(2, "child process failed to read from its parent");
        exit(1);
    }
    fprintf(1, "prime %d\n", prime);

    int flag = read(p[0], &n, sizeof(int));
    if (flag) {
        if (fork() == 0) {
            new_proc(new_p);
        } else {
            close(new_p[0]);
            if (n % prime) {
                write(new_p[1], &n, sizeof(int));
            }
            while (read(p[0], &n, sizeof(int)) == sizeof(int)) {
                if (n % prime) {
                    write(new_p[1], &n, sizeof(int));
                }
            }
            close(p[0]);
            close(new_p[1]);
            wait(0);
        }
    }
    exit(0);
}