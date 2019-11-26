#include "p3commands.h"

struct pelem {
  pid_t pid;
  int status;
  time_t time;
  char ** cmd;
  int sigorval;
};

int buildPElem(iterator list,pid_t pid,const char *trozos[]);

int showElem(iterator list);

int freeElem(void *);


//------------------------------------------------------------------------

int priority (const char * trozos[], int ntrozos, struct extra_info *ex_inf) {
  errno = 0;
  if (trozos[1] == NULL) {
    fprintf(stderr,"Error: invalid arguments. priority pid [value]");
    return 0;
  }
  int pid = atoi(trozos[1]);
  if (trozos[2] == NULL)
    if (getpriority(PRIO_PROCESS,pid) == -1 && errno != 0) {
      perror("antonio");
      return -1;
    }
  int niceoffset = atoi(trozos[2]);
  if (setpriority(PRIO_PROCESS,pid,niceoffset) == -1) {
    perror("Error setpriority en priority");
    return -1;
  }
  return 0;
};

int cmdfork (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  pid_t pidhijo;
  if ((pidhijo = fork())==-1){
    perror("Error: fork failed");
    return -1;
  }
  if (pidhijo!=0){
    waitpid(pidhijo,NULL,0); //padre
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
