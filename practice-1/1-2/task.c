#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    pid_t pid;
    
    printf("Родительский процесс: PID = %d\n", getpid());
    
    // Создание дочернего процесса
    pid = fork();
    
    if (pid < 0) {
        // Ошибка при создании процесса
        perror("Ошибка при создании процесса (fork)");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        // Дочерний процесс
        printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
        printf("Дочерний процесс запускает команду: ls -la\n");
        
        // Запуск команды ls -la
        execl("/bin/ls", "ls", "-la", NULL);
        
        // Если execl вернул управление, значит произошла ошибка
        perror("Ошибка при запуске команды (execl)");
        exit(EXIT_FAILURE);
    } else {
        // Родительский процесс
        int status;
        
        printf("Родительский процесс ждёт завершения дочернего (PID = %d)\n", pid);
        
        // Ожидание завершения дочернего процесса
        if (wait(&status) == -1) {
            perror("Ошибка при ожидании завершения процесса (wait)");
            exit(EXIT_FAILURE);
        }
        
        // Проверка статуса завершения дочернего процесса
        if (WIFEXITED(status)) {
            printf("Команда ls завершена с кодом возврата: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Команда ls завершена сигналом: %d\n", WTERMSIG(status));
        }
        
        printf("Родительский процесс завершает работу\n");
    }
    
    return EXIT_SUCCESS;
}