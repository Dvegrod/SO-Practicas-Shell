#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXLEN 255

//Lista:

struct node {
   char com[MAXLEN];
   struct node *next;
};

struct node * lista;

void CreateList(struct node **plist){
    *plist = NULL;
}

void InsertElement(struct node **plist, char element[]) {
  struct node **last = plist;
  while (*last != NULL) { //Looks for the last pointer
    last = &(*last)->next;
  };
  *last = (struct node *) malloc(sizeof(struct node)); //Memory request for the new node
  int i;
  for (i = 0; (element[i] != '\0') && (i < MAXLEN); i++) //Saving command
    (*last)->com[i] = element[i];
  (*last)->com[i-1] = '\0'; //Substitutes the \n at the end of stored commands by \0
  (*last)->next = NULL;
};

int RemoveElement(struct node **plist, int position){
  if (*plist == NULL) return -1;

  struct node **pointertopointer = plist; //To aim to the pointer in the list that aims to the node to be removed
  for (int i = 0; i < position; i++) {
    if (*pointertopointer == NULL) return -1;  //Index out of bounds
    pointertopointer = &(*pointertopointer)->next;
  }
  struct node *auxiliar = *pointertopointer;  //Auxiliar stores the memory address of the node to be removed
  *pointertopointer = (*pointertopointer)->next; //The pointer in the list that aims to the expiring node now aims to the node after it (sometimes NULL)
  free(auxiliar);
  return 0;
};

//Shell:

//Funciones para leer los comandos de la Shell

struct node *lista;

void printPrompt(){
    printf("# ");
}

void readInput(char comando[]){
    fgets(comando,MAXLEN,stdin);
    InsertElement(&lista, comando);
}

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

// Funciones para las tareas de cada comando de la shell

void autores(char * opcion){
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
            printf("%s : unrecognised command option\n",opcion);
        }
    }
}

void pid(char *opcion){
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
            printf("%s : unrecognised command option\n", opcion);
        }
    }
}

void cdir(char * opcion){
    char buf[MAXLEN];
    if (opcion == NULL){
        printf("%s\n",getcwd(buf,MAXLEN));
    }
    else
    {
        if (chdir(opcion)){ //if chdir() returns -1 it means an error ocurred
            printf("Error: %s directory does not exist\n",opcion);
        }
    }
}

void fecha_hora(char f_h, char * opcion){
    time_t now; //time type variable
    time(&now); //writes current time to NOW variable
    struct tm *local = localtime(&now); //a structure to separate minutes, hours, day, month, year

    if (opcion == NULL){
    if (f_h == 'f'){
        printf("Today is %1.2d/%1.2d/%d\n",local->tm_mday, local->tm_mon+1, local->tm_year+1900);
    }
    else if (f_h == 'h'){
        printf("Right now it is %d:%d:%d\n",local->tm_hour,local->tm_min,local->tm_sec);
    }
    }
    else
    {
        printf(" %s : unrecognised command option\n",opcion);
    }
}

void hist(char * opcion){
  if (opcion != NULL) { //if there IS an option
    if (!strcmp(opcion,"-c")){ //if that option is "-c"
      while (lista != NULL) RemoveElement(&lista,0);
      printf("Cleared command history\n");
    }else printf("%s : unrecognised command option\n",opcion);
  }
  else {
    struct node * pointer = lista;
    for (int i = 1; pointer != NULL; i++) {
      printf("%d > %s\n",i,pointer->com);
      pointer = pointer->next;
    };
  }
}

void processInput(char comando[], int * salir){
    char *trozos[5]; //This array is of length 5 instead of 2 to prevent the program from crashing in case some user enters a third command option
                    // this third option will be always ignored
    int nopc = TrocearCadena(comando, trozos);

    if (nopc > 0){
        if (!strcmp(trozos[0],"autores")){
                autores(trozos[1]);
        }
        else if (!strcmp(trozos[0],"pid"))
        {
            pid(trozos[1]);
        }
        else if (!strcmp(trozos[0],"cdir"))
        {
            cdir(trozos[1]);
        }
        else if (!strcmp(trozos[0],"fecha")){
            fecha_hora('f',trozos[1]);
        }
        else if (!strcmp(trozos[0],"hora")){
            fecha_hora('h',trozos[1]);
        }
        else if (!strcmp(trozos[0],"hist"))
        {
            hist(trozos[1]);
        }
        else if (!strcmp(trozos[0],"fin") || !strcmp(trozos[0],"end") || !strcmp(trozos[0], "exit")){
            *salir = 1;
        }
        else
        {
            printf("%s : unrecognised command\n",trozos[0]);
        }
    }
}

void disposeAll(struct node **plist) {
  while (RemoveElement(plist,0) == 0);
}

int main(int argc, char const *argv[])
{
    int salir = 0;
    char comando[MAXLEN];
    while(!salir){
        printPrompt();
        readInput(comando);
        processInput(comando, &salir);
    }
    disposeAll(&lista);
    return 0;
}
