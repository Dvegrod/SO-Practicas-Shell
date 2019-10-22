#ifndef DLIST
#define DLIST
#include "exinf.h"

struct node {
  char com[MAXLEN];
  struct node *next;
};

//Interfaz

struct node * CreateList();

void InsertElement(struct node ** last, char *element);

int RemoveElement(struct node **plist, int position);

void disposeAll(struct node ** ptolist);

#endif
