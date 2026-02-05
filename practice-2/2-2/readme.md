# Синхронизация потоков с использованием мьютексов

## Описание задания

Написать программу на языке C с использованием библиотеки pthread, которая:
1. Создает глобальный массив `arr[10]`, инициализированный нулями
2. Создает 10 потоков, каждый из которых записывает в соответствующий элемент массива
3. Поток с индексом `i` записывает значение `i * 10` в элемент `arr[i]`
4. Использует мьютекс для защиты доступа к общему массиву
5. Демонстрирует корректную синхронизацию потоков

## Технические требования

- **Использование мьютексов**: `pthread_mutex_t`
- **Используемые функции**: 
  - `pthread_mutex_init()` - инициализация мьютекса
  - `pthread_mutex_lock()` - блокировка мьютекса
  - `pthread_mutex_unlock()` - разблокировка мьютекса
  - `pthread_mutex_destroy()` - уничтожение мьютекса
- **Компиляция**: `gcc -pthread -o task task.c`

## Инструкция по запуску

### 1. Создание файла с исходным кодом

Создайте файл `task.c` со следующим содержимым:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int arr[SIZE] = {0};
pthread_mutex_t mutex;

void* worker(void* arg) {
    int i = *(int*)arg;
    
    pthread_mutex_lock(&mutex);
    
    arr[i] = i * 10;
    printf("Поток %d записал %d в arr[%d]\n", i, arr[i], i);
    
    pthread_mutex_unlock(&mutex);
    
    free(arg);
    return NULL;
}

int main() {
    pthread_t threads[SIZE];
    
    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < SIZE; i++) {
        int* index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i], NULL, worker, index);
    }
    
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nФинальный массив: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

## Вывод
Поток 0 записал 0 в arr[0]
Поток 2 записал 20 в arr[2]
Поток 3 записал 30 в arr[3]
Поток 1 записал 10 в arr[1]
Поток 4 записал 40 в arr[4]
Поток 5 записал 50 в arr[5]
Поток 6 записал 60 в arr[6]
Поток 7 записал 70 в arr[7]
Поток 8 записал 80 в arr[8]
Поток 9 записал 90 в arr[9]

Финальный массив: 0 10 20 30 40 50 60 70 80 90