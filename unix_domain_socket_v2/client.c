#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define CLI_PATH "./"
#define MAXLINE  80

// TODO: terminal signal, unlink socket file

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fputs("usage: ./client <socket_path>", stderr);
        exit(1);
    }
    char *server_sock_path = argv[1];

    struct sockaddr_un sock_addr;
    int                clisock_len = sizeof(sock_addr);

    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    // get client address and bind
    memset(&sock_addr, 0, clisock_len);
    sock_addr.sun_family = AF_UNIX;
    sprintf(sock_addr.sun_path, "%s%05d.sock", CLI_PATH, getpid());
    // get length
    int len = offsetof(struct sockaddr_un, sun_path) + strlen(sock_addr.sun_path);
    // when already exists
    unlink(sock_addr.sun_path);
    // bind
    if ((bind(fd, (struct sockaddr *)&sock_addr, len)) < 0) {
        perror("bind error");
        exit(1);
    }

    // get server address
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    sprintf(sock_addr.sun_path, "%s", server_sock_path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(server_sock_path);
    if (connect(fd, (struct sockaddr *)&sock_addr, len) < 0) {
        perror("connect error");
        exit(1);
    }

    char buf[MAXLINE];
    int  n;
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        write(fd, buf, strlen(buf));
        n = read(fd, buf, MAXLINE);
        if (n == 0) {
            printf("the other side has been closed.\n");
        }
        else {
            write(STDOUT_FILENO, buf, n);
        }
    }

    return EXIT_SUCCESS;
}
