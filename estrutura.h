#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#define BUFFER_SIZE 1024
// ===============================
// Mensagens 
// Topicos 
// Nome dos Pipes e Info Users
// Comandos
//================================


typedef struct innerMsg {
    /*data*/

    char message[300];

}innerMsg;

//encapsulamento de mensagem
// int tipo representará leitr
typedef struct tipoMsg 
{
    /* data */
    int tipo;  // 0 
    innerMsg Mensagem; 
}tipoMsg;

typedef struct userRecog
{
    /* data */
    char username[100]; 
    char rcvpipename[100]; //cada user vai ter um pipe associado a ele , para receber os dados , o nome do pipe vai ser enviado para o server

}userRecog;

typedef struct UR
{
    /* data */
    int tipo; // 1
    userRecog User;
}UR;


