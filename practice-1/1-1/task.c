#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 512

int main() {
    int input_fd, output_fd;
    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];
    int count_a = 0;
    int count_A = 0;
    
    // Открываем файл text.txt для чтения
    input_fd = open("text.txt", O_RDONLY);
    if (input_fd == -1) {
        perror("Ошибка при открытии text.txt");
        return 1;
    }
    
    // Читаем файл и подсчитываем буквы
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == 'a') {
                count_a++;
            } else if (buffer[i] == 'A') {
                count_A++;
            }
        }
    }
    
    if (bytes_read == -1) {
        perror("Ошибка при чтении text.txt");
        close(input_fd);
        return 1;
    }
    
    // Закрываем входной файл
    if (close(input_fd) == -1) {
        perror("Ошибка при закрытии text.txt");
        return 1;
    }
    
    // Открываем файл count.txt для записи (создаём, если не существует)
    output_fd = open("count.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Ошибка при открытии/создании count.txt");
        return 1;
    }
    
    // Формируем строку с результатами
    char result[100];
    int length = snprintf(result, sizeof(result), 
                         "Количество букв 'a': %d\nКоличество букв 'A': %d\n", 
                         count_a, count_A);
    
    // Записываем результат в файл
    if (write(output_fd, result, length) != length) {
        perror("Ошибка при записи в count.txt");
        close(output_fd);
        return 1;
    }
    
    // Закрываем выходной файл
    if (close(output_fd) == -1) {
        perror("Ошибка при закрытии count.txt");
        return 1;
    }
    
    printf("Подсчёт завершён. Результат записан в count.txt\n");
    printf("Найдено: %d 'a' и %d 'A'\n", count_a, count_A);
    
    return 0;
}