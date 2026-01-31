// upcopychars.c
// Читает stdin по 1 байту, переводит в верхний регистр и сразу пишет в stdout.
// В каноническом режиме символы "до ENTER" программе не приходят.

#include <unistd.h>
#include <ctype.h>
#include <errno.h>

int main(void) {
    unsigned char ch;

    while (1) {
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n == 0) break;                 // EOF (Ctrl-D на пустой строке)
        if (n < 0) {
            if (errno == EINTR) continue;  // если прервали сигналом
            return 1;
        }

        ch = (unsigned char)toupper(ch);
        if (write(STDOUT_FILENO, &ch, 1) < 0) return 1;
    }
    return 0;
}
