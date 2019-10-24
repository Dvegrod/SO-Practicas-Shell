/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/

#ifndef __DYN_LIST
  #define __DYN_LIST
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
