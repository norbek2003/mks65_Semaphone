#include "semaphone.h"

int control_create(){
  int semaphore_id = semget(KEY, 1, IPC_CREAT | 0644);
  union semun s;
  s.val = 1;
  if(semaphore_id < 0){
    return raise_error("Couldn't open semaphore", 1);
  }
  printf("Created the semaphore!\n");
  if(semctl(semaphore_id, 0, SETVAL, s) < 0){
    return raise_error("Couldn't set semaphore", 1);
  }
  int shared_mem_id = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
  if(shared_mem_id < 0){
    return raise_error("Couldn't open/create shared memory", 1);
  }
  printf("Created the shared memory!\n");
  if(open(FILENAME, O_CREAT | O_TRUNC | O_RDWR, 0644) < 0){
    return raise_error("Couldn't open/create file", 1);
  }
  printf("File created!\n");
  return 0;
}
int control_view(){
  char readBuffer[1];
  int fd = open(FILENAME, O_RDONLY);
  if(fd < 0){
    return raise_error("File hasn't been created yet. Try ./control -c", 0);
  }
  printf("Thus far, the story goes like this...\n");
  while(read(fd, readBuffer, 1) > 0 ){
    printf("%s", readBuffer);
  }
  return 0;
}
int control_remove(){
  int semaphore_id = semget(KEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  if(semaphore_id < 0){
    return raise_error("Couldn't open semaphore", 1);
  }
  printf("Waiting for semaphore to open ...\n");
  semop(semaphore_id, &sb, 1);
  printf("Your turn to play now!\n");
  int shared_mem_id = shmget(KEY, SEG_SIZE, 0);  
  semctl(semaphore_id, IPC_RMID, 0);
  shmctl(shared_mem_id, IPC_RMID, 0);
  printf("---------------------\n")
  control_view();
  printf("THE END!\n");
  printf("---------------------\n")
  printf("Shared memory and semaphore removed\n");
  if(remove(FILENAME)){
    return raise_error("Tried to remove the file", 1);
  }
  printf("Removed the file\n");
  return 0;
}

void help(){
  printf("-c Creates the shared memory, semaphore, and file.\n");
  printf("-r Removes the shared memory, semaphore, and file. Displays the story.\n");
  printf("-v Displays the story\n");
  printf("-h Displays this help message\n");
}
int main(int argc, char * argv[]){
  if(argc < 2){
    return raise_error("No flag given", 0);
  }
  char arg = argv[1][1];
  switch(arg){
  case 'c':
    return control_create();
  case 'r':
    return control_remove();
  case 'v':
    return control_view();
  case 'h':
    help();
    break;
  default:
    printf("Please use a valid flag\n");
    help();
    break;
  }
  return 0;
}
