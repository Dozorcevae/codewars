// fillqueue.c
// Запускается в ТОМ терминале, который мы "смотрим" через watchtty.
// Ничего не читает со stdin: просто висит, чтобы ввод накапливался в очереди.
// Выход: Ctrl-C.

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void on_sigint(int signo) {
    (void)signo;
    _exit(0);
}

int main(void) {
    signal(SIGINT, on_sigint);
    printf("fillqueue: type a lot in this terminal. Ctrl-C to exit.\n");
    fflush(stdout);

    while (1) pause();
    return 0;
}
