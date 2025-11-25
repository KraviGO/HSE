#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

static volatile sig_atomic_t ack_received = 0;
static pid_t peer_pid = 0;

void ack_handler(int signo) {
    (void)signo;
    ack_received = 1;
}

int main(void) {
    struct sigaction sa;

    printf("PID отправителя: %d\n", getpid());
    printf("Введите PID приёмника: ");
    fflush(stdout);

    if (scanf("%d", &peer_pid) != 1) {
        fprintf(stderr, "Не удалось считать PID приёмника\n");
        return 1;
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = ack_handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Ошибка sigaction");
        return 1;
    }

    long input;
    printf("Введите целое число для отправки: ");
    fflush(stdout);

    if (scanf("%ld", &input) != 1) {
        fprintf(stderr, "Не удалось считать число\n");
        return 1;
    }

    int32_t value = (int32_t)input;

    for (int i = 31; i >= 0; --i) {
        int bit = (value >> i) & 1;
        int sig = bit ? SIGUSR2 : SIGUSR1;

        ack_received = 0;

        if (kill(peer_pid, sig) == -1) {
            perror("Не удалось отправить бит");
            return 1;
        }

        while (!ack_received) {
            pause();
        }
    }

    if (kill(peer_pid, SIGINT) == -1) {
        perror("Не удалось отправить SIGINT");
        return 1;
    }

    printf("Число %ld успешно отправлено\n", input);

    return 0;
}