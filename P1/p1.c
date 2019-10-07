#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXLEN 256 //maximum length of strings (arrays of 256 chars)
#define MAX_N_ARG 32 //maximum number of arguments to a shell command

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
}

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
}

//Shell:

//Funciones para leer los comandos de la Shell

struct node *lista;

void printPrompt(){
    printf("# ");
}

void readInput(char comando[]){
    fgets(comando,MAXLEN,stdin);
    if (comando[0] != '\n')
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

void autores(const char * opcion){
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

void pid(const char *opcion){
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

void cdir(const char * opcion){
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

void fecha_hora(const char f_h){
    time_t now; //time type variable
    time(&now); //writes current time to NOW variable
    struct tm *local = localtime(&now); //a structure to separate minutes, hours, day, month, year

    if (f_h == 'f'){
        printf("Today is %1.2d/%1.2d/%d\n",local->tm_mday, local->tm_mon+1, local->tm_year+1900);
    }
    else if (f_h == 'h'){
        printf("Right now it is %d:%d:%d\n",local->tm_hour,local->tm_min,local->tm_sec);
    }
}

void disposeAll(struct node **plist) {
  while (RemoveElement(plist,0) == 0);
}

void hist(const char * opcion){
  if (opcion != NULL) { //if there IS an option
    if (!strcmp(opcion,"-c")){ //if that option is "-c"
      disposeAll(&lista);
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

void crear(const char * trozos[]){
    
}

void borrar(const char * trozos[]){

}

char ** getInfo(const char *path){
    char ** strfileinfo = (char **) malloc(12 * sizeof(char *));
}

void info(const char * trozos[]){

}

void listar(const char * trozos[]){

}

//Función para decidir qué comando se va a ejecutar

void processInput(char comando[], int * salir){
    char *trozos[MAX_N_ARG];
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
            fecha_hora('f');
        }
        else if (!strcmp(trozos[0],"hora")){
            fecha_hora('h');
        }
        else if (!strcmp(trozos[0],"hist")){
            hist(trozos[1]);
        }
        else if (!strcmp(trozos[0],"crear")){
            crear(trozos);
        }
        else if (!strcmp(trozos[0],"borrar")){
            borrar(trozos);
        }
        else if (!strcmp(trozos[0], "info")){
            info(trozos);
        }
        else if (!strcmp(trozos[0],"listar")){
            listar(trozos);
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

int processInput(char comando[], struct extra_inf *ex_inf){
    struct{
        char * cmd_name;
        int (* cmd_fun) (const char * trozos[], int ntrozos, struct extra_inf *ex_inf)
    } cmds[] = {
        {"autores", autores},
        {"pid", pid},
        {"cdir", cdir},
        {"fecha", fecha},
        {"hora", hora},
        {"hist", hist},
        {"crear", crear},
        {"borrar", borrar},
        {"info", info},
        {"listar", listar},
        {"fin", salir},
        {"end", salir},
        {"exit", salir},
        {NULL, NULL}
    };
    
    for (int i = 0; cmds[i].cmd_name != NULL; i++)
    {
        if (!strcmp(trozos[0],cmds[i].cmd_name)){
            cmds[i].cmd_fun (trozos, ntrozos, ex_inf);
            break;
        } //if
    } //for
    if (cmds[i].cmd_name == NULL){ //cmd not found
        printf("cmd not found");
    } //cmd not found
}

int main(int argc, char const *argv[]){
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
