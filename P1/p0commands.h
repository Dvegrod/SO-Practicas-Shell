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
    
    int autores(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int pid(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int cdir(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int fecha(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int hora(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int hist(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

#endif
