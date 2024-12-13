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
#include <sys/select.h>
#define BUFFER_SIZE 1024
// ===============================
// Mensagens 
// Topicos 
// Nome dos Pipes e Info Users
// Comandos
//================================


typedef struct topico
{
    char topico[100];
}topic;

typedef struct innerMsg {
    /*data*/
    char message[300];
}innerMsg;

typedef struct userRecog
{
    /* data */
    char username[100]; 
    char rcvpipename[100]; //cada user vai ter um pipe associado a ele , para receber os dados , o nome do pipe vai ser enviado para o server

}userRecog;

typedef struct threadhelper
{
    int cont [10]; 
}helper;

typedef struct Alltog
{
    /* data */
    innerMsg msg; //1
    topic topico[20]; //2 
    userRecog user[10]; //3
    helper help; 
    int tipo;
};





typedef struct{
    int tipo; 
    union{ // 

    }
}PEDIDO;



