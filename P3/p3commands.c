#include "p3commands.h"
#define TRUE_TO_ONE(x) (x) ? 1 : 0
#define PRUNNING 0
#define PSTOPPED 1
#define PTERM    2
#define PSIGN    4


struct pelem {
  pid_t pid;
  int status;
  struct tm * time;
  char ** cmd;
  int nargs;
  int * sigorval;
};

//ESTO ES SUSCEPTIBLE A CAMBIOS

int buildPElem(iterator list,pid_t pid,const char *trozos[],int n) {
  struct pelem * elem = malloc(sizeof(struct pelem));
  elem->pid = pid;
  elem->status = PRUNNING;
  //TIME
  time_t epch;
  ctime(&epch);
  elem->time = localtime(&epch); //Atentos al valgrind
  //CMD
  char ** copytrozos = malloc(sizeof(char *) * n);
  for (int j = 0; j < n; j++) {
    copytrozos[j] = malloc(sizeof(char) * strlen(trozos[j]));
    strncpy(copytrozos[j],trozos[j],MAXLEN);
  }
  elem->nargs = n;
  elem->cmd = copytrozos;;
  //SIGNAL OR VALUE PENDIENTE
  elem->sigorval = NULL;;
  InsertElement(list,elem);
  return 0;
};

int statusUpdate(struct pelem * e) {
  int status;
  int wpid = waitpid(e->pid,&status,WNOHANG | WUNTRACED | WCONTINUED);
  if (wpid == e->pid) {
    e->status = (TRUE_TO_ONE(WIFSIGNALED(status)) << 2)
               |(TRUE_TO_ONE(WIFEXITED(status))   << 1)
               |(TRUE_TO_ONE(WIFSTOPPED(status)));
    if (e->sigorval == NULL) e->sigorval = malloc(sizeof(int));

    switch (e->status) {
      case PTERM : {
        *e->sigorval = WEXITSTATUS(status);
        break;
      }
      case PSIGN : {
        *e->sigorval = WTERMSIG(status);
        break;
      }
      case PSTOPPED : {
        *e->sigorval = WSTOPSIG(status);
       break;
      }
    }
  }
  status = e->status;
  return status;
}

int showPElem(struct pelem * e) {
  //Status
  statusUpdate(e);
  //signal or value of return
  char sigorval[20];
  char * format = (e->status & PRUNNING) ? "\0" : (e->status & (PSTOPPED | PSIGN) ? "Signal: %i" : "Value: %i");
  sprintf(sigorval,format,*e->sigorval);
  //time
  char date[20];
  strftime(date,20,"%a %b %d %T %Y",e->time); //STATUS?
  printf(" Pid: %5i Status: %i Started: %s %s Command: ",
         e->pid,e->status,date,sigorval);
  for (int j = 0; j < e->nargs; j++) printf("%s ",e->cmd[j]);
  printf("\n");
  return 0;
}

int searchPElem(lista * list,pid_t pid,struct pelem ** pointer) {
  *pointer = NULL;
  for (iterator i = first(list); !isLast(i); i = next(i)) {
    struct pelem * elem = getElement(i);
    if (elem->pid == pid) {
      *pointer = elem;
      return 0;
    }
  }
  return 1; //Not found
};

int freePElem(void * elem) {
  struct pelem * e = elem;
  for (int i = 0; i < e->nargs; i++) free(e->cmd[i]);
  free(e->cmd);
  free(e->sigorval);
  free(elem);
  return 0;
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
    return 0;
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
  execvp(trozos[1],(char * const *)&trozos[2]);//ojo al tipo del segundo arg
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
    return pidhijo; //padre
  }
  else{
    return 0; //hijo
  }
}

int splano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  int pidhijo;
  switch (pidhijo = spfork()) {
    case -1 : return -1; //Caso error
    case 0  : { //Codigo del hijo
      return cmdexec(trozos,ntrozos,NULL);
    }
    default : { //Codigo del padre
      return buildPElem(&ex_inf->procesos,pidhijo,trozos,ntrozos);
    }
  }
}

int listarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  for (iterator i = first(&ex_inf->procesos); !isLast(i); i = next(i)) showPElem(getElement(i));
  return 0;
}

int cmdproc (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  if (ntrozos < 2) {
    return listarprocs(NULL,0,NULL);
  }
  else {
    pid_t pid = atoi(trozos[1]);
    struct pelem * elem;
    searchPElem(&ex_inf->procesos,pid,&elem);
    return showPElem(elem);
  }
}

int borrarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  if (!strcmp(trozos[1],"-term")){
    //mostrar procesos hijos que terminaron normalmente
    for(iterator i = first(&(ex_inf->procesos)); !isLast(i); i = next(i)){
      ;
    }
  }
  else if(!strcmp(trozos[1],"-sig")){
    //mostrar procesos hijos que terminaron por señal
    for(iterator i = first(&(ex_inf->procesos)); !isLast(i); i = next(i)){
      struct pelem * e = getElement(i);
      statusUpdate(e);
      if (e->status & PSIGN) showPElem(e);
    }
  }
  else{
    fprintf(stderr, "Error: invalid argument to borraprocs");
  }
  return 0;
}
