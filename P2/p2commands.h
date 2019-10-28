#ifndef __P2_C
    #define __P2_C
    
    #include "exinf.h"
    #include "dynlist.h"
    
    int asignar(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int desasignar(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int borrarkey(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int mem(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int volcar(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int llenar(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int recursiva(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int rfich(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int wfich(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    
#endif
