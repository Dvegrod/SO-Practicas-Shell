#ifndef EXINF
#define EXINF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#define MAXLEN 256 //maximum length of strings (arrays of 256 chars)

struct extra_info {
  struct node * lista;
};
#endif
