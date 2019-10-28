#include "./dynlist.h"

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
    int i;
    while (*last != NULL){ //'last' points to the last node
        last = &((*last)->next); //moves one node forward
    }
    *last = (struct node *) malloc(sizeof(struct node)); //allocates memory for the new node
    (*last)->next = NULL; //sets to null the new end of the list
    (*last)->elem = element;
}

int RemoveElement(struct node **plist, int position){
  if (*plist == NULL) return -1;

  struct node **pointertopointer = plist; //To aim to the pointer in the list that aims to the node to be removed
  for (int i = 0; i < position; i++) {
    if (*pointertopointer == NULL) return -1;  //Index out of bounds
    pointertopointer = &(*pointertopointer)->next;
  }
  struct node *auxiliar = *pointertopointer;  //Auxiliar stores the memory address of the node to be removed
  *pointertopointer = (*pointertopointer)->next; //The pointer in the list that aims to the expiring node now aims to the node after it (sometimes NULL)
  free(auxiliar->elem);
  free(auxiliar);
  return 0;
}

void disposeAll(struct node ** ptolist) {
  while (RemoveElement(ptolist,0) == 0);
  /* struct node * aux; //Otra posibilidad
  while (*ptolist != NULL)
  {
      aux = *ptolist;
      *ptolist = aux->next;
      free(aux);
      }*/
}

iterator first(lista list) {
  return &list;
}

iterator next(iterator it) {
  return &(*it)->next;
}

iterator isLast(iterator it) {
  return (*it) == NULL;
}

void * getElement(iterator p) {
  return (*p)->elem;
}