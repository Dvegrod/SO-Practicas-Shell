#include "dynlist.h"

/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/
//Lista:

struct node {
  void * elem;
  struct node *next;
};

struct node * CreateList(){
    return NULL;
}

void InsertElement(struct node ** last, void *element){
    while (*last != NULL){ //'last' points to the last node
        last = &((*last)->next); //moves one node forward
    }
    *last = (struct node *) malloc(sizeof(struct node)); //allocates memory for the new node
    (*last)->next = NULL; //sets to null the new end of the list
    (*last)->elem = element;
}

int RemoveElementAt(struct node **plist, int position, void (*freeE)(void *)){
  if (*plist == NULL) return -1;

  struct node **pointertopointer = plist; //To aim to the pointer in the list that aims to the node to be removed
  for (int i = 0; i < position; i++) {
    if (*pointertopointer == NULL) return -1;  //Index out of bounds
    pointertopointer = &(*pointertopointer)->next;
  }
  struct node *auxiliar = *pointertopointer;  //Auxiliar stores the memory address of the node to be removed
  *pointertopointer = (*pointertopointer)->next; //The pointer in the list that aims to the expiring node now aims to the node after it (sometimes NULL)
  freeE(auxiliar->elem);
  free(auxiliar);
  return 0;
}

int RemoveElement(struct node **plist, void * elem,  void (*freeE)(void *)){
  struct node **pointertopointer = plist;
  for (iterator i = first(pointertopointer);!isLast(i);i = next(i)) {
    if (elem == getElement(i)) {
      RemoveElement(i,0,freeE);
      return 0;
    }
  }
  return 1;
}

void disposeAll(struct node ** ptolist, void (*freeE)(void *)) {
  while (RemoveElementAt(ptolist,0,freeE) == 0);
  /* struct node * aux; //Otra posibilidad
  while (*ptolist != NULL)
  {
      aux = *ptolist;
      *ptolist = aux->next;
      free(aux);
      }*/
}

int isEmptyList(lista l) {
  return (l == NULL);
}

iterator first(iterator it) {
  return it;
}

iterator next(iterator it) {
  return &(*it)->next;
}

int isLast(iterator it) {
  if (*it == NULL) {
    return 1;
  }
  else return 0;
}

void * getElement(iterator p) {
  return (*p)->elem;
}
