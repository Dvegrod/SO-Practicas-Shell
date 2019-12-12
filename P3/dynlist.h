/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/

#ifndef __DYN_LIST
  #define __DYN_LIST
  #include <stdlib.h>
  //Interfaz

  typedef struct node * lista;
  typedef struct node ** iterator;

  lista CreateList();

  void InsertElement(lista *last, void *element);

  int RemoveElementAt(lista *plist, iterator position, int (*freeE)(void *));

  int RemoveElement(lista *plist, void * elem, int (*freeE)(void *));

  int disposeAll(lista * ptolist, int (*freeE)(void *));

  int isEmptyList(lista l);

  iterator first(lista * list);

  iterator next(iterator p);

  int isLast(iterator p);

  void * getElement(iterator p);

#endif
