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
  struct node *last = *plist;
  while (last != NULL) {
    last = last->next;
  };
  last = (struct node *) malloc(sizeof(struct node));
  for (int i = 0; (element[i] != '\0') && (i < MAXLEN); i++)
    last->com[i] = element[i];
  last->next = NULL;
};

int RemoveElement(struct node **plist, int position){
  struct node **pointertopointer = plist; //To aim the pointer in the list that aims the node to be removed
  for (int i = 0; i < position; i++) {
    pointertopointer = &(**pointertopointer).next;
    if (*pointertopointer == NULL) return -1;  //Index out of bounds
  }
  struct node *auxiliar = *pointertopointer;
  *pointertopointer = (**pointertopointer).next;
  free(auxiliar);
  return 0;
};
//  ->(*LIST)->(NODE(*))->(NODE(*))->(NODE(*))

//Shell:

//Funciones para leer los comandos de la Shell

struct node *lista;

void printPrompt(){
    printf(" # ");
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
    if (strcmp(opcion,"-l")){
        printf("carlos.torres \n");
        printf("d.s.vega \n");
    }
    else if (strcmp(opcion, "-n"))
    {
        printf("Carlos Torres Paz\n");
        printf("Daniel Sergio Vega Rodríguez\n");
    }
    else
    {
        printf("Carlos Torres Paz : carlos.torres");
        printf("Daniel Sergio Vega Rodríguez : d.s.vega");
    }
}

void pid(char * opcion){
    if (strcmp(opcion, "-p"))
    {
        printf("PID of parent process: %i\n", getppid());
    }else
    {
        printf("PID of Shell: %i\n", getpid());
    }   
}

void cdir(char * opcion){
    char buf[MAXLEN];
    if (strlen(opcion)==0){
        printf("%s\n",getcwd(buf,MAXLEN));
    }else
    {
        chdir(opcion);
    }
}

void fecha_hora(char f_h){
    time_t now; //time type variable
    time(&now); //writes current time to NOW variable
    struct tm *local = localtime(&now); //a structure to separate minutes, hours, day, month, year

    if (f_h = 'f'){
        printf("Today is %d %d, %d",local->tm_yday, local->tm_mon+1, local->tm_year+1900);
    }
    else if (f_h = 'h'){
        printf("Right now it is %d:%d:%d",local->tm_hour,local->tm_min,local->tm_sec);
    }
}

void hist(char * opcion){
    if (strcmp(opcion,"-c")){
        // vaciar historial
    }else{
        // mostrar historial
    }
}

void processInput(char comando[], int * salir){
    char *trozos[2];
    TrocearCadena(comando, trozos);

    if (strcmp(trozos[0],"autores")){
        autores(trozos[1]);        
    }
    else if (strcmp(trozos[0],"pid"))
    {
        pid(trozos[1]);
    }
    else if (strcmp(trozos[0],"cdir"))
    {
        cdir(trozos[1]);
    }
    else if (strcmp(trozos[0],"fecha")){
        fecha_hora('f');
    }
    else if (strcmp(trozos[0],"hora")){
        fecha_hora('h');
    }
    else if (strcmp(trozos[0],"hist"))
    {
        hist(trozos[1]);
    }
    else if (strcmp(trozos[0],"fin") || strcmp(trozos[0],"end") || strcmp(trozos[0], "exit")){
        *salir = 1;
    }
    else
    {
        printf("Comando no válido\n");
    }
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
    return 0;
}