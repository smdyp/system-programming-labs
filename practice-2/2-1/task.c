#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Структура для передачи данных в поток
typedef struct {
    long long result;
    char operation; // 's' - сумма, 'p' - произведение
} ThreadResult;

// Функция для вычисления суммы чисел от 1 до 1000
void* calculate_sum(void* arg) {
    ThreadResult* res = (ThreadResult*)arg;
    long long sum = 0;
    
    for (int i = 1; i <= 1000; i++) {
        sum += i;
    }
    
    res->result = sum;
    res->operation = 's';
    
    printf("Поток суммы: вычисление завершено, сумма = %lld\n", sum);
    pthread_exit((void*)res);
}

// Функция для вычисления произведения чисел от 1 до 10
void* calculate_product(void* arg) {
    ThreadResult* res = (ThreadResult*)arg;
    long long product = 1;
    
    for (int i = 1; i <= 10; i++) {
        product *= i;
    }
    
    res->result = product;
    res->operation = 'p';
    
    printf("Поток произведения: вычисление завершено, произведение = %lld\n", product);
    pthread_exit((void*)res);
}

int main() {
    pthread_t thread_sum, thread_product;
    ThreadResult* sum_result;
    ThreadResult* product_result;
    int ret;
    
    printf("=== Программа многопоточных вычислений ===\n");
    printf("Главный поток запущен (ID: %lu)\n", pthread_self());
    
    // Выделяем память для результатов
    sum_result = (ThreadResult*)malloc(sizeof(ThreadResult));
    product_result = (ThreadResult*)malloc(sizeof(ThreadResult));
    
    if (!sum_result || !product_result) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return EXIT_FAILURE;
    }
    
    // Создаем поток для вычисления суммы
    printf("\nСоздаем поток для вычисления суммы чисел 1-1000...\n");
    ret = pthread_create(&thread_sum, NULL, calculate_sum, (void*)sum_result);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания потока суммы: %d\n", ret);
        free(sum_result);
        free(product_result);
        return EXIT_FAILURE;
    }
    printf("Поток суммы создан (ID: %lu)\n", thread_sum);
    
    // Создаем поток для вычисления произведения
    printf("Создаем поток для вычисления произведения чисел 1-10...\n");
    ret = pthread_create(&thread_product, NULL, calculate_product, (void*)product_result);
    if (ret != 0) {
        fprintf(stderr, "Ошибка создания потока произведения: %d\n", ret);
        pthread_join(thread_sum, NULL);
        free(sum_result);
        free(product_result);
        return EXIT_FAILURE;
    }
    printf("Поток произведения создан (ID: %lu)\n", thread_product);
    
    // Ожидаем завершения потоков
    printf("\nГлавный поток ожидает завершения дочерних потоков...\n");
    
    void* sum_return;
    void* product_return;
    
    ret = pthread_join(thread_sum, &sum_return);
    if (ret != 0) {
        fprintf(stderr, "Ошибка ожидания потока суммы: %d\n", ret);
    } else {
        printf("Поток суммы завершился успешно\n");
    }
    
    ret = pthread_join(thread_product, &product_return);
    if (ret != 0) {
        fprintf(stderr, "Ошибка ожидания потока произведения: %d\n", ret);
    } else {
        printf("Поток произведения завершился успешно\n");
    }
    
    // Выводим результаты
    printf("\n=== Результаты вычислений ===\n");
    
    ThreadResult* sum_res = (ThreadResult*)sum_return;
    ThreadResult* product_res = (ThreadResult*)product_return;
    
    if (sum_res && sum_res->operation == 's') {
        printf("Сумма чисел от 1 до 1000: %lld\n", sum_res->result);
        
        // Проверяем результат математической формулой: n*(n+1)/2
        long long expected_sum = 1000 * (1000 + 1) / 2;
        if (sum_res->result == expected_sum) {
            printf("✓ Результат верен (проверено по формуле n*(n+1)/2)\n");
        } else {
            printf("✗ Неожиданный результат, ожидалось: %lld\n", expected_sum);
        }
    }
    
    if (product_res && product_res->operation == 'p') {
        printf("Произведение чисел от 1 до 10 (10!): %lld\n", product_res->result);
        
        // Проверяем результат факториала
        long long expected_product = 1;
        for (int i = 1; i <= 10; i++) {
            expected_product *= i;
        }
        
        if (product_res->result == expected_product) {
            printf("✓ Результат верен (10! = 3,628,800)\n");
        } else {
            printf("✗ Неожиданный результат, ожидалось: %lld\n", expected_product);
        }
    }
    
    // Освобождаем память
    free(sum_result);
    free(product_result);
    
    printf("\n=== Программа успешно завершена ===\n");
    return EXIT_SUCCESS;
}
