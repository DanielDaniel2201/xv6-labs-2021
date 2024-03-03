#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        char *msg = "Usage: sleep <positive number>\ne.g. sleep 10\n";
        fprintf(2, msg);
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}