#include "semaphone.h"
int raise_error(char *error, int use_errno){
  printf("ERROR! %s %s\n", error, use_errno ? strerror(errno) : "");
  return 1;
}
