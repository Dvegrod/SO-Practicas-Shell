#ifndef P0C
#define P0C
#include "./exinf.h"
#include "./dynlist.h"

int autores(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

int pid(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

int cdir(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

int fecha(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

int hora(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

int hist(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

#endif
