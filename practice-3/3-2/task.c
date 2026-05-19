#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    pid_t pid;
    int numbers[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    
    // Удаляем FIFO если существует
    unlink(FIFO_NAME);
    
    // Создаем FIFO
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    
    printf("FIFO создан: %s\n", FIFO_NAME);
    
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) { // Отправитель
        printf("Родитель: открытие FIFO для записи...\n");
        int fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        
        printf("Родитель: отправка чисел...\n");
        // Отправляем числа
        if (write(fd, numbers, sizeof(numbers)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        
        printf("Родитель: данные отправлены\n");
        close(fd);
        wait(NULL);
        
    } else { // Получатель
        printf("Дочерний: открытие FIFO для чтения...\n");
        int fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        
        int received[10];
        
        printf("Дочерний: чтение чисел...\n");
        // Читаем числа
        if (read(fd, received, sizeof(received)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        // Вычисляем среднее арифметическое
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += received[i];
            printf("received[%d] = %d\n", i, received[i]);
        }
        float average = sum / 10.0;
        
        printf("Сумма: %d\n", sum);
        printf("Среднее арифметическое: %.2f\n", average);
        close(fd);
    }
    
    // Удаляем FIFO
    unlink(FIFO_NAME);
    printf("FIFO удален\n");
    return 0;
}