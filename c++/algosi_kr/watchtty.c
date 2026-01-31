// watchtty.c
// Использование: ./watchtty /dev/pts/N
// Печатает количество байт, ожидающих чтения во входной очереди указанного tty.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /dev/pts/N\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Watching: %s (Ctrl-C to stop)\n", argv[1]);

    while (1) {
        int n = 0;
        if (ioctl(fd, FIONREAD, &n) != 0) {
            perror("ioctl(FIONREAD)");
            close(fd);
            return 1;
        }
        printf("\rbytes waiting: %d   ", n);
        fflush(stdout);
        usleep(200000); // 200 ms
    }

    // unreachable
    close(fd);
    return 0;
}
