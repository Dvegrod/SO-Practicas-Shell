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

int RemoveElementAt(struct node **plist, iterator position, int (*freeE)(void *)){
  if (*plist == NULL) return -1;

  struct node **pointertopointer = plist; //To aim to the pointer in the list that aims to the node to be removed
  for (iterator i = first(pointertopointer);i != position; i = next(i)) {
    if (*pointertopointer == NULL) return -1;  //Index out of bounds
    pointertopointer = &(*pointertopointer)->next;
  }
  struct node *auxiliar = *pointertopointer;  //Auxiliar stores the memory address of the node to be removed
  *pointertopointer = (*pointertopointer)->next; //The pointer in the list that aims to the expiring node now aims to the node after it (sometimes NULL)
  int ret = freeE(auxiliar->elem);
  free(auxiliar);
  return ret;
}

int RemoveElement(struct node **plist, void * elem,  int (*freeE)(void *)){
  struct node **pointertopointer = plist;
  for (iterator i = first(pointertopointer);!isLast(i);i = next(i)) {
    if (elem == getElement(i)) {
      return RemoveElementAt(i,first(i),freeE);
    }
  }
  return 1;
}

int disposeAll(struct node ** ptolist, int (*freeE)(void *)) {
  int ret = 0;
  while ((ret = RemoveElementAt(ptolist,first(ptolist),freeE)) == 0);
  /* struct node * aux; //Otra posibilidad
  while (*ptolist != NULL)
  {
      aux = *ptolist;
      *ptolist = aux->next;
      free(aux);
      }*/
  return ret <= 0 ? ret : 0;
}

int isEmptyList(lista l) {
  return (l == NULL);
}

iterator first(lista * it) { //No hacer modificaciones concurrentes
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
