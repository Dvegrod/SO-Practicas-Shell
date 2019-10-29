#include "p2commands.h"
#define ASIGNAR_MALLOC 1
#define ASIGNAR_MMAP 2
#define ASIGNAR_C_SHARED 4
#define ASIGNAR_SHARED 8
#define ASIGNAR_ADDR 16


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
    //mostrar todas las direcciones asignadas
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
    unsigned int permisos_open = 0;
    unsigned int permisos_mmap = 0;
    struct stat * statbuf;
    int i = 0;
    int fd = 0;
    void *file_ptr;
    char *path = trozos[2];
    if (path==NULL){
        //mostrar lista de direcciones de mmap
        return 0;
    }
    //lee los permisos pasados por parámetro
    if (trozos[3]==NULL){
        permisos_open = S_IRWXU;
    }
    else{
        while (trozos[3][i]!='\0')
        {
            if (trozos[3][i]=='r'){ (permisos_open |= S_IRUSR); (permisos_mmap |= PROT_READ);};
            if (trozos[3][i]=='w'){ (permisos_open |= S_IWUSR); (permisos_mmap |= PROT_WRITE);};
            if (trozos[3][i]=='x'){ (permisos_open |= S_IXUSR); (permisos_mmap |= PROT_EXEC);};
            i++;
        }
    }
    
    fd = open(path,0,permisos_open);
    if (fd==-1){
        perror(strerror(errno));
        return -1;
    }
    
    if (fstat(fd,statbuf)==-1){
        free(statbuf);
        perror(strerror(errno));
        return -1;
    }
    
    file_ptr = mmap(NULL,statbuf->st_size,permisos_mmap,MAP_SHARED,fd,0);
    if (file_ptr == MAP_FAILED){
        free(statbuf);
        perror(strerrro(errno));
        return -1;
    }
    printf("File %s mapped at %p",path,file_ptr);
    //almacenar entrada en la lista de mmap
    return 0;
}

int asignar_crear_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf){
    int key = 0,size = 0,shared_id = 0;
    void *shm_ptr;
    if ((trozos[2]==NULL) || (trozos[3]==NULL)){ //no key or size are specified
        //mostrar lista de direcciones de áreas de memoria compartidas
    }
    key = atoi(trozos[2]);
    size = atoi(trozos[3]);
    
    shared_id = shmget(key,size,IPC_CREAT|IPC_EXCL);
    if (shared_id==-1){ //if shmget fails
        perror(strerror(errno));
        return -1;
    }
    
    shm_ptr = shmat(shared_id,NULL,0);
    if (shm_ptr == MAP_FAILED){
        perror(strerror(errno));
        return -1;
    }
    printf("Allocated shared memory (key %d) at %p",key,shm_ptr);
    //añadir a la lista de direcciones de áreas de memoria compartidas
}

int asignar_shared(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    int key = 0, shared_id = 0;
    void *shm_ptr;
    if (trozos[2]==NULL){ //no key is specified
        //mostrar lista de direcciones de áreas de memoria compartidas
    }
    key = atoi(trozos[2]);
    
    shared_id = shmget(key,0,0);
    if (shared_id == -1){
        perror(strerror(errno));
        return -1;
    }
    shm_ptr = shmat(shared_id,NULL,0);
    if (shm_ptr == MAP_FAILED){
        perror(strerror(errno));
        return -1;
    }
    printf("Allocated shared memory (key %d) at %p",key,shm_ptr);
    //añadir a la lista de direcciones de áreas de memoria compartidas
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int desasignar(char const * trozos[], int ntrozos, struct extra_info *ex_inf){

    //Mapear opciones
    unsigned int options = 0x0;
    if (!strcmp(trozos[1],"-malloc")) {options = options | ASIGNAR_MALLOC;}
    if (!strcmp(trozos[1],"-mmap")) {options = options | ASIGNAR_MMAP;}
    if (!strcmp(trozos[1],"-shared")) {options = options | ASIGNAR_SHARED;}
    if (!strcmp(trozos[1],"-addr")) {options = options | ASIGNAR_ADDR;}

    //Llamar a la función adecuada
    if(options & ASIGNAR_MALLOC){
        return desasignar_malloc(trozos,ntrozos,ex_inf);
    }
    if(options & ASIGNAR_MMAP){
        return desasignar_mmap(trozos,ntrozos,ex_inf);
    }
    if(options & ASIGNAR_SHARED){
        return desasignar_shared(trozos,ntrozos,ex_inf);
    }
    if(options & ASIGNAR_ADDR){
        return desasignar_addr(trozos,ntrozos,ex_inf);
    }
    //mostrar todas las direcciones asignadas
}

int desasignar_malloc(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    void * ptr;
    int tam;
    if (trozos[2]==NULL){
        //mostrar lista de direcciones asignadas con malloc
        return 0;
    }
    tam = atoi(trozos[2]);
    //buscar en la lista la primera entrada con tamaño=tam y devolver su direccion al puntero ptr
    if (ptr==NULL){
        //mostrar lista de direcciones asignadas con malloc
    }
    printf("Block at address %p deallocated (malloc)\n",ptr);
    free(ptr);
}

int desasignar_mmap(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    char *path;
    struct stat *statbuf;
    void *fileptr;
    if (trozos[2]==NULL){
        //mostrar la lista de direcciones mapeadas con mmap
        return 0;
    }
    //buscar fichero fich en la lista de ficheros mapeados a memoria, sacar también el puntero a la direccion fileptr
    if (stat(path,statbuf)==-1){
        perror(strerror(errno));
        return -1;
    }
    if (munmap(path,statbuf->st_size)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Block at address %p deallocated (mmap)",fileptr);
    //eliminar la entrada de la lista de archivos mapeados con mmap
}

int desasignar_shared(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    struct shmid_ds *shmbuf;
    int shmid = 0, key = 0;
    void *ptr;
    if (trozos[2]==NULL){
        //mostrar lista de áreas de memoria compartidas
        return 0;
    }
    key = atoi(trozos[2]);
    //buscar clave KEY en la lista
    if (key==0){ //clave no encontrada
        //mostrar lista de áreas de memoria compartidas
        return -1;
    }
    //recuperar de la lista la información de la clave (key) y la dirección (ptr)
    shmid = shmget(key, 0, 0);
    if (shmid==-1){
        perror(strerror(errno));
        return -1;
    }
    if (shmctl(shmid,IPC_RMID,shmbuf)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Block at address %p deallocated (shared)\n",ptr);
    //borrar la entrada de la lista
}

int desasignar_addr(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    void *addr;
    if (trozos[2]==NULL){
        //mostrar todas las direcciones asignadas (las 3 listas)
        return 0;
    }
    //buscar la dirección addr en la lista
    if (1/*reservada con malloc*/){
        free(addr);
        printf("Block at address %p deallocated (malloc)\n",addr);
        //eliminar la entrada de la lista
        return 0;
    }
    if (1/*reservada con mmap*/){
        //recuperar el path del fichero desde la lista
        trozos[2] == "";//path del fichero
        return desasignar_mmap(trozos,ntrozos,ex_inf);
    }
    if (1/*memoria compartida*/){
        int key;
        //recuperar la clave compartida de la lista
        sprintf(trozos[2],"%d",key);
        return desasignar_shared(trozos,ntrozos,ex_inf);
    }
}

int borrarkey(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    int shmid = 0, key = 0;
    struct shmid_ds * shmbuf;
    if (trozos[2]==NULL){
        printf("No key was entered\n");
        return -1;
    }
    shmid = shmget(key, 0, 0);
    if (shmid==-1){
        perror(strerror(errno));
        return -1;
    }
    if (shmctl(shmid,IPC_RMID,shmbuf)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Shared memory region of key %d removed",key);
    //borrar la entrada de la lista
}