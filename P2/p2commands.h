#ifndef __P2_C
    #define __P2_C
    
    #include "exinf.h"
    #include "dynlist.h"
    
    int asignar(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int desasignar(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int borrarkey(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int cmd_mem(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int volcar(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int llenar(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int recursiva(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int rfich(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
    
    int wfich(const char * trozos[], int ntrozos, struct extra_info *ex_inf);
//--------------------------------------------------------------------------------
    void disposeTrilist(struct extra_info *ex_inf);
#endif
