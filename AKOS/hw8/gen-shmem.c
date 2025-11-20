// gen-shmem.c
// write a random number between 0 and 999 to the shm every 1 second
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

int *terminate_flag;
void handler(int sig) {
  *terminate_flag = 1;
}

int main(){
  int shm_id;
  char gen_object[] = "gen-memory"; //  имя объекта
  int num;

  signal(SIGINT, handler);

  if ( (shm_id = shm_open(gen_object, O_CREAT|O_RDWR, 0666)) == -1 ) {
    perror("shm_open");
    return 1;
  } else {
    printf("Object is open: name = %s, id = 0x%x\n", gen_object, shm_id);
  }
  // Задание размера объекта памяти
  if (ftruncate(shm_id, sizeof (num) * 2) == -1) {
    perror("ftruncate");
    return 1;
  } else {
    printf("Memory size set and = %lu\n", sizeof (num));
  }

  srand(time(NULL));
  //получить доступ к памяти
  int* addr = mmap(0, sizeof(num), PROT_WRITE|PROT_READ, MAP_SHARED, shm_id, 0);
  if (addr == (int*)-1 ) {
    printf("Error getting pointer to shared memory\n");
    return 1;
  }

  terminate_flag = addr + 1;
  *terminate_flag = 0;

  srandom(time(NULL));

  while(!(*terminate_flag)){
    num = random() % 1000;
    *addr = num;
    printf("write a random number %d\n", num);
    sleep(1);
  }

  printf("Client exiting...\n");

  munmap(addr, sizeof(int) * 2);
  close(shm_id);
  shm_unlink(gen_object);

  return 0;
}
