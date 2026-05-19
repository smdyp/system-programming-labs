#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    int numbers[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sum = 0;
    
    // Создание канала
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) { // Родительский процесс
        close(pipefd[0]); // Закрываем чтение
        
        // Отправляем массив чисел
        if (write(pipefd[1], numbers, sizeof(numbers)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        
        close(pipefd[1]); // Закрываем запись
        wait(NULL); // Ожидаем завершения дочернего процесса
        
    } else { // Дочерний процесс
        close(pipefd[1]); // Закрываем запись
        
        int received[10];
        
        // Читаем массив чисел
        if (read(pipefd[0], received, sizeof(received)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        // Вычисляем сумму
        for (int i = 0; i < 10; i++) {
            sum += received[i];
        }
        
        printf("Сумма чисел: %d\n", sum);
        close(pipefd[0]);
    }
    
    return 0;
}