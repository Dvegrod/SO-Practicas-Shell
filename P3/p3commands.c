#include "p3commands.h"

int priority (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  return 0;
}

int cmdfork (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  pid_t pidhijo;
  if ((pidhijo = fork())==-1){
    perror("Error: fork failed");
    return -1;
  }
  if (pidhijo!=0){
    waitpid(pid,NULL,0); //padre
  }
  else{
    return 0; //hijo
  }
}

int cmdexec (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  execvp(trozos[1],&(trozos[2]));
  perror("Error: exec failed");
  return -1;
}

int pplano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  return 0;
}

int splano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  return 0;
}

int listarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  return 0;
}

int cmdproc (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  return 0;
}

int borrarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  return 0;
}
