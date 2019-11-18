/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/

#ifndef __P0_C
    #define __P0_C
    
    #include "./exinf.h"
    #include "./dynlist.h"
    
    int autores(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int pid(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int cdir(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int fecha(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int hora(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int hist(const char * trozos[], int ntrozos, struct extra_info *ex_inf);

#endif
