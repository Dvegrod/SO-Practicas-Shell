#include "p3commands.h"

int priority (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int cmdfork (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int cmdexec (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int pplano (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int splano (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int listarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int cmdproc (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int borrarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
