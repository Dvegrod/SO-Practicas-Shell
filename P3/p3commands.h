/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/


#ifndef __P3_C
#define __P3_C
#include "exinf.h"

int priority (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int cmdfork (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int cmdexec (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int pplano (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int splano (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int listarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int cmdproc (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int borrarprocs (const char * trozos[], int ntrozos, struct extra_info *ex_inf);
int direct_cmd (const char * trozos[], int ntrozos, struct extra_info *ex_inf);

int freePElem(void * elem);

#endif
