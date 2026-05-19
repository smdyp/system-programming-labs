#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static volatile sig_atomic_t running = 1;

void handle_signal(int signalNumber)
{
  (void)signalNumber;
  running = 0;
}

int main(void)
{
  int *numbers;
  int *extraNumbers;
  int *newNumbers;
  int *sum;
  int i;

  signal(SIGTERM, handle_signal);
  signal(SIGINT, handle_signal);

  numbers = malloc(100 * sizeof(int));
  if (numbers == NULL)
  {
    perror("malloc");
    return EXIT_FAILURE;
  }

  extraNumbers = calloc(50, sizeof(int));
  if (extraNumbers == NULL)
  {
    perror("calloc");
    free(numbers);
    return EXIT_FAILURE;
  }

  newNumbers = realloc(numbers, 200 * sizeof(int));
  if (newNumbers == NULL)
  {
    perror("realloc");
    free(numbers);
    free(extraNumbers);
    return EXIT_FAILURE;
  }

  numbers = newNumbers;

  sum = malloc(sizeof(int));
  if (sum == NULL)
  {
    perror("malloc");
    free(numbers);
    free(extraNumbers);
    return EXIT_FAILURE;
  }

  *sum = 0;

  for (i = 0; i < 200; i++)
  {
    numbers[i] = i + 1;
    *sum += numbers[i];
  }

  printf("PID: %d\n", getpid());
  printf("Memory allocated\n");
  printf("Sum: %d\n", *sum);
  printf("Program is running. Use ps, pmap, top or /proc to analyze it.\n");

  while (running)
  {
    sleep(1);
  }

  free(numbers);
  free(extraNumbers);
  free(sum);

  printf("Program stopped\n");

  return EXIT_SUCCESS;
}