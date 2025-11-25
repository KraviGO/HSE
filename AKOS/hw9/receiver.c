#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

static volatile sig_atomic_t bit_flag = 0;
static volatile sig_atomic_t last_bit = 0;
static volatile sig_atomic_t finished = 0;
static pid_t peer_pid = 0;

void bit_handler(int signo) {
    if (signo == SIGUSR1) {
        last_bit = 0;
    } else if (signo == SIGUSR2) {
        last_bit = 1;
    }

    bit_flag = 1;

    if (peer_pid > 0) {
        kill(peer_pid, SIGUSR1);
    }
}

void end_handler(int signo) {
    (void)signo;
    finished = 1;
}

int main(void) {
    struct sigaction sa_bit, sa_end;

    printf("PID приёмника: %d\n", getpid());
    printf("Введите PID отправителя: ");
    fflush(stdout);

    if (scanf("%d", &peer_pid) != 1) {
        fprintf(stderr, "Не удалось считать PID отправителя\n");
        return 1;
    }

    sigemptyset(&sa_bit.sa_mask);
    sa_bit.sa_flags = SA_RESTART;
    sa_bit.sa_handler = bit_handler;

    if (sigaction(SIGUSR1, &sa_bit, NULL) == -1) {
        perror("Ошибка sigaction(SIGUSR1)");
        return 1;
    }
    if (sigaction(SIGUSR2, &sa_bit, NULL) == -1) {
        perror("Ошибка sigaction(SIGUSR2)");
        return 1;
    }

    sigemptyset(&sa_end.sa_mask);
    sa_end.sa_flags = SA_RESTART;
    sa_end.sa_handler = end_handler;

    if (sigaction(SIGINT, &sa_end, NULL) == -1) {
        perror("Ошибка sigaction(SIGINT)");
        return 1;
    }

    uint32_t value = 0;
    int bits = 0;

    printf("Ожидание числа...\n");

    while (!finished) {
        pause();

        if (bit_flag) {
            value = (value << 1) | (last_bit & 1);
            bits++;
            bit_flag = 0;
        }
    }

    if (bits != 32) {
        fprintf(stderr, "Ожидалось 32 бита, получено %d\n", bits);
    }

    int32_t result = (int32_t)value;
    printf("Получено число: %d\n", result);

    return 0;
}