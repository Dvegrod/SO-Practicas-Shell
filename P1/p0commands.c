
#include "./p0commands.h"

int autores(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    char *opcion = trozos[1];
    if (opcion == NULL){ //If there is no option
        printf("Carlos Torres Paz : carlos.torres\n");
        printf("Daniel Sergio Vega Rodríguez : d.s.vega\n");
    }
    else
    {
        if (!strcmp(opcion,"-n"))
        {
            printf("Carlos Torres Paz\n");
            printf("Daniel Sergio Vega Rodríguez\n");
        }
        else if (!strcmp(opcion, "-l"))
        {
            printf("carlos.torres\n");
            printf("d.s.vega\n");
        }
        else
        {
            printf(" %s : unrecognised command option\n",opcion);
        }
    }
    return 0;
}

int pid(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    char *opcion = trozos[1];
    if (opcion==NULL)
    {
        printf("PID of Shell: %i\n", getpid());
    }
    else
    {
        if (!strcmp(opcion,"-p"))
        {
            printf("PID of parent process: %i\n", getppid());
        }
        else{
            printf(" %s : unrecognised command option\n", opcion);
            return -1;
        }
    }
    return 0;
}

int cdir(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    char *opcion = trozos[1];
    char buf[MAXLEN];
    if (opcion == NULL){
        printf("%s\n",getcwd(buf,MAXLEN));
    }
    else
    {
        if (chdir(opcion)){ //if chdir() returns -1 it means an error ocurred
            printf("Error: %s directory does not exist\n",opcion);
            return -1;
        }
    }
    return 0;
}

int fecha(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    time_t now; //time type variable
    time(&now); //writes current time to NOW variable
    struct tm *local = localtime(&now); //a structure to separate minutes, hours, day, month, year
    printf("Today is %1.2d/%1.2d/%d\n",local->tm_mday, local->tm_mon+1, local->tm_year+1900);
    return 0;
}

int hora(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    time_t now; //time type variable
    time(&now); //writes current time to NOW variable
    struct tm *local = localtime(&now); //a structure to separate minutes, hours, day, month, year
    printf("Right now it is %d:%d:%d\n",local->tm_hour,local->tm_min,local->tm_sec);
    return 0;
}

int hist(char * trozos[], int ntrozos, struct extra_info *ex_inf){
  char *opcion = trozos[1];
  if (opcion != NULL) { //if there IS an option
    if (!strcmp(opcion,"-c")){ //if that option is "-c"
        disposeAll(&(ex_inf->lista));
        printf("Cleared command history\n");
    }
    else
    {
        printf(" %s : unrecognised command option\n",opcion);
        return -1;
    }
  }
  else { //if there is NO option
    struct node *pointer = (ex_inf->lista);
    for (int i = 1; pointer != NULL; i++) {
      printf("%d > %s\n",i,pointer->com);
      pointer = pointer->next;
    }
  }
  return 0;
}
