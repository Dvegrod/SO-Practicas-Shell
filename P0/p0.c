#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 255
//Lista:

struct node {
    char com[MAXLEN];
    struct node *next;
};

struct node * lista;

void CreateList(struct node **plist){
    *plist = NULL;
}

void InsertElement(struct node **plist, char element[]) {
  struct node *last = *plist;
  while (last != NULL) {
    last = last->next;
  };
  last = (struct node *) malloc(sizeof(struct node));
  for (int i = 0; (element[i] != '\0') && (i < MAXLEN); i++)
    last->com[i] = element[i];
  last->next = NULL;
};

int RemoveElement(struct node **plist, int position){
  struct node **pointertopointer = plist; //To aim the pointer in the list that aims the node to be removed
  for (int i = 0; i < position; i++) {
    pointertopointer = &(**pointertopointer).next;
    if (*pointertopointer == NULL) return -1;  //Index out of bounds
  }
  struct node *auxiliar = *pointertopointer;
  *pointertopointer = (**pointertopointer).next;
  free(auxiliar);
  return 0;
};
//  ->(*LIST)->(NODE(*))->(NODE(*))->(NODE(*))

//Shell:

struct node *lista;

void printPrompt(){
    printf(" # ");
}

void readInput(){
    char comando[255];
    fgets(comando,255,stdin);
    InsertElement(&lista, comando);
}

int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void processInput(){
    
}

int main(int argc, char const *argv[])
{
    int salir = 0;
    while(!salir){
        printPrompt();
        readInput();
        processInput();
    }
    return 0;
}
