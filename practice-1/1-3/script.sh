#!/bin/bash

# Компактная версия скрипта

# Создание директорий
mkdir -p level1/level2/level3 || { echo "Ошибка создания директорий" >&2; exit 1; }

# Создание файла с содержимым
cat > level1/level2/level3/secret.txt << 'EOF'
Это секретный файл
Содержимое доступно только владельцу
Права доступа: 600 (rw-------)
EOF

# Установка прав доступа
chmod 600 level1/level2/level3/secret.txt || { echo "Ошибка установки прав" >&2; exit 1; }

# Вывод информации
echo "Создан файл: $(realpath level1/level2/level3/secret.txt)"
echo "Права доступа:"
ls -ld level1/level2/level3/secret.txt