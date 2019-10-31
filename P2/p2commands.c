#include "p2commands.h"
#define ASIGNAR_MALLOC 1
#define ASIGNAR_MMAP 2
#define ASIGNAR_C_SHARED 4
#define ASIGNAR_SHARED 8
#define ASIGNAR_ADDR 16
#define SS_MALLOC 1
#define SS_MMAP 2
#define SS_SHM 4
#define SS_ADDR 8

struct melem {
  unsigned long dir;
  unsigned long size;
  char date[30];
  void * others;
};

struct immap {
  char filename[512];
  int fd;
};


int buildElem(unsigned long tam, unsigned long where, iterator list, void * ex) {
  struct melem * new = malloc(sizeof(struct melem));
  //Error
  if (new == NULL) {
    printf("%s",strerror(errno));
    return -1;
  }
  new->dir = where;
  new->size = tam;
  time_t * now = malloc(sizeof(time_t));
  if (time(now)==-1){ //writes current time to NOW variable
    perror(strerror(errno));
    return -1;
  };
  strftime(new->date,30,"%a %b %d %T %Y",localtime(now));
  new->others = NULL;
  if (ex != NULL)
    new->others = ex;
  InsertElement(list,new);
  return 0;
}

int showElem(lista list, int flag) {
  for (iterator i = first(&list); !isLast(i); i = next(i)) {
    struct melem * elem = getElement(i);
    printf("%lxu: size: %lu ",elem->dir,elem->size);
    switch (flag) {
      case SS_MALLOC : {
        printf("malloc ");
        break;
      }
      case SS_MMAP : {
        struct immap * others = elem->others;
        printf("mmap %s (fd: %i) ",others->filename,others->fd);
        break;
      }
      case SS_SHM : {
        printf("shared memory (key: %i) ",*((int *) elem->others));
        break;
      }
      default : return -1;//Temp
    }
    printf("%s\n",elem->date);
  }
  return 0;
}

int searchElem(lista list, int flag, struct melem ** e, void * id) {
  *e = NULL;
  for (iterator i = first(&list); !isLast(i); i = next(i)) {
    struct melem * elem = getElement(i);
    switch (flag) {
      case SS_MALLOC:
        if (elem->size == *((unsigned long *) id)) {
          *e = elem;
          return 0;
        }
        else break;
      case SS_MMAP: {
        struct immap * others = elem->others;
        if (!strcmp(others->filename,(char *)id)) {
          *e = elem;
          return 0;
        }
        break;
      }
      case SS_SHM:
        if (*(int *)elem->others == *(int *)id) {
          *e = elem;
          return 0;
        }
        else break;
      case SS_ADDR:
        if (elem->dir == *(unsigned long *)id) {
          *e = elem;
          return 0;
        }
        else break;
      default: return -1;
    }
  }
  return 0;
}

int asignar_malloc(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
int asignar_mmap(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
int asignar_crear_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf);
int asignar_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf);



int asignar(char const * trozos[], int ntrozos, struct extra_info *ex_inf){

    if (trozos[1] == NULL) return showElem(ex_inf->memoria.lmalloc,SS_MALLOC)
                           | showElem(ex_inf->memoria.lmmap,SS_MMAP)
                           | showElem(ex_inf->memoria.lshmt,SS_SHM);
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
    if (trozos[2] == NULL)
      return showElem(ex_inf->memoria.lmalloc,SS_MALLOC);
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
    printf("allocated %d at %p\n",tam,tmp);
    buildElem(tam,(unsigned long) tmp,&(ex_inf->memoria.lmalloc),NULL);
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
      showElem(ex_inf->memoria.lmmap,SS_MMAP);
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
        perror(strerror(errno));
        return -1;
    }
    printf("File %s mapped at %p\n",path,file_ptr);
    //
    struct immap * ifile = malloc(sizeof(struct immap));
    sprintf(ifile->filename,"%s",path);
    ifile->fd = fd;
    buildElem(statbuf->st_size,(unsigned long) file_ptr,&ex_inf->memoria.lmmap,ifile);
    free(statbuf); //Pendiente de revision
    return 0;
}

int asignar_crear_shared(char const * trozos[], int ntrozos, struct extra_info *ex_inf){
    int key = 0,size = 0,shared_id = 0;
    void *shm_ptr;
    if ((trozos[2]==NULL) || (trozos[3]==NULL)){ //no key or size are specified
      return showElem(ex_inf->memoria.lshmt,SS_SHM);
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
    printf("Allocated shared memory (key %d) at %p\n",key,shm_ptr);
    buildElem(size,(unsigned long) shm_ptr,&ex_inf->memoria.lshmt,NULL);
    return 0;
}

int asignar_shared(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    int key = 0, shared_id = 0;
    void *shm_ptr;
    if (trozos[2]==NULL){ //no key is specified
      return showElem(ex_inf->memoria.lshmt,SS_SHM);
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
    printf("Allocated shared memory (key %d) at %p\n",key,shm_ptr);
    //
    buildElem(shstat->shm_segsz,(unsigned long) shm_ptr,&ex_inf->memoria.lshmt,pkey);
    free(shstat);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int desasignar_addr(char const * trozos[], int ntrozos, struct extra_info * ex_inf);
int desasignar_shared(char const * trozos[], int ntrozos, struct extra_info * ex_inf);
int desasignar_mmap(char const * trozos[], int ntrozos, struct extra_info * ex_inf);
int desasignar_malloc(char const * trozos[], int ntrozos, struct extra_info * ex_inf);


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
    unsigned long tam;
    if (trozos[2]==NULL){
      return showElem(ex_inf->memoria.lmalloc,SS_MALLOC);
    }
    tam = strtoul(trozos[2],NULL,10);
    //
    struct melem * e = NULL;
    searchElem(ex_inf->memoria.lmalloc,SS_MALLOC,&e,&tam);
    //
    if (e == NULL){
      return showElem(ex_inf->memoria.lmalloc,SS_MALLOC);
    }
    printf("Block at address %p deallocated (malloc)\n",(void *) e->dir);
    free((void *) e->dir);
    RemoveElementAt(&ex_inf->memoria.lmalloc,e);
    return 0;
}

int desasignar_mmap(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    char const *path = trozos[2];
    if (trozos[2]==NULL){
      return showElem(ex_inf->memoria.lmmap,SS_MMAP);
    }
    //buscar fichero fich en la lista de ficheros mapeados a memoria, sacar también el puntero a la direccion fileptr
    struct melem * e = NULL;
    searchElem(ex_inf->memoria.lmmap,SS_MMAP,&e,(void *) path);//PENDIENTE CONST VOID
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
      return showElem(ex_inf->memoria.lshmt,SS_SHM);
    }
    key = atoi(trozos[2]);
    struct melem * e = NULL;
    searchElem(ex_inf->memoria.lshmt,SS_SHM,&e,&key);
    //
    if (key==0){
      return showElem(ex_inf->memoria.lshmt,SS_SHM);
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
    void * addr = (void *) trozos[2];
    if (trozos[2]==NULL){
      return showElem(ex_inf->memoria.lmalloc,SS_MALLOC)
        | showElem(ex_inf->memoria.lmmap,SS_MMAP)
        | showElem(ex_inf->memoria.lshmt,SS_SHM);
    }
    //buscar la dirección addr en la lista
    struct melem * e = NULL;
    searchElem(ex_inf->memoria.lmalloc,SS_ADDR,&e,addr);
    if (e != NULL){
        free(addr);
        printf("Block at address %p deallocated (malloc)\n",addr);
        //eliminar la entrada de la lista
        return 0;
    }
    searchElem(ex_inf->memoria.lmmap,SS_ADDR,&e,addr);
    if (e != NULL){
        struct immap * others = e->others;
        //recuperar el path del fichero desde la lista
        trozos[2] = others->filename;//path del fichero WHAT trozos dos cambia de direccion (produce no alcanzables? const?)
        return desasignar_mmap(trozos,ntrozos,ex_inf);
    }
    searchElem(ex_inf->memoria.lshmt,SS_ADDR,&e,addr);
    if (e != NULL){
        int key = *(int *)e->others;
        //recuperar la clave compartida de la lista
        sprintf(trozos[2],"%d",key); //Lo mismo
        return desasignar_shared(trozos,ntrozos,ex_inf);
    }
    //Not found
    return showElem(ex_inf->memoria.lmalloc,SS_MALLOC)
      | showElem(ex_inf->memoria.lmmap,SS_MMAP)
      | showElem(ex_inf->memoria.lshmt,SS_SHM);
}

int borrarkey(char const * trozos[], int ntrozos, struct extra_info * ex_inf){
    int shmid = 0, key = 0;
    if (trozos[2]==NULL){
        printf("No key was entered\n");
        return -1;
    }
    key = atoi(trozos[2]);
    shmid = shmget(key, 0, 0);
    if (shmid==-1){
        perror(strerror(errno));
        return -1;
    }
    if (shmctl(shmid,IPC_RMID,NULL)==-1){
        perror(strerror(errno));
        return -1;
    }
    printf("Shared memory region of key %d removed",key); //Nada es eliminado??
    return 0;
}
