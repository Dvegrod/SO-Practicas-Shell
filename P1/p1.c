#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
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
//Estructuras:
struct strfiledata {
  char * inodenum,*filetype,*permissions,*hlinksnum,*user,*group,*size,*date,*name;
};

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

// Funciones de utilidad dadas en el enunciado:

char TipoFichero (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * ConvierteModo (mode_t m, char *permisos){
    strcpy (permisos,"---------- ");
    permisos[0]=TipoFichero(m);
    if (m&S_IRUSR) permisos[1]='r'; /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r'; /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return permisos;
}

char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    permisos[0]=TipoFichero(m);
    if (m&S_IRUSR) permisos[1]='r'; /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r'; /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return (permisos);
}

char * ConvierteModo3 (mode_t m){
    char * permisos;
    permisos=(char *) malloc (12);
    strcpy (permisos,"---------- ");
    permisos[0]=TipoFichero(m);
    if (m&S_IRUSR) permisos[1]='r'; /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r'; /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return (permisos);
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

struct strfiledata getInfo(const char *path){
  struct strfiledata strfinfo;
  struct stat *finfo = NULL;
  if (lstat(path,finfo) < 0) printf("%s",strerror(errno)); //Syscall
  //INODE NUMBER
  sprintf(strfinfo.inodenum,"%lu",finfo->st_ino);
  //TYPE
  *strfinfo.filetype = TipoFichero(finfo->st_mode);
  //PERMISSIONS
  strfinfo.permissions = ConvierteModo2(finfo->st_mode);
  //HARD LINK NUMBER
  sprintf(strfinfo.hlinksnum,"%lu",finfo->st_nlink);
  //USER
  strfinfo.user = getpwuid(finfo->st_uid)->pw_name;
  //GROUP
  strfinfo.group = getgrgid(finfo->st_gid)->gr_name;
  //SIZE
  sprintf(strfinfo.size,"%lu",finfo->st_size);
  //DATE FORMAT
  struct tm * mtime = localtime(&finfo->st_mtime);
  strftime(strfinfo.date,20,"%b %d %T",mtime);
  //FILE NAME: cuts the rest of the path
  int i = 0,j = 0;
  while (path[i] != '\0') if (path[i] == '/') j = i + 1;
  sprintf(strfinfo.name,"%s",&path[j]); //File name
  return strfinfo;
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
