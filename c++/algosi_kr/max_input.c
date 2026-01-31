// max_input.c
// Печатает POSIX-минимум MAX_INPUT через pathconf для текущего терминала.

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
    const char *tty = ttyname(STDIN_FILENO);
    if (!tty) {
        perror("ttyname");
        return 1;
    }

    errno = 0;
    long v = pathconf(tty, _PC_MAX_INPUT);
    if (v == -1) {
        if (errno == 0) {
            // -1 и errno==0 значит "не ограничено" (редко для _PC_MAX_INPUT),
            // но по стандарту так бывает для некоторых pathconf.
            printf("pathconf(%s, _PC_MAX_INPUT) = unlimited/indeterminate\n", tty);
        } else {
            perror("pathconf");
            return 1;
        }
    } else {
        printf("tty: %s\n", tty);
        printf("pathconf(_PC_MAX_INPUT) = %ld\n", v);
    }
    return 0;
}
