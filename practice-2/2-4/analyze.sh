#!/bin/bash

# Скрипт для анализа системных вызовов с помощью strace

echo "=== Анализ системных вызовов с strace ===" > analysis.txt
echo "Дата и время: $(date)" >> analysis.txt
echo "" >> analysis.txt

# Компиляция программы
echo "1. Компиляция программы..." | tee -a analysis.txt
gcc -o program program.c
if [ $? -ne 0 ]; then
    echo "Ошибка компиляции" | tee -a analysis.txt
    exit 1
fi
echo "Программа скомпилирована успешно" >> analysis.txt
echo "" >> analysis.txt

# Создание тестового файла с входными данными
echo "2. Создание тестового файла..." >> analysis.txt
cat > test_input.txt << 'EOF'
Первая строка для тестирования
Вторая строка с данными
Третья строка программы
Четвертая и последняя строка
EOF
echo "Создан test_input.txt с 4 строками" >> analysis.txt
echo "" >> analysis.txt

# Запуск strace с выводом в файл
echo "3. Запуск strace с перенаправлением ввода из файла..." >> analysis.txt
echo "Команда: strace -o strace_output.txt ./program < test_input.txt" >> analysis.txt
echo "" >> analysis.txt

strace -o strace_output.txt ./program < test_input.txt > program_output.txt 2>&1

echo "Результат работы программы сохранен в program_output.txt" >> analysis.txt
echo "Вывод strace сохранен в strace_output.txt" >> analysis.txt
echo "" >> analysis.txt

# Анализ вызовов read
echo "4. Анализ вызовов read():" >> analysis.txt
echo "------------------------" >> analysis.txt
grep -c "^read(" strace_output.txt >> analysis.txt
echo "Все вызовы read():" >> analysis.txt
grep "^read(" strace_output.txt >> analysis.txt
echo "" >> analysis.txt

# Анализ вызовов write
echo "5. Анализ вызовов write():" >> analysis.txt
echo "-------------------------" >> analysis.txt
grep -c "^write(" strace_output.txt >> analysis.txt
echo "Все вызовы write():" >> analysis.txt
grep "^write(" strace_output.txt >> analysis.txt
echo "" >> analysis.txt

# Подробная статистика
echo "6. Подробная статистика:" >> analysis.txt
echo "----------------------" >> analysis.txt

# Подсчет вызовов read с параметрами
echo "READ вызовы (сгруппированы по FD):" >> analysis.txt
grep "^read(" strace_output.txt | awk -F'[(),]' '{print $2}' | sort | uniq -c >> analysis.txt
echo "" >> analysis.txt

# Подсчет вызовов write с параметрами
echo "WRITE вызовы (сгруппированы по FD):" >> analysis.txt
grep "^write(" strace_output.txt | awk -F'[(),]' '{print $2}' | sort | uniq -c >> analysis.txt
echo "" >> analysis.txt

# Анализ других интересных системных вызовов
echo "7. Другие интересные системные вызовы:" >> analysis.txt
echo "------------------------------------" >> analysis.txt

echo "Количество открытий файлов (open):" >> analysis.txt
grep -c "^open(" strace_output.txt >> analysis.txt
echo "" >> analysis.txt

echo "Вызовы для управления памятью:" >> analysis.txt
grep -E "^(brk|mmap|mprotect|munmap)\(" strace_output.txt | head -10 >> analysis.txt
echo "" >> analysis.txt

echo "Вызовы для завершения программы:" >> analysis.txt
grep -E "^(exit|exit_group)\(" strace_output.txt >> analysis.txt
echo "" >> analysis.txt

# Сводка
echo "8. Сводка анализа:" >> analysis.txt
echo "-----------------" >> analysis.txt
echo "Всего системных вызовов:" >> analysis.txt
wc -l strace_output.txt | awk '{print $1}' >> analysis.txt
echo "" >> analysis.txt

echo "Топ-5 самых частых системных вызовов:" >> analysis.txt
awk '{print $1}' strace_output.txt | sed 's/(.*//' | sort | uniq -c | sort -rn | head -5 >> analysis.txt
echo "" >> analysis.txt

echo "=== Анализ завершен ===" >> analysis.txt

# Вывод результатов на экран
echo ""
echo "Результаты анализа сохранены в analysis.txt"
echo "Краткая сводка:"
echo "---------------"
echo "Всего вызовов read(): $(grep -c "^read(" strace_output.txt)"
echo "Всего вызовов write(): $(grep -c "^write(" strace_output.txt)"
echo ""
echo "Для подробного анализа смотрите файлы:"
echo "  analysis.txt      - полный отчет"
echo "  strace_output.txt - полный вывод strace"
echo "  program_output.txt - вывод программы"
echo "  test_input.txt    - тестовые входные данные"