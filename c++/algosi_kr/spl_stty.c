// spl_stty.c
// Мини-версия "stty" для учебных экспериментов.
// Поддержка:
//   ./spl_stty
//   ./spl_stty size
//   ./spl_stty erase X
//   ./spl_stty erase ^F
//   ./spl_stty erase ^?   (DEL = 127)

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

static int parse_cc_value(const char *s, unsigned char *out) {
    if (!s || !*s) return 0;

    // Формат ^X
    if (s[0] == '^' && s[1] != '\0') {
        if (s[1] == '?' && s[2] == '\0') { // ^? => DEL
            *out = 127;
            return 1;
        }
        // ^@ -> 0, ^A -> 1, ... ^Z -> 26, ^[ -> 27 и т.п.
        unsigned char c = (unsigned char)s[1];
        *out = (unsigned char)(c & 0x1F);
        return 1;
    }

    // Обычный одиночный символ
    if (s[1] == '\0') {
        *out = (unsigned char)s[0];
        return 1;
    }

    return 0;
}

static void print_size(void) {
    struct winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == 0) {
        printf("rows=%u cols=%u\n", ws.ws_row, ws.ws_col);
    } else {
        perror("ioctl(TIOCGWINSZ)");
    }
}

static void print_erase(const struct termios *t) {
    unsigned char e = (unsigned char)t->c_cc[VERASE];
    if (e < 32) {
        printf("erase = ^%c (code %u)\n", (char)(e + '@'), (unsigned)e);
    } else if (e == 127) {
        printf("erase = ^? (DEL, code 127)\n");
    } else {
        printf("erase = '%c' (code %u)\n", (char)e, (unsigned)e);
    }
}

int main(int argc, char **argv) {
    struct termios t;
    if (tcgetattr(STDIN_FILENO, &t) != 0) {
        perror("tcgetattr");
        return 1;
    }

    if (argc == 1) {
        print_erase(&t);
        print_size();
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "size") == 0) {
        print_size();
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "erase") == 0) {
        unsigned char v;
        if (!parse_cc_value(argv[2], &v)) {
            fprintf(stderr, "Bad erase value. Use single char or ^X (e.g. X, ^F, ^?)\n");
            return 1;
        }
        t.c_cc[VERASE] = v;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &t) != 0) {
            perror("tcsetattr");
            return 1;
        }
        // перечитаем и покажем
        if (tcgetattr(STDIN_FILENO, &t) == 0) print_erase(&t);
        return 0;
    }

    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s\n", argv[0]);
    fprintf(stderr, "  %s size\n", argv[0]);
    fprintf(stderr, "  %s erase X\n", argv[0]);
    fprintf(stderr, "  %s erase ^F\n", argv[0]);
    return 1;
}
