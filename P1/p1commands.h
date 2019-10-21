
#include <stdio.h>
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

#define LIST_LONG 1
#define LIST_RECR 2
#define LIST_NVRB 4

struct strfiledata {
   char inodenum[1000],permissions[12],hlinksnum[10],user[200],group[200],size[10],date[20],name[200],linksto[200];
 };
//Interfaz

int crear(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int borrar(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int info(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int listar(char * trozos[], int ntrozos, struct extra_info *ex_inf);

