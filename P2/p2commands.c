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
    unsigned int permisos_open = 0;
    unsigned int permisos_mmap = 0;
    struct stat * statbuf;
    int i = 0;
    int fd = 0;
    void *file;
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
    errno = 0;
    fd = open(path,0,permisos_open);
    if (fd==-1){
        perror(strerror(errno));
        return -1;
    }
    errno = 0;
    if (fstat(fd,statbuf)==-1){
        free(statbuf);
        perror(strerror(errno));
        return -1;
    }
    errno = 0;
    file = mmap(NULL,statbuf->st_size,permisos_mmap,MAP_SHARED,fd,0);
    if (file == MAP_FAILED){
        free(statbuf);
        perror(strerrro(errno));
        return -1;
    }
    printf("File %s mapped at %p",path,file);
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
    errno = 0;
    shared_id = shmget(key,size,IPC_CREAT|IPC_EXCL);
    if (shared_id==-1){ //if shmget fails
        perror(strerror(errno));
        return -1;
    }
    errno = 0;
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
    errno = 0;
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