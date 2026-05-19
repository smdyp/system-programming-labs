Практика 4: Память, отображение файлов, демонизация
Задание №3: Демоны и фоновые процессы

Реализовать демона, который выполняет фоновую работу и корректно завершается по сигналу.

Вариант задания
Демон создаёт файл pidfile.txt со своим PID и удаляет его при завершении.

Особенность реализации
Программа демонстрирует стандартную процедуру создания демона в Unix/Linux: отделение от терминала, создание новой сессии, смена рабочей директории, сброс маски прав доступа, закрытие ненужных файловых дескрипторов. Демон обрабатывает сигналы SIGTERM и SIGINT для корректного завершения и удаления pid-файла.

Используемые системные вызовы
В программе используются:

fork() — создание дочернего процесса;

setsid() — создание новой сессии;

chdir() — изменение рабочей директории;

umask() — установка маски прав доступа;

close() — закрытие файловых дескрипторов;

open() — создание pid-файла;

write() — запись PID в файл;

unlink() — удаление pid-файла;

signal() — установка обработчиков сигналов;

pause() — ожидание сигнала;

sleep() — периодическое выполнение действий.

Файл программы
Основной файл программы:

text
task.c
Компиляция
Для компиляции выполнить команду:

bash
gcc -Wall -Wextra -o task task.c
Запуск
Запустить демона:

bash
./task
Остановить демона:

bash
kill -TERM <PID>
# или
kill -INT <PID>
Ожидаемый результат
При запуске демона:

text
Демон запущен. PID: 12345
PID записан в файл pidfile.txt
Демон работает в фоне. Для остановки используйте: kill -TERM 12345
Содержимое pidfile.txt после запуска:

text
12345
При остановке демона в лог-файле:

text
[2026-05-19 12:30:00] Демон запущен. PID: 12345
[2026-05-19 12:30:05] Демон выполняет работу...
[2026-05-19 12:30:10] Демон выполняет работу...
[2026-05-19 12:30:15] Получен сигнал завершения. Демон останавливается.
[2026-05-19 12:30:15] PID-файл удалён.
Полный листинг программы (task.c)

c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#define PIDFILE "pidfile.txt"
#define LOGFILE "daemon.log"
#define WORK_INTERVAL 5

volatile sig_atomic_t running = 1;

void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        running = 0;
    }
}

void write_log(const char *message) {
    FILE *log = fopen(LOGFILE, "a");
    if (log != NULL) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char timestamp[20];
        strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(log, "[%s] %s\n", timestamp, message);
        fclose(log);
    }
}

void daemonize() {
    pid_t pid;
    
    // Создание дочернего процесса
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // Родительский процесс завершается
        exit(EXIT_SUCCESS);
    }
    
    // Создание новой сессии
    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    
    // Игнорирование сигнала SIGHUP
    signal(SIGHUP, SIG_IGN);
    
    // Второй fork для гарантии от получения терминала
    pid = fork();
    if (pid < 0) {
        perror("second fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    
    // Смена рабочей директории
    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    
    // Установка маски прав доступа
    umask(0);
    
    // Закрытие файловых дескрипторов
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void create_pidfile() {
    int fd = open(PIDFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open pidfile");
        exit(EXIT_FAILURE);
    }
    
    char pid_str[16];
    int len = snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    
    if (write(fd, pid_str, len) != len) {
        perror("write pidfile");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);
}

void remove_pidfile() {
    unlink(PIDFILE);
}

int main() {
    // Установка обработчиков сигналов
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    // Демонизация
    daemonize();
    
    // Создание pid-файла
    create_pidfile();
    write_log("Демон запущен. PID записан в pidfile.txt");
    
    // Основной цикл работы демона
    while (running) {
        write_log("Демон выполняет работу...");
        sleep(WORK_INTERVAL);
    }
    
    // Завершение работы
    write_log("Получен сигнал завершения. Демон останавливается.");
    remove_pidfile();
    write_log("PID-файл удалён. Демон завершён.");
    
    return 0;
}
Краткое описание работы

Устанавливаются обработчики сигналов SIGTERM и SIGINT.

Выполняется демонизация (двойной fork, setsid, chdir, umask, закрытие fd).

Создаётся pid-файл с PID демона.

Запускается основной цикл с периодической записью в лог.

При получении сигнала завершения выставляется флаг running = 0.

Цикл завершается, демон удаляет pid-файл и записывает сообщение в лог.

Программа завершается.

