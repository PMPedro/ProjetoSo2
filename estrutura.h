#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <string.h>
#include  <sys/wait.h>
// ===============================
// Mensagens 
// Topicos 
// Nome dos Pipes e Info Users
// Comandos
//================================


typedef struct innerMsg {
    /*data*/

    char message[300];

};

//encapsulamento de mensagem
// int tipo representará leitr
typedef struct Capsule 
{
    /* data */
    int tipo;  
    innerMsg Mensagem; 


};
