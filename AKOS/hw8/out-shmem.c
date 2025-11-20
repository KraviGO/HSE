// out-shmem.c
// read from the shm every 1 second
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

int *terminate_flag;
void handler(int sig) {
  *terminate_flag = 1;
}

int main() {
  int shm_id;
  char gen_object[] = "gen-memory"; //  имя объекта

  signal(SIGINT, handler);

  //открыть объект
  if ( (shm_id = shm_open(gen_object, O_RDWR, 0666)) == -1 ) {
    printf("Opening error\n");
    perror("shm_open");
    return 1;
  } else {
    printf("Object is open: name = %s, id = 0x%x\n", gen_object, shm_id);
  }

  //получить доступ к памяти
  int* addr = mmap(0, sizeof(int) * 2, PROT_WRITE|PROT_READ, MAP_SHARED, shm_id, 0);
  if (addr == (int*)-1 ) {
    printf("Error getting pointer to shared memory\n");
    return 1;
  }

  terminate_flag = addr + 1;

  while(!(*terminate_flag)){
    sleep(1);
    printf("%d\n", *addr);
  }

  printf("Server exiting...\n");

  munmap(addr, sizeof(int)*2);
  close(shm_id);
  shm_unlink(gen_object);  

  return 0;
}
