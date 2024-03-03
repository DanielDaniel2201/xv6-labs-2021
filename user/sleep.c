#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        char *msg = "Usage: sleep <positive number>\n e.g. sleep 10\n";
        write(1, msg, strlen(msg));
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}