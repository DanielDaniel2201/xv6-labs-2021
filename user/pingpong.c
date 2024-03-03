// #include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p0[2], p1[2];
    pipe(p0); // pipe for dataflow from parent to child
    pipe(p1); // pipe for dataflow from child to parent

    if (fork() == 0) {
        close(p0[1]);
        close(p1[0]);
        char buf[1];
        while (read(p0[0], buf, 1) != 1) {}
        fprintf(1, "%d: received ping\n", getpid());
        write(p1[1], "a", 1);
        exit(0);
    } else {
        close(p0[0]);
        close(p1[1]);
        write(p0[1], "a", 1);
        char buf[1];
        while (read(p1[0], buf, 1) != 1) {}
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }

}