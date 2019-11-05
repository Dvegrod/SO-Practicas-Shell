#include "exinf.h"
#include "dynlist.h"
#include "p0commands.h"
#include "p1commands.h"
#include "p2commands.h"
#define MAX_N_ARG 32 //maximum number of arguments to a shell command
#define SHELL_EXIT_SIGNAL 2

/*
  Sistemas Operativos
  Práctica 1
  Carlos Torres (carlos.torres)
  Daniel Sergio Vega (d.s.vega)
  Grupo 4.3
*/

//Shell:

//Funciones para leer los comandos de la Shell

void printPrompt(){
    printf("# ");
}

int readInput(char comando[], struct extra_info * ex_inf){
    if (fgets(comando,MAXLEN,stdin)==NULL){
        printf("stdin could not be read\n");
        return SHELL_EXIT_SIGNAL; //returns an exit signal that tells main() to clean up and exit
    }
    if (comando[0]!='\n'){
        char * com = malloc(256*sizeof(char));
        sprintf(com,"%s\b ",comando);
        InsertElement(&(ex_inf->historial), com);
    }
    return 0;
}

int TrocearCadena(char * cadena, char const * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

int salir(char const *trozos[], int ntrozos, struct extra_info *ex_inf){
  return SHELL_EXIT_SIGNAL;
}

//Función para decidir qué comando se va a ejecutar

int processInput(char comando[], struct extra_info *ex_inf){
    char const *trozos[MAX_N_ARG];
    int ntrozos = TrocearCadena(comando,trozos);
    int i = 0;
    int return_value = 0;

    struct {
        char * cmd_name;
        int (* cmd_fun) (char const * trozos[], int ntrozos, struct extra_info *ex_inf);
    } cmds[] = {
        {"autores", autores},
        {"pid", pid},
        {"cdir", cdir},
        {"fecha", fecha},
        {"hora", hora},
        {"hist", hist},
        {"crear", crear},
        {"borrar", borrar},
        {"info", info},
        {"listar", listar},
        {"asignar", asignar},
        {"desasignar", desasignar},
        {"borrarkey", borrarkey},
        //{"mem", cmd_mem},
        {"volcar", volcar},
        {"llenar", llenar},
        {"recursiva", recursiva},
        {"rfich", rfich},
        {"wfich", wfich},
        {"fin", salir},
        {"end", salir},
        {"exit", salir},
        {NULL, NULL}
    };

    if (trozos[0]==NULL){ //in case nothing is entered
        return 0;
    }
    
    for (i = 0; cmds[i].cmd_name != NULL; i++)
    {
        if (!strcmp(trozos[0],cmds[i].cmd_name)){
            return_value = cmds[i].cmd_fun (trozos, ntrozos, ex_inf);
            break;
        } //if
    } //for
    if (cmds[i].cmd_name == NULL){ //cmd not found
        printf(" %s : unrecognised command\n", trozos[0]);
        return -1;
    } //cmd not found
    return return_value;
}


int main(int argc, char const *argv[]){
    char comando[MAXLEN];

    struct extra_info * ex_inf;
    ex_inf = (struct extra_info *) malloc(sizeof(struct extra_info));

    ex_inf->historial = CreateList();
    ex_inf->memoria.lmalloc = CreateList();
    ex_inf->memoria.lmmap = CreateList();
    ex_inf->memoria.lshmt = CreateList();

    do{
        printPrompt();
        readInput(comando, ex_inf);
    } while(processInput(comando, ex_inf) != SHELL_EXIT_SIGNAL);

    disposeAll(&ex_inf->historial,free); //cleans up and exits
    disposeTrilist(ex_inf);
    free(ex_inf);
    return EXIT_SUCCESS;
}
