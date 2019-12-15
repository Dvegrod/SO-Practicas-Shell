#include "p3commands.h"
#include <signal.h>
#define SUWAIT 1
#define SUNOWAIT 0
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

char* strstatus(int status){
  switch (status){
  case PRUNNING: return "Running";
  case PSTOPPED: return "Stopped";
  case PTERM : return "Terminated normally";
  case PSIGN : return "Terminated by signal";
  default : return "Undefined";
  }
}


int buildPElem(iterator list,pid_t pid,const char *trozos[],int n) {
  struct pelem * elem = malloc(sizeof(struct pelem));
  elem->pid = pid;
  elem->status = PRUNNING;
  //TIME
  time_t epch;
  time(&epch);
  elem->time = localtime(&epch);
  //CMD
  char ** copytrozos = malloc(sizeof(char *) * n);
  for (int j = 0; j < n; j++) {
      copytrozos[j] = malloc(sizeof(char) * strlen(trozos[j])+1);
      strncpy(copytrozos[j],trozos[j],strlen(trozos[j])+1);
  }
  elem->nargs = n;
  elem->cmd = copytrozos;;
  //SIGNAL OR VALUE PENDIENTE
  elem->sigorval = NULL;;
  InsertElement(list,elem);
  return 0;
};

int statusUpdate(struct pelem * e, int flag) {
  //PROBLEMA: El status siempre sale o running o stopped. Aunque el proceso haya acabado sigue saliendo STOPPED
  int status;
  int wpid = waitpid(e->pid,&status,flag ? 0 : WNOHANG | WUNTRACED | WCONTINUED);
  if (wpid == e->pid) {
    if (e->sigorval == NULL) e->sigorval = malloc(sizeof(int));

    if (WIFEXITED(status))   {e->status = PTERM;    *e->sigorval = WEXITSTATUS(status);}
    if (WIFSIGNALED(status)) {e->status = PSIGN;    *e->sigorval = WTERMSIG(status);}
    if (WIFSTOPPED(status))  {e->status = PSTOPPED; *e->sigorval = WSTOPSIG(status);}
    if (WIFCONTINUED(status)){e->status = PRUNNING; free(e->sigorval);}
  }
  status = e->status;
  return status;
}

int showPElem(struct pelem * e) {
  //Status
  statusUpdate(e,0);
  //signal or value of return
  char sigorval[30];
  if (e->status & (PSIGN | PSTOPPED))
    sprintf(sigorval,"Signal: %i",*e->sigorval);
  else
    if (e->status & PTERM)
      sprintf(sigorval,"Value: %i",*e->sigorval);
    else
      sprintf(sigorval,"\b");
  //time
  char date[20];
  strftime(date,20,"%a %b %d %T %Y",e->time); //STATUS?
  printf(" Pid: %5i | Status: %s | Started: %s %s | Command: ",
         e->pid,strstatus(e->status),date,sigorval);
  //command
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
  if (e->sigorval != NULL)
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
  pid_t pid = atoi(trozos[1]);

  int priority;
  if (trozos[2] == NULL) {
    if ((priority = getpriority(PRIO_PROCESS,pid)) == -1 && errno != 0) {
      perror("Error getpriority en priority");
      return -1;
    }
    printf("Nice value of process: %i is %i\n",pid,priority);
    return 0;
  }
  int niceoffset = atoi(trozos[2]);
  if (setpriority(PRIO_PROCESS,pid,niceoffset) == -1) {
    perror("Error setpriority en priority");
    return -1;
  }
  return 0;
};

//This function receives a string in the form of "@pri" and changes the process own priority
void chpri (const char * str){
  if (str[0]!='@') return; //If called with the wrong argument, does nothing
  int nicef = atoi(&str[1]);
  if (setpriority(PRIO_PROCESS,getpid(),nicef) == -1) {
    perror("Error setpriority en chpri");
  }
}

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
  if (ntrozos <= 1) {
    fprintf(stderr,"Error: No program specified\n");
    return -1;
  }
  trozos[ntrozos] = NULL;
  if (trozos[1][0] == '@') { //this means that trozos[1] has @pri
    chpri(trozos[1]); //changes priority
    execvp(trozos[2],(char * const *) (&trozos[2]));//ojo al tipo del segundo arg
    perror("Error: exec failed");
    exit(EXIT_FAILURE);
  }
  execvp(trozos[1],(char * const *) (&trozos[1]));
  perror("Error:exec failed");
  exit(EXIT_FAILURE);
}

int pplano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  pid_t ppid = getpid();//No me convence
  if (cmdfork(NULL,0,NULL) == -1) return -1;
  if (ppid == getpid()) return 0;
  return cmdexec(trozos,ntrozos,NULL);
}

/*
//Función que filtra el argumento @pri antes de llamar al código del comando pplano
int pplano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  if (trozos[1][0]=='@'){
    chpri(trozos[1]);
    for (int i = ntrozos-1; i > 0; i--) trozos[i] = trozos[i-1];
    return cmd_pplano(trozos, ntrozos-1, ex_inf);
  }
  return cmd_pplano(trozos,ntrozos,ex_inf);
}
*/

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

/*
//Función que filtra el argumento @pri antes de llamar al código del comando splano
int splano (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  if (trozos[1][0]=='@'){
    chpri(trozos[1]);
    for (int i = ntrozos-1; i > 0; i--) trozos[i] = trozos[i-1];
    return cmd_splano(trozos, ntrozos-1, ex_inf);
  }
  return cmd_splano(trozos, ntrozos, ex_inf);
}
*/

int listarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  for (iterator i = first(&ex_inf->procesos); !isLast(i); i = next(i)) showPElem(getElement(i));
  return 0;
}

int cmdproc (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  switch (ntrozos){
  case 1: //se llamó a proc sin argumentos
    return listarprocs(trozos,ntrozos,ex_inf);
  case 2:{ //se tiene un argumento
    if(!strcmp(trozos[1],"-fg")){ //el comando fue "proc -fg" sin especificar PID
      fprintf(stderr,"Error: no PID specified\n");
      return -1;
    }
    else{ //el comando fue "proc pid"
      pid_t pid = atoi(trozos[1]);

      if ((pid<0) || (pid == getpid())) return listarprocs(trozos,ntrozos,ex_inf);
      struct pelem * elem;
      if (searchPElem(&ex_inf->procesos, pid, &elem) == 1){
        return listarprocs(trozos,ntrozos,ex_inf); //pid is not a subprocess of the shell
      }
      return showPElem(elem);
    }
  }
  case 3:{ //el comango fue "proc -fg pid"
    if(!strcmp(trozos[1],"-fg")){
      pid_t pid = atoi(trozos[2]);

      struct pelem * elem;
      searchPElem(&ex_inf->procesos, pid, &elem);
      statusUpdate(elem,SUWAIT);
      /* showPElem(elem); */
      /* RemoveElement(&ex_inf->procesos,elem,freePElem); */
      return 0;
    }
    fprintf(stderr, "Error: Invalid flag specified\n");
    return -1;
  }
  default:
    fprintf(stderr, "Error: Not recognised");
    return -1;
  }
}


int borrarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf){
  if (trozos[1]==NULL){
    fprintf(stderr, "Error: No option specified (-term | -sig)\n");
    return -1;
  }  //SIGSEGV por probable acceso concurrente
  int option = PRUNNING;
  if (!strcmp(trozos[1],"-term")) option = PTERM;
  else
    if (!strcmp(trozos[1],"-sig")) option = PSIGN;
    else {
      fprintf(stderr,"Invalid argument: borrarprocs (-term | -sig)");
      return -1;
    }
  for(iterator i = first(&(ex_inf->procesos)); !isLast(i); i = next(i)){
    struct pelem * e = getElement(i);
    statusUpdate(e,SUNOWAIT);
    if (e->status == option) {
      RemoveElement(&ex_inf->procesos,e,freePElem);
      i = first(&ex_inf->procesos);
    }
  }
  return 0;
}

int direct_cmd (const char ** trozos, int ntrozos, struct extra_info *ex_inf){
  char *newtrozos[ntrozos+1];
  newtrozos[0] = malloc(sizeof(char)*2);
  newtrozos[0] = "\b";
  newtrozos[ntrozos] = NULL;
  if (trozos[ntrozos-1][0] == ('&')){
    for (int i = 1; i < ntrozos; i++)
      newtrozos[i] = (char *) trozos[i-1];
    return splano((const char **)newtrozos, ntrozos, ex_inf);
  }
  for (int i = 1; i <= ntrozos; i++)
    newtrozos[i] = (char *) trozos[i-1];
  return pplano((const char **)newtrozos, ntrozos+1, ex_inf);
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////   FUNCIONES DEL PDF    /////////////////////////////
/*

// The following code creates a child process that executes funcion2 while the
// parent executes funcion1. When the child has ended, the parent process
// executes funcion3

if ((pid=fork())==0) {
  funcion2();
  exit(0);
}
else {
  funcion1();
  waitpid(pid,NULL,0);
  funcion3();
}

// As exit() ends a program, we could rewrite it like this (without the else)

if ((pid=fork())==0) {
  funcion2();
  exit(0);
}
funcion1();
waitpid(pid,NULL,0);
funcion3();

//In this code both the parent process and the child process execute funcion3()

if ((pid=fork())==0)
  funcion2();
else
  funcion1();
funcion3();

// For a process to execute a program WE MUST USE the execvp() system
// call. execvp searches the executables in the directories specified in the PATH
// environment variable. execvp() only returns a value in case of error, otherwise
// it replaces the calling process’s code. Here you have an example using execl.

execl("/bin/ls","ls","-l","/usr",NULL);
funcion(); /*no se ejecuta a no ser que execl falle*/

/*
execvp operates the exactly the same but with two small differences

• it searches for executables in the PATH so, instead of specifying "/bin/ls" it would suffice to pass just "ls"

• we pass a NULL terminated array of pointers, instead of a variable number of pointers to the arguments

To check a process state we can use waitpid() with the following flags:

waitpid(pid, &estado, WNOHANG |WUNTRACED |WCONTINUED) will give us information about the state of process pid
in the variable estado ONLY WHEN THE RETURNED VALUE IS pid.
Such information can be evaluated with the macros descibed in man waitpid (WIFEXITED, WIFSIGNALED . . . )

The following functions allow us to obtain the signal name from the signal
number and viceversa. (in systems where we do not have sig2str or str2sig)

*/

/******************************SENALES ******************************************/
struct SEN{
  char *nombre;
  int senal;
};

static struct SEN sigstrnum[]={
    "HUP", SIGHUP,
    "INT", SIGINT,
    "QUIT", SIGQUIT,
    "ILL", SIGILL,
    "TRAP", SIGTRAP,
    "ABRT", SIGABRT,
    "IOT", SIGIOT,
    "BUS", SIGBUS,
    "FPE", SIGFPE,
    "KILL", SIGKILL,
    "USR1", SIGUSR1,
    "SEGV", SIGSEGV,
    "USR2", SIGUSR2,
    "PIPE", SIGPIPE,
    "ALRM", SIGALRM,
    "TERM", SIGTERM,
    "CHLD", SIGCHLD,
    "CONT", SIGCONT,
    "STOP", SIGSTOP,
    "TSTP", SIGTSTP,
    "TTIN", SIGTTIN,
    "TTOU", SIGTTOU,
    "URG", SIGURG,
    "XCPU", SIGXCPU,
    "XFSZ", SIGXFSZ,
    "VTALRM", SIGVTALRM,
    "PROF", SIGPROF,
    "WINCH", SIGWINCH,
    "IO", SIGIO,
    "SYS", SIGSYS,

/*senales que no hay en todas partes*/
    #ifdef SIGPOLL
      "POLL", SIGPOLL,
    #endif

    #ifdef SIGPWR
      "PWR", SIGPWR,
    #endif

    #ifdef SIGEMT
      "EMT", SIGEMT,
    #endif

    #ifdef SIGINFO
      "INFO", SIGINFO,
    #endif

    #ifdef SIGSTKFLT
      "STKFLT", SIGSTKFLT,
    #endif

    #ifdef SIGCLD
      "CLD", SIGCLD,
    #endif

    #ifdef SIGLOST
      "LOST", SIGLOST,
    #endif

    #ifdef SIGCANCEL
      "CANCEL", SIGCANCEL,
    #endif

    #ifdef SIGTHAW
      "THAW", SIGTHAW,
    #endif

    #ifdef SIGFREEZE
      "FREEZE", SIGFREEZE,
    #endif

    #ifdef SIGLWP
      "LWP", SIGLWP,
    #endif

    #ifdef SIGWAITING
      "WAITING", SIGWAITING,
    #endif

      NULL,-1,
    }; /*fin array sigstrnum */


int Senal(char * sen){ /*devuelve el numero de señal a partir del nombre*/
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
    if (!strcmp(sen, sigstrnum[i].nombre))
      return sigstrnum[i].senal;
  return -1;
}

char *NombreSenal(int sen){ /*devuelve el nombre senal a partir de la senal*/
  /* para sitios donde no hay sig2str*/
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
    if (sen==sigstrnum[i].senal)
      return sigstrnum[i].nombre;
return ("SIGUNKNOWN");
}