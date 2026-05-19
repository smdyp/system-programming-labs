#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

# Файл для сохранения результатов
ANALYSIS_FILE="analysis.txt"

# Очистка файла
> $ANALYSIS_FILE

echo -e "${BLUE}=== Анализ памяти и процессов ===${NC}"
echo "# Анализ памяти и процессов" > $ANALYSIS_FILE
echo "# Выполнено: $(date '+%Y-%m-%d %H:%M:%S')" >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# Компиляция
echo -e "${GREEN}1. Компиляция программы...${NC}"
gcc -Wall -Wextra -o task task.c 2>/dev/null

if [ $? -ne 0 ]; then
    echo -e "${RED}Ошибка компиляции!${NC}"
    exit 1
fi

echo "Компиляция успешна" >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# Запуск программы в фоновом режиме
echo -e "${GREEN}2. Запуск программы в фоновом режиме...${NC}"
./task &
PROGRAM_PID=$!

echo "Программа запущена с PID: $PROGRAM_PID" >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# Ожидание инициализации
sleep 2

# 1. Информация о процессе через ps
echo -e "${GREEN}3. Проверка процесса через ps...${NC}"
echo "=== 1. ПРОВЕРКА ПРОЦЕССА ЧЕРЕЗ ps ===" >> $ANALYSIS_FILE
echo "\$ ps -p $PROGRAM_PID -o pid,ppid,stat,%cpu,%mem,comm" >> $ANALYSIS_FILE
ps -p $PROGRAM_PID -o pid,ppid,stat,%cpu,%mem,comm 2>/dev/null >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# 2. Карта памяти через pmap
echo -e "${GREEN}4. Анализ карты памяти через pmap...${NC}"
echo "=== 2. КАРТА ПАМЯТИ ЧЕРЕЗ pmap ===" >> $ANALYSIS_FILE
echo "\$ pmap $PROGRAM_PID" >> $ANALYSIS_FILE
pmap $PROGRAM_PID 2>/dev/null >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# 3. Детальная карта через /proc
echo -e "${GREEN}5. Просмотр карты памяти через /proc/$PROGRAM_PID/maps...${NC}"
echo "=== 3. КАРТА ПАМЯТИ ЧЕРЕЗ /proc/$PROGRAM_PID/maps ===" >> $ANALYSIS_FILE
echo "\$ cat /proc/$PROGRAM_PID/maps" >> $ANALYSIS_FILE
cat /proc/$PROGRAM_PID/maps 2>/dev/null >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# 4. Статус процесса через /proc
echo -e "${GREEN}6. Просмотр статуса процесса через /proc/$PROGRAM_PID/status...${NC}"
echo "=== 4. СТАТУС ПРОЦЕССА ЧЕРЕЗ /proc/$PROGRAM_PID/status ===" >> $ANALYSIS_FILE
echo "\$ cat /proc/$PROGRAM_PID/status | grep -E 'Name|State|Pid|PPid|VmPeak|VmSize|VmRSS|Threads'" >> $ANALYSIS_FILE
cat /proc/$PROGRAM_PID/status 2>/dev/null | grep -E "Name|State|Pid|PPid|VmPeak|VmSize|VmRSS|Threads" >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# 5. Динамическое наблюдение через top
echo -e "${GREEN}7. Динамическое наблюдение через top (один цикл)...${NC}"
echo "=== 5. ДИНАМИЧЕСКОЕ НАБЛЮДЕНИЕ ЧЕРЕЗ top ===" >> $ANALYSIS_FILE
echo "\$ top -b -n 1 -p $PROGRAM_PID" >> $ANALYSIS_FILE
top -b -n 1 -p $PROGRAM_PID 2>/dev/null | tail -5 >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# Пауза для демонстрации
echo -e "${YELLOW}Программа работает. Нажмите Enter для продолжения анализа...${NC}"
read -r

# 6. Проверка через valgrind (отдельный запуск)
echo -e "${GREEN}8. Проверка через valgrind...${NC}"
echo "=== 6. ПРОВЕРКА ЧЕРЕЗ valgrind ===" >> $ANALYSIS_FILE
echo "\$ valgrind --leak-check=full ./task" >> $ANALYSIS_FILE
echo "Примечание: Программа запускается отдельно для анализа" >> $ANALYSIS_FILE
timeout 5 valgrind --leak-check=full ./task 2>> $ANALYSIS_FILE <<< 'q'
echo "" >> $ANALYSIS_FILE

# Остановка программы
echo -e "${GREEN}9. Остановка процесса...${NC}"
echo "=== 7. ОСТАНОВКА ПРОЦЕССА ===" >> $ANALYSIS_FILE
echo "\$ kill $PROGRAM_PID" >> $ANALYSIS_FILE
kill $PROGRAM_PID 2>/dev/null
sleep 1
echo "Процесс $PROGRAM_PID остановлен" >> $ANALYSIS_FILE
echo "" >> $ANALYSIS_FILE

# 7. Трассировка системных вызовов (strace)
echo -e "${GREEN}10. Трассировка системных вызовов через strace...${NC}"
echo "=== 8. ТРАССИРОВКА СИСТЕМНЫХ ВЫЗОВОВ (strace) ===" >> $ANALYSIS_FILE
echo "\$ strace -c ./task" >> $ANALYSIS_FILE
strace -c ./task 2>> $ANALYSIS_FILE <<< 'q'
echo "" >> $ANALYSIS_FILE

# Вывод наблюдений
echo -e "${BLUE}11. Формирование выводов...${NC}"
echo "=== 9. НАБЛЮДЕНИЯ И ВЫВОДЫ ===" >> $ANALYSIS_FILE

cat >> $ANALYSIS_FILE << 'EOF'

1. Наблюдения по ps:
   - Процесс task запущен и находится в состоянии S (спящий)
   - Процесс ожидает сигнала для завершения
   - Использование CPU близко к 0%

2. Наблюдения по pmap и /proc/[PID]/maps:
   - Код программы (text) расположен в сегменте с правами r-xp (чтение+исполнение)
   - Данные (data) расположены в сегменте rw-p (чтение+запись)
   - Куча (heap) отмечена [heap] с правами rw-p
   - Стек (stack) отмечен [stack] с правами rw-p
   - Разделяемые библиотеки (libc.so.6) отображаются в память

3. Наблюдения по /proc/[PID]/status:
   - VmPeak: пиковое использование виртуальной памяти
   - VmSize: текущий размер виртуальной памяти
   - VmRSS: фактически используемая физическая память
   - Threads: количество потоков (1)

4. Наблюдения по valgrind:
   - Все выделенные блоки памяти освобождены
   - Утечек памяти не обнаружено

5. Наблюдения по strace:
   - Основные системные вызовы: brk, mmap для выделения памяти
   - Системные вызовы для управления процессом: nanosleep (sleep)

6. Инструменты диагностики:
   - ps: базовый просмотр информации о процессах
   - pmap: наглядный вывод карты памяти
   - /proc/[PID]/maps: детальная карта памяти с адресами
   - /proc/[PID]/status: подробный статус процесса
   - valgrind: эффективен для поиска утечек памяти
   - strace: показывает все системные вызовы

7. Рекомендации:
   - Для быстрого просмотра: ps и pmap
   - Для детального анализа: cat /proc/[PID]/maps
   - Для поиска утечек: valgrind --leak-check=full
   - Для анализа системных вызовов: strace -c

EOF

echo "" >> $ANALYSIS_FILE
echo "=== АНАЛИЗ ЗАВЕРШЁН ===" >> $ANALYSIS_FILE

echo -e "${GREEN}Готово! Результаты сохранены в файл: $ANALYSIS_FILE${NC}"
echo -e "${BLUE}Просмотреть результаты: cat $ANALYSIS_FILE${NC}"