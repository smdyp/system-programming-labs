#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>

#define PIDFILE "pidfile.txt"
#define LOGFILE "daemon.log"

static volatile sig_atomic_t running = 1;

void handle_signal(int signalNumber)
{
  (void)signalNumber;
  running = 0;
}

int write_log(const char *message)
{
  int fd;

  fd = open(LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (fd == -1)
  {
    return -1;
  }

  if (write(fd, message, strlen(message)) == -1)
  {
    close(fd);
    return -1;
  }

  if (close(fd) == -1)
  {
    return -1;
  }

  return 0;
}

void create_pidfile()
{
  int fd;
  char pid_str[16];
  int len;

  fd = open(PIDFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1)
  {
    write_log("ERROR: Failed to create pidfile\n");
    return;
  }

  len = snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
  
  if (write(fd, pid_str, len) != len)
  {
    write_log("ERROR: Failed to write pid to pidfile\n");
  }

  close(fd);
}

void remove_pidfile()
{
  unlink(PIDFILE);
}

int main(void)
{
  pid_t pid;
  struct sigaction sa;

  pid = fork();

  if (pid == -1)
  {
    perror("fork");
    return EXIT_FAILURE;
  }

  if (pid > 0)
  {
    printf("Демон запущен. PID: %d\n", pid);
    printf("Лог-файл: %s\n", LOGFILE);
    printf("PID-файл: %s\n", PIDFILE);
    printf("Для остановки демона используйте: kill -TERM %d\n", pid);
    return EXIT_SUCCESS;
  }

  if (setsid() == -1)
  {
    perror("setsid");
    return EXIT_FAILURE;
  }

  if (chdir("/") == -1)
  {
    perror("chdir");
    return EXIT_FAILURE;
  }

  umask(0);

  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_signal;

  if (sigaction(SIGTERM, &sa, NULL) == -1)
  {
    perror("sigaction");
    return EXIT_FAILURE;
  }

  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    perror("sigaction");
    return EXIT_FAILURE;
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  // Создание pid-файла
  create_pidfile();
  write_log("Демон запущен. PID-файл создан.\n");

  // Основной цикл работы демона
  while (running)
  {
    time_t now;
    struct tm *timeInfo;
    char buffer[100];

    now = time(NULL);
    timeInfo = localtime(&now);

    if (timeInfo != NULL)
    {
      strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] Демон выполняет работу...\n", timeInfo);
      write_log(buffer);
    }

    sleep(5);
  }

  // Завершение работы
  write_log("Получен сигнал завершения. Демон останавливается.\n");
  remove_pidfile();
  write_log("PID-файл удалён. Демон завершён.\n");

  return EXIT_SUCCESS;
}