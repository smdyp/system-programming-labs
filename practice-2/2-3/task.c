#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

volatile sig_atomic_t counter = 0;
volatile sig_atomic_t running = 1;

void safe_write(const char* str) {
    write(STDOUT_FILENO, str, strlen(str));
}

void handle_sigusr1(int sig) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), 
             "\n[SIGUSR1] Текущий счетчик: %d\n", counter);
    write(STDOUT_FILENO, buffer, strlen(buffer));
}

void handle_sigterm(int sig) {
    write(STDOUT_FILENO, "\n[SIGTERM] Завершение работы\n", 28);
    running = 0;
}

int main() {
    printf("PID: %d\n", getpid());
    printf("Отправьте SIGUSR1 для показа счетчика\n");
    printf("Отправьте SIGTERM для завершения\n\n");
    
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGTERM, handle_sigterm);
    
    for (counter = 1; counter <= 100 && running; counter++) {
        printf("Счетчик: %d\n", counter);
        sleep(1);
    }
    
    printf("\nПрограмма завершена. Счетчик: %d\n", counter - 1);
    return 0;
}