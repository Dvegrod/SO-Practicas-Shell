#include "p2commands.h"
#define ASIGNAR_MALLOC 1
#define ASIGNAR_MMAP 2
#define ASIGNAR_C_SHARED 4
#define ASIGNAR_SHARED 8
#define ASIGNAR_ADDR 16

struct melem {
  unsigned long dir;
  unsigned long size;
  char * date;
  void * others;
};

struct immap {
  char filename[512];
  int fd;
};


int buildElem(unsigned long where,unsigned long tam, lista list, void * ex) {
  struct melem * new = malloc(sizeof(struct melem));
  //Error
  if (new == NULL) {
    printf("%s",strerror(errno));
    return -1;
  }
  new->dir = where;
  new->size = tam;
  time_t * now = malloc(sizeof(time_t));
  //Error
  if (now == NULL) {
    printf("%s",strerror(errno));
    return -1;
  }
  if (time(now)==-1){ //writes current time to NOW variable
    perror(strerror(errno));
    return -1;
  };
  strftime(new->date,30,"%a %b %d %T %Y",localtime(now));
  free(now);
  new->others = NULL;
  if (ex != NULL)
      new->others = ex;
  InsertElement(&list,new);
  return 0;
}

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
        for (iterator i = first(ex_inf->memoria.lmalloc); !isLast(i); i = next(i)) {
          struct melem * elem = getElement(i);
          printf("%lxu: size: %lu malloc %s",elem->dir,elem->size,elem->date);
        }
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
    buildElem(tam,(unsigned long) tmp,ex_inf->memoria.lmalloc,NULL);
    return 0;
}

int asignar_mmap(char const * trozos[], int ntrozos, struct extra_info *ex_inf){
    unsigned int permisos_open = 0;
    unsigned int permisos_mmap = 0;
    struct stat * statbuf = malloc(sizeof(struct stat));
    int i = 0;
    int fd = 0;
    void *file_ptr;
    char const *path = trozos[2];
    if (path==NULL){
        for (iterator i = first(ex_inf->memoria.lmmap); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            struct immap * others = elem->others;
            printf("%lxu: size: %lu mmap %s %iu %s",elem->dir,elem->size,others->filename,others->fd,elem->date);
        }
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
        perror(strerror(errno));
        return -1;
    }
    printf("File %s mapped at %p",path,file_ptr);
    struct immap * ifile = malloc(sizeof(struct immap));
    sprintf(ifile->filename,"%s",path);
    ifile->fd = fd;
    buildElem(statbuf->st_size,(unsigned long) file_ptr,ex_inf->memoria.lmmap,ifile);
    return 0;
}

int asignar_crear_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf){
    int key = 0,size = 0,shared_id = 0;
    void *shm_ptr;
    if ((trozos[2]==NULL) || (trozos[3]==NULL)){ //no key or size are specified
      return 0;//WHAT
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
    buildElem(size,(unsigned long) shm_ptr,ex_inf->memoria.lshmt,NULL);
    return 0;
}

int asignar_shared(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    int key = 0, shared_id = 0;
    void *shm_ptr;
    if (trozos[2]==NULL){ //no key is specified
        for (iterator i = first(ex_inf->memoria.lshmt); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            int * pkey = elem->others;
            printf("%lxu: size: %lu shared memory (key: %i) %s",elem->dir,elem->size,*pkey,elem->date);
        }
        return 0;
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
    int * pkey = malloc(sizeof(int));
    *pkey = key;
    //
    struct shmid_ds * shstat = malloc(sizeof(struct shmid_ds));
    shmctl(shared_id,IPC_STAT,shstat);
    printf("Allocated shared memory (key %d) at %p",key,shm_ptr);
    //
    buildElem(shstat->shm_segsz,(unsigned long) shm_ptr,ex_inf->memoria.lshmt,pkey);
    free(shstat);
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
    void * ptr = NULL;
    unsigned long tam;
    if (trozos[2]==NULL){
        for (iterator i = first(ex_inf->memoria.lmalloc); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            printf("%lxu: size: %lu malloc %s",elem->dir,elem->size,elem->date);
        }
        return 0;
    }
    tam = strtoul(trozos[2],NULL,10);
    struct melem * e = NULL;
    for (iterator i = first(ex_inf->memoria.lmalloc); !isLast(i); i = next(i)) {
      struct melem * elem = getElement(i);
      if (elem->size == tam) { //PENDIENTE
        ptr = (void *) elem->dir;
        e = elem;
        destroyIt(i); break;
      }
    }
    if (ptr == NULL){
        for (iterator i = first(ex_inf->memoria.lmalloc); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            printf("%lxu: size: %lu malloc %s",elem->dir,elem->size,elem->date);
        }
        return 0;
    }
    printf("Block at address %p deallocated (malloc)\n",ptr);
    free(ptr);
    RemoveElementAt(&ex_inf->memoria.lmalloc,e);
    return 0;
}

int desasignar_mmap(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    char const *path = trozos[2];
    if (trozos[2]==NULL){
        for (iterator i = first(ex_inf->memoria.lmmap); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            struct immap * others = elem->others;
            printf("%lxu: size: %lu mmap %s %iu %s",elem->dir,elem->size,others->filename,others->fd,elem->date);
        }
        return 0;
    }
    //buscar fichero fich en la lista de ficheros mapeados a memoria, sacar también el puntero a la direccion fileptr
    struct melem * e = NULL,* elem = NULL;
    struct immap * others = NULL;
    int n = 0;
    for (iterator i = first(ex_inf->memoria.lmmap); !isLast(i); i = next(i)) {
        elem = getElement(i);
        others = elem->others;
        n++;
        if (!strcmp(others->filename,path)) {
          e = elem;
          destroyIt(i); break;
        }
    }
    //
    if (munmap((void *) e->dir,e->size)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Block at address %lux deallocated (mmap)",e->dir);
    //eliminar la entrada de la lista de archivos mapeados con mmap
    RemoveElementAt(&ex_inf->memoria.lmmap,e);
    return 0;
}

int desasignar_shared(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    int shmid = 0, key = 0;
    if (trozos[2]==NULL){
        for (iterator i = first(ex_inf->memoria.lshmt); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            int * pkey = elem->others;
            printf("%lxu: size: %lu shared memory (key: %i) %s",elem->dir,elem->size,*pkey,elem->date);
        }
        return 0;
    }
    key = atoi(trozos[2]);
    struct melem * e = NULL;
    for (iterator i = first(ex_inf->memoria.lshmt); !isLast(i); i = next(i)) {
        struct melem * elem = getElement(i);
        if (*((int *)(elem->others)) == key) {
          e = elem;
          destroyIt(i); break;
        }
    }
    if (key==0){ //WHAT clave no encontrada
        for (iterator i = first(ex_inf->memoria.lshmt); !isLast(i); i = next(i)) {
            struct melem * elem = getElement(i);
            int * pkey = elem->others;
            printf("%lxu: size: %lu shared memory (key: %i) %s",elem->dir,elem->size,*pkey,elem->date);
        }
        return -1;
    }
    //recuperar de la lista la información de la clave (key) y la dirección (ptr)
    shmid = shmget(*((int *) e->others), 0, 0);
    if (shmid==-1){
        perror(strerror(errno));
        return -1;
    }
    if (shmctl(shmid,IPC_RMID,NULL)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Block at address %lux deallocated (shared)\n",e->dir);
    RemoveElementAt(&ex_inf->memoria.lshmt,e);
    return 0;
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
    shmid = shmget(key, 0, 0); //WAT?
    if (shmid==-1){
        perror(strerror(errno));
        return -1;
    }
    if (shmctl(shmid,IPC_RMID,shmbuf)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Shared memory region of key %d removed",key);
    RemoveElementAt(&ex_inf->memoria.lshmt,)
}
