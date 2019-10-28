#include "p2commands.h"
#define ASIGNAR_MALLOC 1
#define ASIGNAR_MMAP 2
#define ASIGNAR_C_SHARED 4
#define ASIGNAR_SHARED 8



int asignar_malloc(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
int asignar_mmap(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
int asignar_crear_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
int asignar_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf);

int asignar(char const * trozos[], int ntrozos, struct extra_info *ex_inf){

    //Mapear opciones
    unsigned int options = 0x0;
    if (!strcmp(trozos[1],"-malloc")) {options = options | ASIGNAR_MALLOC;}
    if (!strcmp(trozos[1],"-mmap")) {options = options | ASIGNAR_MMAP;}
    if (!strcmp(trozos[1],"-crearshared")) {options = options | ASIGNAR_C_SHARED;}
    if (!strcmp(trozos[1],"-shared")) {options = options | ASIGNAR_SHARED;}

    //Llamar a la función adecuada
    if(options & ASIGNAR_MALLOC){
        return asignar_malloc(trozos,ntrozos,ex_inf);
    }
    if(options & ASIGNAR_MMAP){
        return asignar_mmap(trozos,ntrozos,ex_inf);
    }
    if(options & ASIGNAR_C_SHARED){
        return asignar_crear_shared(trozos,ntrozos,ex_inf);
    }
    if(options & ASIGNAR_SHARED){
        return asignar_shared(trozos,ntrozos,ex_inf);
    }
}

int asignar_malloc(char const * trozos[], int ntrozos, struct extra_info *ex_inf){
    int tam;
    void *tmp;
    if (trozos[2] == NULL){
        //mostrar lista de direcciones de malloc
        return 0;
    }
    tam = atoi(trozos[2]);
    if (tam==0){
        //malloc(0) ??
        return 0;
    }
    tmp = malloc(tam);
    if (tmp==NULL){
        printf("Memory allocation failed\n");
        return -1;
    }
    printf("allocated %d at %p",tam,tmp);
    //añadir entrada a la lista
    return 0;
}

int asignar_mmap(char const * trozos[], int ntrozos, struct extra_info *ex_inf){
    char *path = trozos[2];
}