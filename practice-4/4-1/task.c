#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Функция для подсчёта UTF-8 символов
size_t utf8_strlen(const char *s) {
    size_t count = 0;
    while (*s) {
        // Если это не продолжение UTF-8 символа
        if ((*s & 0xC0) != 0x80) {
            count++;
        }
        s++;
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    char *str = NULL;
    size_t size = 0;
    ssize_t len;
    
    printf("Введите строку (русские/английские символы): ");
    
    // getline автоматически выделяет память
    len = getline(&str, &size, stdin);
    
    if (len == -1) {
        perror("getline failed");
        exit(EXIT_FAILURE);
    }
    
    // Удаляем символ новой строки
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
    
    // Подсчёт символов в UTF-8 строке
    size_t char_count = utf8_strlen(str);
    
    printf("Исходная строка: %s\n", str);
    printf("Длина строки (байт): %zd\n", len);
    printf("Длина строки (символов): %zu\n", char_count);
    
    // Демонстрация работы calloc
    char *str2 = (char*)calloc(len + 1, sizeof(char));
    if (str2 == NULL) {
        perror("calloc failed");
        free(str);
        exit(EXIT_FAILURE);
    }
    
    strcpy(str2, str);
    printf("Строка после calloc: %s\n", str2);
    
    // Демонстрация realloc для уменьшения размера
    char *temp = (char*)realloc(str, (len + 1) * sizeof(char));
    if (temp == NULL) {
        perror("realloc for shrink failed");
    } else {
        str = temp;
        printf("Память уменьшена до точного размера строки\n");
    }
    
    // Освобождение памяти
    free(str);
    free(str2);
    printf("Память освобождена.\n");
    
    return 0;
}