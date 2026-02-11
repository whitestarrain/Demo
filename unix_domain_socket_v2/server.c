#include <arpa/inet.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#define MAXLINE 80

// TODO: terminal signal, unlink socket file

int main(int argc, char *argv[])
{
    struct sockaddr_un un;
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, "un_test.sock");
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }
    int size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    if (bind(fd, (struct sockaddr *)&un, size) < 0) {
        perror("bind error");
        exit(1);
    }
    if (listen(fd, 10) < 0) {
        perror("listen error");
        exit(1);
    }

    printf("Accepting connection...\n");

    struct sockaddr_un cli_addr;
    int                clifd;
    socklen_t          len;
    uid_t              uidptr;
    char               buf[MAXLINE];
    while (1) {
        len = sizeof(cli_addr);
        if ((clifd = accept(fd, (struct sockaddr *)&cli_addr, &len)) < 0) {
            perror("accpet error");
            exit(1);
        }
        // obtain clisock_len.sun_path's length
        len -= offsetof(struct sockaddr_un, sun_path);
        // sun_path最后，设置为0，表示结束
        cli_addr.sun_path[len] = 0;
        struct stat statbuf;
        if (stat(cli_addr.sun_path, &statbuf) < 0) {
            perror("stat error");
            exit(1);
        }
        if (S_ISSOCK(statbuf.st_mode) == 0) {
            perror("S_ISSOCK error");
            exit(1);
        }
        uidptr = statbuf.st_uid;
        while (1) {
            int n = read(clifd, buf, MAXLINE);
            if (n == 0) {
                printf("the other side has been closed\n");
                break;
            }
            printf("received from from uid %d\n", (int)uidptr);
            for (int i = 0; i < n; i++) {
                buf[i] = toupper(buf[i]);
            }
            write(clifd, buf, n);
        }
        close(clifd);
    }

    return EXIT_SUCCESS;
}
