#ifndef P0C
#define P0C
#include "./exinf.h"
#include "./dynlist.h"

int autores(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int pid(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int cdir(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int fecha(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int hora(char * trozos[], int ntrozos, struct extra_info *ex_inf);

int hist(char * trozos[], int ntrozos, struct extra_info *ex_inf);

#endif
