#include "semaphone.h"
int main(int argc, char * argv[]){
  int semaphore_id = semget(KEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  if(semaphore_id < 0){
    return raise_error("Couldn't open semaphore.", 1);
  }
  printf("Waiting for semaphore to open ...\n");
  semop(semaphore_id, &sb, 1);
  printf("Your turn to play now!\n");
  int shared_mem_id = shmget(KEY, SEG_SIZE, 0);
  char buffer[SEG_SIZE];
  char * last;
  last = shmat(shared_mem_id, 0, 0);
  printf("Last Addition: %s\n", last);
  printf("Your Addition:\n->");
  fgets(last, SEG_SIZE, stdin);
  strcpy(buffer, last);
  *strchr(last, '\n') = 0;
  int fd = open(FILENAME, O_WRONLY | O_APPEND);
  write(fd, buffer, strlen(buffer));
  sb.sem_op = 1;
  semop(semaphore_id, &sb, 1);
  shmdt(last);
  return 0;
}
