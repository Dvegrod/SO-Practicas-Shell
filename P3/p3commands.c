#include "p3commands.h"

struct pelem {
  pid_t pid;
  int status;
  time_t time;
  const char ** cmd;
  int * sigorval;
};
//ESTO ES SUSCEPTIBLE A CAMBIOS

int buildPElem(iterator list,pid_t pid,const char *trozos[],int n) {
  struct pelem * elem = malloc(sizeof(struct pelem));
  elem->pid = pid;
  elem->status = 1;
  //TIME
  ctime(&elem->time);
  //CMD
  const char ** copytrozos = malloc(sizeof(char *) * n);
  for (int j = 0; j < n; j++) copytrozos[j] = trozos[j]; //PELIGRO
  elem->cmd = copytrozos;;
  //SIGNAL OR VALUE
  elem->sigorval = NULL;;
  InsertElement(list,elem);
  return 0;
};

int showElem(iterator list) {
  for (iterator i = first(&list); !isLast(i); i = next(i));
}

int freePElem(void * elem) {
  free(((struct pelem *) elem)->sigorval);
  free(elem);
}


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
      perror("Error getpriority en priority");
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
  if (trozos[1] == NULL) {
    fprintf(stderr,"No program specified");
    return -1;
  }
  if (trozos[1][0] == '@') { //Cuidao
    char pidt[10]; sprintf(pidt,"%i",getpid());
    const char * trzs[] = {NULL,pidt,&trozos[1][1]};
    priority(trzs,3,ex_inf);
  }
  execvp(trozos[1],&trozos[2]);//ojo al tipo del segundo arg
  perror("Error: exec failed");
  return -1;
}

int pplano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  pid_t ppid = getpid();//No me convence
  if (cmdfork(NULL,0,NULL) == -1) return -1;
  if (ppid == getpid()) return 0;
  return cmdexec(trozos,ntrozos,NULL);
}


//AUXILIAR
int spfork() {
  pid_t pidhijo;
  if ((pidhijo = fork())==-1){
    perror("Error: fork failed");
    return -1;
  }
  if (pidhijo!=0){
    return 1; //padre
  }
  else{
    return 0; //hijo
  }
}

int splano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  switch (spfork()) {
    case -1 : return -1;
    case 1  : return 0;
    case 0  : {
      //Incompleto
      cmdexec(trozos,ntrozos,NULL);
    }
  }
  return -1;
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
