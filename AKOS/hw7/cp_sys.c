/*
 * ФИО: Кравченко Игорь Александрович.
 * Группа: БПИ249.
 * Ожидаемая оценка: 8.
 */

// Опционально +1 балл.
// В коде реализованы два режима работы, переключаемые через аргумент командной строки:
// -m big — используется буфер больше размера файла.
// -m chunk32 — используется буфер 32 байта.

// Опционально +1 балл.
// Копирует все типы обычных файлов.
// Использует fstat() для получения прав доступа и fchmod() для их точного восстановления.
// Сохраняются исполняемые биты, что позволяет запускать копии бинарников и скриптов.
// Текстовые файлы не становятся исполняемыми, если исходные таковыми не были.


#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Функция вывода строки.
static void print_err(const char *msg) {
  write(STDERR_FILENO, msg, strlen(msg));
}

// Завершение программы с сообщением.
static void fail(const char *msg) {
  print_err("Ошибка: ");
  print_err(msg);
  print_err("\n");
  _exit(1);
}

// Режимы копирования. По умолчанию - MODE_32B.
typedef enum { MODE_32B, MODE_BIG } copy_mode_t;

// Определение режима работы программы.
static copy_mode_t get_mode(int *argc, char ***argv) {
  if (*argc >= 3 && strcmp((*argv)[1], "-m") == 0) {
    copy_mode_t mode;

    if (strcmp((*argv)[2], "big") == 0) {
      mode = MODE_BIG;
    }
    else if (strcmp((*argv)[2], "chunk32") == 0) {
      mode = MODE_32B;
    }
    else {
      fail("Неизвестный режим (используйте 'big' или 'chunk32').");
    }

    for (int i = 1; i + 2 < *argc; ++i) {
      (*argv)[i] = (*argv)[i + 2];
    }
    *argc -= 2;

    return mode;
  }

  return MODE_32B;
}

// Копирование одним большим буфером.
static void copy_big(int in_fd, int out_fd, off_t size) {
  if (size < 0) {
    fail("Неверный размер файла.");
  }

  size_t buf_size = (size_t)size + 1;
  char *buf = malloc(buf_size);

  if (!buf) {
    fail("Ошибка выделения памяти.");
    return;
  }

  ssize_t readed = 0;
  while (readed < size) {
    ssize_t r = read(in_fd, buf + readed, size - readed);

    if (r < 0) {
      free(buf);
      fail("Ошибка чтения файла.");
      return;
    }
    if (r == 0) {
      break;
    }

    readed += r;
  }

  ssize_t written = 0;
  while (written < readed) {
    ssize_t w = write(out_fd, buf + written, readed - written);

    if (w < 0) {
      free(buf);
      fail("Ошибка записи файла.");
      return;
    }

    written += w;
  }

  free(buf);
}

// Копирование через циклический буфер 32 байта.
static void copy_chunk32(int in_fd, int out_fd) {
  const size_t CHUNK = 32;
  char buf[CHUNK];
  ssize_t r;

  while ((r = read(in_fd, buf, CHUNK)) > 0) {
    ssize_t total = 0;
    while (total < r) {
      ssize_t w = write(out_fd, buf + total, r - total);

      if (w < 0) {
        fail("Ошибка записи.");
      }

      total += w;
    }
  }

  if (r < 0) {
    fail("Ошибка чтения.");
  }
}

// Копирование прав доступа.
static void copy_permissions(int out_fd, const struct stat *st) {
  if (fchmod(out_fd, st->st_mode & 07777) < 0) {
    fail("Ошибка установки прав доступа.");
  }
}

int main(int argc, char *argv[]) {
  // Определяем режим работы программы (big или chunk32).
  copy_mode_t mode = get_mode(&argc, &argv);

  // Проверяем количество аргументов.
  if (argc != 3) {
    print_err("Использование: cp_sys [-m big|chunk32] <SRC> <DST>\n");
    return 1;
  }

  // Пути к исходному и новому файлу.
  const char *src = argv[1];
  const char *dst = argv[2];

  // Открываем исходный файл только для чтения.
  int in_fd = open(src, O_RDONLY);
  if (in_fd < 0) fail("Не удалось открыть исходный файл.");

  // Получаем информацию о файле.
  struct stat st;
  if (fstat(in_fd, &st) < 0) {
    fail("Ошибка получения информации о файле.");
  }

  // Проверяем, что копируем обычный файл.
  if (!S_ISREG(st.st_mode)) {
    fail("Копирование только обычных файлов.");
  }

  // Создаём или перезаписываем файл назначения.
  int out_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (out_fd < 0) {
    fail("Не удалось создать файл назначения.");
  }

  // Копируем содержимое выбранным способом.
  if (mode == MODE_BIG) {
    copy_big(in_fd, out_fd, st.st_size);
  }
  else {
    copy_chunk32(in_fd, out_fd);
  }

  // Сбрасываем данные на диск.
  fsync(out_fd);

  // Сохраняем права доступа исходного файла.
  copy_permissions(out_fd, &st);

  // Закрываем оба файла.
  close(out_fd);
  close(in_fd);

  return 0;
}