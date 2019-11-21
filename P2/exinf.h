/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/

#ifndef __EX_INF
  #define __EX_INF
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/mman.h>
  #include <sys/ipc.h>
  #include <sys/shm.h>
  #include <dirent.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <pwd.h>
  #include <grp.h>
  #include <errno.h>
  #include <ctype.h>
  #include "dynlist.h"
  #define MAXLEN 1024 //maximum length of strings (arrays of 1024 chars)


struct extra_info{
  lista historial;
  lista memoria;
};

#endif
