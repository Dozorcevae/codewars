// upcopychars2.c
// Читает stdin по 1 байту и пишет в stdout 1 байт (без изменений).
// Удобно для экспериментов с MAX_INPUT, icrnl/onlcr, перенаправлений.

#include <unistd.h>
#include <errno.h>

int main(void) {
    unsigned char ch;

    while (1) {
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n == 0) break;                 // EOF
        if (n < 0) {
            if (errno == EINTR) continue;
            return 1;
        }
        if (write(STDOUT_FILENO, &ch, 1) < 0) return 1;
    }
    return 0;
}
