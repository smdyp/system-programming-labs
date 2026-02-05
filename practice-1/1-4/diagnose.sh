#!/bin/bash

# Диагностика процессов
# Скрипт для сбора информации о процессах системы

REPORT_FILE="report.txt"
echo "=== Отчет диагностики процессов ===" > $REPORT_FILE
echo "Дата и время: $(date)" >> $REPORT_FILE
echo "Пользователь: $USER" >> $REPORT_FILE
echo "" >> $REPORT_FILE

# 1. Найти процессы пользователя
echo "1. Процессы пользователя $USER:" | tee -a $REPORT_FILE
echo "========================================" | tee -a $REPORT_FILE
ps -u $USER -o pid,ppid,cmd,%cpu,%mem,stat --no-headers 2>/dev/null | head -20 | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE

# Получаем PID текущего shell для примера
SHELL_PID=$$
echo "PID текущего shell: $SHELL_PID" | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE

# 2. Найти процесс с максимальным CPU
echo "2. Топ-5 процессов по использованию CPU:" | tee -a $REPORT_FILE
echo "========================================" | tee -a $REPORT_FILE
top -bn1 -o +%CPU | head -15 | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE

# 3. Вывести карту памяти для процесса (используем текущий shell)
if [ -d "/proc/$SHELL_PID" ]; then
    echo "3. Карта памяти процесса $SHELL_PID (bash):" | tee -a $REPORT_FILE
    echo "========================================" | tee -a $REPORT_FILE
    cat /proc/$SHELL_PID/maps 2>/dev/null | head -30 | tee -a $REPORT_FILE
else
    echo "3. Не удалось получить информацию о процессе $SHELL_PID" | tee -a $REPORT_FILE
fi
echo "" >> $REPORT_FILE

# 4. Дополнительная информация: открытые файлы
echo "4. Открытые файлы процесса $SHELL_PID:" | tee -a $REPORT_FILE
echo "========================================" | tee -a $REPORT_FILE
lsof -p $SHELL_PID 2>/dev/null | head -20 | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE

# 5. Информация из /proc
echo "5. Информация о процессе из /proc/$SHELL_PID/status:" | tee -a $REPORT_FILE
echo "========================================" | tee -a $REPORT_FILE
cat /proc/$SHELL_PID/status 2>/dev/null | grep -E "^(Name|Pid|PPid|State|VmSize|VmRSS)" | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE

# 6. Пример использования kill (информационный, не выполняем)
echo "6. Информация о сигналах (команда kill):" | tee -a $REPORT_FILE
echo "========================================" | tee -a $REPORT_FILE
echo "Примеры сигналов для команды kill:" | tee -a $REPORT_FILE
echo "  kill -SIGTERM PID  - корректное завершение" | tee -a $REPORT_FILE
echo "  kill -SIGKILL PID  - принудительное завершение" | tee -a $REPORT_FILE
echo "  kill -SIGHUP PID   - перезапуск процесса" | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE

echo "Отчет сохранен в файл: $REPORT_FILE" | tee -a $REPORT_FILE
echo "" >> $REPORT_FILE
echo "=== Диагностика завершена ===" | tee -a $REPORT_FILE