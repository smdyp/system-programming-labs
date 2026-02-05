#!/bin/bash

echo "=== Тестирование программы обработки сигналов ==="

# Компилируем программу
echo "Компиляция программы..."
gcc -o task task.c

if [ $? -ne 0 ]; then
    echo "Ошибка компиляции"
    exit 1
fi

echo "Запуск программы в фоновом режиме..."
./task &
PID=$!

echo "PID программы: $PID"
echo ""
echo "Ожидание 3 секунды..."
sleep 3

echo ""
echo "1. Отправка SIGUSR1 (показать счетчик)..."
kill -SIGUSR1 $PID
sleep 2

echo ""
echo "2. Отправка SIGUSR1 еще раз..."
kill -SIGUSR1 $PID
sleep 4

echo ""
echo "3. Отправка SIGTERM (завершение)..."
kill -SIGTERM $PID

echo ""
echo "Ожидание завершения программы..."
wait $PID

echo ""
echo "=== Тестирование завершено ==="