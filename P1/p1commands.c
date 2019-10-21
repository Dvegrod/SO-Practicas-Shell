#include "./p1commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>

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

int reclisting(char * path,unsigned int options,int reclevel);

int crear(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    int fd;
    if (!strcmp(trozos[1],"-d")){ //creates a directory
        if (trozos[2] == NULL){
            reclisting(".", LIST_NVRB, 0);
            return 0;
        }
        else{
            if (!mkdir(trozos[2], S_IRWXU | S_IRWXG)){ //creates directory
                printf(" Directory %s has been created\n", trozos[2]);
                return 0;
            }
            else{
                if (errno == EEXIST){
                    printf(" Error: File or directory %s already exists\n", trozos[2]); //file already exists
                    return -1;
                }
                else{
                    printf(" Error: Directory %s could not be created\n", trozos[2]); //other errors
                    return -1;
                }
            }//else
        }//else
    }//if 
    else if ((trozos[1][0] == '-') && (trozos[1][0] != 'd')){ //non valid option
        printf(" %s : unrecognised command option\n", trozos[1]);
        return -1;
    } //else if
    else    //tries to create a file
    {
        fd = open(trozos[1], O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); //system call to create the file
        if (fd == -1){
            if (errno == EEXIST){ //file exists
                printf(" Error: File %s already exists\n",trozos[1]);
                return -1;
            }
            else{ //other errors
                printf(" Error: file %s could not be created\n", trozos[1]);
                return -1;
            }
        }
        else{
            printf(" File %s created\n", trozos[1]); //file created successfully
            close(fd); //closes the file after being created
            return 0;
        }
    }
}

int borrar(char * trozos[], int ntrozos, struct extra_info *ex_inf){
    if (!strcmp(trozos[1], "-r")){ //if the -r flag is specified
        printf(" -r flag :D\n"); //recursive directory deleting. we need to implement LISTAR first
        return 0;
    }
    else if ((trozos[1][0] == '-') && (trozos[1][1] != 'r')){ //not valid flag
        printf(" %s: unrecognised command option", trozos[1]);
        return -1;
    }
    else{ //no flag is specified
        if (remove(trozos[1]) == -1){
            printf(" Error: File %s could not be deleted\n", trozos[1]);
            return -1;
        }
        else{
            printf(" File %s deleted\n", trozos[1]);
            return 0;
        }
    }
}



struct strfiledata * getInfo(char *path){ //This function is a helper that gets all the stat info of a file and puts it in a struct "strfiledata"
  struct strfiledata * strfinfo = NULL; //Struct that will contain the file info formatted as "strings"
  struct stat finfo; //Data buffer for fstat
  //MALLOC
  strfinfo = (struct strfiledata *) malloc(sizeof(struct strfiledata));
  if (lstat(path,&finfo) < 0) {
    printf("%s\n %s <-",strerror(errno),path); //Syscall
    return NULL;
  }
  //INODE NUMBER
  sprintf(strfinfo->inodenum,"%lu",finfo.st_ino);
  //TYPE & PERMISSIONS
  sprintf(strfinfo->permissions,"%s",ConvierteModo2(finfo.st_mode));
  //HARD LINK NUMBER
  sprintf(strfinfo->hlinksnum,"%lu",finfo.st_nlink);
  //USER
  struct passwd * tempu = getpwuid(finfo.st_uid);
  //Error
  if (tempu == NULL) {
    printf("Error: %s",strerror(errno));
    return NULL;
  };
  sprintf(strfinfo->user,"%s",tempu->pw_name);
  //GROUP
  struct group * tempg = getgrgid(finfo.st_gid);
  //Error
  if (tempu == NULL) {
    printf("Error: %s",strerror(errno));
    return NULL;
  };
  sprintf(strfinfo->group,"%s",tempg->gr_name);
  //SIZE
  sprintf(strfinfo->size,"%lu",finfo.st_size);
  //DATE FORMAT
  struct tm * mtime = localtime(&finfo.st_mtime);
  strftime(strfinfo->date,20,"%b %d %H:%M",mtime);
  //FILE NAME: cuts the rest of the path
  int j=0;
  for (int i = 0; path[i] != '\0'; i++) if (path[i] == '/') j = i+1;
  sprintf(strfinfo->name,"%s",&path[j]); //File name
  //LINKS TO
  if (readlink(path,strfinfo->linksto,200) == -1) {
    if (errno != 22) printf("%s",strerror(errno));
    strfinfo->linksto[0] = '\0';
    };
  return strfinfo;
}

int info(char * trozos[], int ntrozos, struct extra_info *ex_inf){
  for (int i = 1; i < ntrozos; i++) {
    struct strfiledata *  data = getInfo(trozos[i]);
    if (data == NULL) return -1;
    printf("%8s %s %2s %8s %8s %8s %s %8s",data->inodenum,data->permissions,data->hlinksnum,data->user,data->group,
           data->size,data->date,data->name);
    if (data->linksto[0] != '\0') printf(" -> %s",data->linksto);
    printf("\n");
    free(data);
  }
  return 0;
}


int reclisting(char * path,unsigned int options,int reclevel) {
  char space[reclevel + 1];
  for (int i = 0; i < reclevel;i++) space[i] ='-';
  space[reclevel] = '\0';
  struct strfiledata *  data = getInfo(path);
  if (data == NULL) return -1;
  if (options & 0x4 && (data->name[0] == '.')) return 0; //-v
    //
  if (options & 0x1) { //-l
    printf("%s %8s %s %2s %8s %8s %8s %s %8s",space,data->inodenum,data->permissions,data->hlinksnum,data->user,data->group,
             data->size,data->date,data->name);
    if (data->linksto[0] != '\0') printf(" -> %s",data->linksto);
    printf("\n");
    }
  else {
    printf("%s %12s %s\n",space,data->name,data->size);
  }
  if ((options & 0x2) && data->permissions[0] == 'd') { //-r
    DIR * dirpointer = opendir(path);
    free(data);
    reclevel++;
    if (dirpointer == NULL) {
      printf("%s",strerror(errno)); printf("\n");
      return 0;
    };
    struct dirent * contents = readdir(dirpointer);
    while (contents != NULL) {
      char filename[1024];
      sprintf(filename,"%s/%s",path,contents->d_name);
      if (!strcmp(contents->d_name,".") || !strcmp(contents->d_name,"..")) {
        reclisting(filename,options ^ 0x2,reclevel);
      }
      else {
        reclisting(filename,options,reclevel);
      }
      errno = 0;
      contents = readdir(dirpointer);
      if (errno != 0) return -1;
    }
    closedir(dirpointer);
  }
  else
    {
      free(data);
    }
  return 0;
}

int listar(char * trozos[], int ntrozos, struct extra_info *ex_inf) {
  unsigned int options = 0x0;
  int argstart = 1;
  for (int i = 1; i < ntrozos && i < 4; i++) {
    if (!strcmp(trozos[i],"-l")) {options = options | 0x1; argstart++;}
    if (!strcmp(trozos[i],"-r")) {options = options | 0x2; argstart++;}
    if (!strcmp(trozos[i],"-v")) {options = options | 0x4; argstart++;}
  }
  int ret = 0;
  for (int i = argstart;i < ntrozos; i++) ret |= reclisting(trozos[i],options,0);
  return ret;
}
