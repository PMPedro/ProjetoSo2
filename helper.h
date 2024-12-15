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
#include "pthread.h"
#include <stdbool.h>
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
    int duracao; 
}innerMsg;


typedef struct topico
{
    char nomeTopico[100];
    innerMsg msg; 
    innerMsg msgPersistente[5];
    int locked;
}topic;



typedef struct userRecog
{
    /* data */
    char username[100]; 
    char rcvpipename[100]; //cada user vai ter um pipe associado a ele , para receber os dados , o nome do pipe vai ser enviado para o server
    topic topicosInscritos[20];
   // helper thd;

}userRecog;

typedef struct threadhelper
{
    int cont [10]; 
    fd_set read_fds;
    int fd;
}helper;

typedef struct Alltog
{
    /* data */
    //Enviar msg = 1 ; 
    //User inicia = 2 ; 
    //coisas pipe -> nenhum (default)
    // tratar coisas user = 3 , 4 (restantes que forem necessarios )

    topic topico[20];  
    userRecog user[10]; 
    helper help; 
    int tipo;
   
    
}all;