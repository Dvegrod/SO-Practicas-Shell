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
//Estructuras:
struct node {
  char com[MAXLEN];
  struct node *next;
};

typedef struct extra_info {
  struct node * lista;
} EXINF;

struct strfiledata {
  char * inodenum,*filetype,*permissions,*hlinksnum,*user,*group,*size,*date,*name;
};

//Lista:

struct node * CreateList(){
    return NULL;
}

void InsertElement(struct node ** last, char *element){
    int i;
    while (*last != NULL){ //'last' points to the last node
        last = &((*last)->next); //moves one node forward
    }
    *last = malloc(sizeof(struct node)); //allocates memory for the new node
    (*last)->next = NULL; //sets to null the new end of the list
    for (i = 0; (element[i] != '\0') && (i < MAXLEN); i++)
    {
        (*last)->com[i] = element[i];
    }
    (*last)->com[i-1] = '\0'; //replaces the endline character with the end of string flag
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

void disposeAll(struct node ** ptolist) {
  // while (RemoveElement(ptolist,0) == 0);
  struct node * aux;
  while (*ptolist != NULL)
  {
      aux = *ptolist;
      *ptolist = aux->next;
      free(aux);
  }
}

//Shell:

//Funciones para leer los comandos de la Shell

void printPrompt(){
    printf("# ");
}

int readInput(char comando[], struct extra_info * ex_inf){
    fgets(comando,MAXLEN,stdin);
    if (comando[0]!='\n')
        InsertElement(&(ex_inf->lista), comando);
    return 0;
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
    switch (m & S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
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

int crear(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    return 0;
}

int borrar(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    return 0;
}

struct strfiledata getInfo(char *path){
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

int info(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    return 0;
}

int listar(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    return 0;
}

int salir(char *trozos[], int ntrozos, struct extra_info *ex_inf){
    return 1;
}

//Función para decidir qué comando se va a ejecutar

int processInput(char comando[], struct extra_info *ex_inf){
    char *trozos[MAX_N_ARG];
    int ntrozos = TrocearCadena(comando,trozos);
    int i = 0;
    int return_value = 0;

    struct {
        char * cmd_name;
        int (* cmd_fun) (char * trozos[], int ntrozos, struct extra_info *ex_inf);
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

    if (trozos[0]==NULL){ //in case nothing is entered
        return 0;
    }
    
    for (i = 0; cmds[i].cmd_name != NULL; i++)
    {
        if (!strcmp(trozos[0],cmds[i].cmd_name)){
            return_value = cmds[i].cmd_fun (trozos, ntrozos, ex_inf);
            break;
        } //if
    } //for
    if (cmds[i].cmd_name == NULL){ //cmd not found
        printf(" %s : unrecognised command\n", trozos[0]);
        return -1;
    } //cmd not found
    return return_value;
}

int main(int argc, char const *argv[]){
    char comando[MAXLEN];

    struct extra_info * ex_inf;
    ex_inf = (struct extra_info *) malloc(sizeof(struct extra_info));

    ex_inf->lista = CreateList();

    do{
        printPrompt();
        readInput(comando, ex_inf);
    } while(processInput(comando, ex_inf) != 1);

    disposeAll(&(ex_inf->lista));
    return 0;
}
