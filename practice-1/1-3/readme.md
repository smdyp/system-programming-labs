# Создание вложенных директорий и файла с правами доступа

## Описание задания

Написать bash-скрипт, который:
1. Создает вложенные директории: `level1/level2/level3`
2. В директории `level3` создает файл `secret.txt`
3. Устанавливает права доступа `600` для файла `secret.txt`
4. Выводит полный путь к созданному файлу

## Требования

- **Bash скрипты** - использование bash для написания скрипта
- **Работа с правами доступа** - использование команды `chmod`
- **Запуск**: `chmod +x script.sh && ./script.sh`

## Инструкция по запуску

### 1. Создание файла скрипта

Создайте файл `script.sh` с содержимым:

```bash
#!/bin/bash

echo "=== Начало выполнения скрипта ==="

# Создаем вложенные директории
echo "Создание директорий level1/level2/level3..."
mkdir -p level1/level2/level3

if [ $? -ne 0 ]; then
    echo "Ошибка: не удалось создать директории" >&2
    exit 1
fi

echo "Директории успешно созданы"

# Создаем файл secret.txt в level3
echo "Создание файла secret.txt в level3..."
echo "Это секретный файл" > level1/level2/level3/secret.txt

if [ $? -ne 0 ]; then
    echo "Ошибка: не удалось создать файл secret.txt" >&2
    exit 1
fi

echo "Файл secret.txt успешно создан"

# Устанавливаем права доступа 600
echo "Установка прав доступа 600 для secret.txt..."
chmod 600 level1/level2/level3/secret.txt

if [ $? -ne 0 ]; then
    echo "Ошибка: не удалось установить права доступа" >&2
    exit 1
fi

echo "Права доступа успешно установлены"

# Выводим полный путь к созданному файлу
echo "Полный путь к файлу secret.txt:"
realpath level1/level2/level3/secret.txt

# Выводим информацию о правах доступа
echo -e "\nИнформация о файле secret.txt:"
ls -la level1/level2/level3/secret.txt

echo -e "\n=== Скрипт успешно выполнен ==="

## Вывод программы:
Создан файл: /mnt/c/Users/SMD/Documents/GitHub/system-programming-labs/practice-1/1-3/level1/level2/level3/secret.txt
Права доступа:
-rwxrwxrwx 1 smd smd 148 Feb  5  2026 level1/level2/level3/secret.txt