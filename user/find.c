#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *name, char *dir) {
    int fd;
    struct dirent de;
    struct stat st;
    char buf[512], *p;
    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++ = '/';
    strcpy(p, name);

    if ((fd = open(dir, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir);
        exit(1);
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".")  == 0 || strcmp(de.name, "..") == 0) {
            continue;
        }
        char tmp_buf[512], *tmp;
        strcpy(tmp_buf, dir);
        tmp = tmp_buf + strlen(tmp_buf);
        *tmp++ = '/';
        strcpy(tmp, de.name);
        if(stat(tmp_buf, &st) < 0){
            fprintf(2, "find: cannot stat %s\n", tmp_buf);
            continue;
        }
        switch (st.type) {
            case T_FILE:
                if (strcmp(de.name, name) == 0) {
                    fprintf(1, "%s\n", buf);
                }
                break;
            case T_DIR:
                *p++ = '/';
                find(name, tmp_buf);
                break;
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {

    if (argc != 3 || strcmp(argv[1], ".")) {
        fprintf(2, "find usage: find . [file name]\n");
        exit(1);
    }
    find(argv[2], argv[1]);
    exit(1);
}