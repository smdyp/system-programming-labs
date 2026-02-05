#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int line_count = 0;
    
    printf("=== Программа чтения из stdin и записи в stdout ===\n");
    printf("Введите текст (Ctrl+D для завершения):\n");
    printf("----------------------------------------\n");
    
    // Читаем данные из stdin пока не достигнем EOF (Ctrl+D)
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0) {
        line_count++;
        
        // Добавляем нулевой символ в конец для безопасности
        buffer[bytes_read] = '\0';
        
        // Выводим прочитанные данные в stdout
        printf("Строка %d (%ld байт): ", line_count, bytes_read);
        write(STDOUT_FILENO, buffer, bytes_read);
        
        // Если последний символ не \n, добавляем его
        if (bytes_read > 0 && buffer[bytes_read - 1] != '\n') {
            write(STDOUT_FILENO, "\n", 1);
        }
    }
    
    // Проверяем ошибку чтения
    if (bytes_read < 0) {
        perror("Ошибка при чтении из stdin");
        return EXIT_FAILURE;
    }
    
    printf("\n----------------------------------------\n");
    printf("Программа завершена. Прочитано строк: %d\n", line_count);
    
    // Демонстрация разных способов вывода
    printf("\n=== Дополнительная демонстрация ===\n");
    
    // Разные способы вывода (для strace)
    fprintf(stdout, "1. Использование fprintf()\n");
    fputs("2. Использование fputs()\n", stdout);
    printf("3. Использование printf()\n");
    write(STDOUT_FILENO, "4. Использование write()\n", 25);
    
    return EXIT_SUCCESS;
}