//#include "helper.h"
#pragma
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
#include <pthread.h>
#include <stdbool.h>
#define BUFFER_SIZE 1024

#define FIFO_NAME "mainpipe"
int running = 1;









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
    char userRemove[40];
   
    
}all;

typedef struct{
    char user[100];
    char message[300];
    char nometopico[100];
    char pipe[30];
    int tipo;
}Mensagem;

void initializeAll(all *st) {
    if (st == NULL) {
        return; // Evitar problemas caso o ponteiro seja nulo.
    }

    // Inicializar tópicos gerais criados
    for (int i = 0; i < 20; i++) {
        memset(st->topico[i].nomeTopico, 0, sizeof(st->topico[i].nomeTopico));
        memset(st->topico[i].msg.message, 0, sizeof(st->topico[i].msg.message));
        st->topico[i].msg.duracao = 0;
        for (int j = 0; j < 5; j++) {
            memset(st->topico[i].msgPersistente[j].message, 0, sizeof(st->topico[i].msgPersistente[j].message));
            st->topico[i].msgPersistente[j].duracao = 0;
        }
        st->topico[i].locked = 0;
    }

    // Inicializar usuários
    for (int i = 0; i < 10; i++) {
        memset(st->user[i].username, 0, sizeof(st->user[i].username));
        memset(st->user[i].rcvpipename, 0, sizeof(st->user[i].rcvpipename));
        for (int j = 0; j < 20; j++) {
            memset(st->user[i].topicosInscritos[j].nomeTopico, 0, sizeof(st->user[i].topicosInscritos[j].nomeTopico));
            memset(st->user[i].topicosInscritos[j].msg.message, 0, sizeof(st->user[i].topicosInscritos[j].msg.message));
            st->user[i].topicosInscritos[j].msg.duracao = 0;
            for (int k = 0; k < 5; k++) {
                memset(st->user[i].topicosInscritos[j].msgPersistente[k].message, 0, sizeof(st->user[i].topicosInscritos[j].msgPersistente[k].message));
                st->user[i].topicosInscritos[j].msgPersistente[k].duracao = 0;
            }
            st->user[i].topicosInscritos[j].locked = 0;
        }
    }

    // Inicializar o helper
    for (int i = 0; i < 10; i++) {
        st->help.cont[i] = 0;
    }
    FD_ZERO(&st->help.read_fds); // Zerar o conjunto de descritores
    st->help.fd = -1;

    // Inicializar outros campos
    st->tipo = 0;
    memset(st->userRemove, 0, sizeof(st->userRemove));
}





void listMessage(all *aux, all *main)
{
    // printf("\n <MSG> %s", aux->topico[0].msg.message);
}



void processaEnvioMensagens(all *estruturaprincipal, Mensagem mensagem){

    for(int i = 0; i <20; i++){
        
        fflush(stdout);
        //sleep(2);
        if( strlen(estruturaprincipal->topico[i].nomeTopico) == 0 ){
            
            strcpy(estruturaprincipal->topico[i].nomeTopico, mensagem.nometopico) ;
            strcpy(estruturaprincipal->user[i].topicosInscritos, mensagem.nometopico) ;

            printf("\nestrutura topico: %s\n", estruturaprincipal->topico[i].nomeTopico);
            fflush(stdout);
            return;
        }else if ( strcmp(estruturaprincipal->topico[i].nomeTopico, mensagem.nometopico) == 0){ // se encontrar um topico diferente, quebra o ciclo e procura enviar a mensagem
            
            break;
        }

    }

    for(int x = 0; x <20; x++){
        printf("\n\ntopicos existentes\n topico[%d]: %s", x, estruturaprincipal->topico[x].nomeTopico);
    }


    for (int xusers = 0 ; xusers < 10; xusers++){
        printf("\n estruturaprincipal[xusers].username: %s\n", estruturaprincipal->user[xusers].username);
        fflush(stdout);
        if ( strcmp ( estruturaprincipal->user[xusers].username, mensagem.user) != 0){

            for (int yuserregisteredtopics = 0; yuserregisteredtopics < 20; yuserregisteredtopics++){

            //compara se o nome do utilizador xusers é igual ao que enviou a mensagem
                
                if ( strcmp(  estruturaprincipal->user[xusers].topicosInscritos[yuserregisteredtopics].nomeTopico, mensagem.nometopico ) == 0 ){
                    printf("entrou para enviar mensagem!!!!!!!!\n user a receber mensagem:%s\n pipe:%s", estruturaprincipal->user[xusers].username, estruturaprincipal->user[xusers].rcvpipename);
                    write(estruturaprincipal->user[xusers].rcvpipename,&mensagem.message, sizeof(mensagem.message));
                    fflush(stdout);
                    
                }  

            
            
    //write();

        }
    } 
    
    }
    printf("SUCCESS!!!!");

}



void handle_signal(int sig)
{
    printf("Adeus!\n");
    running = 0;
    // close(fd);
    unlink(FIFO_NAME);
}




void entradaUser(all *main, Mensagem mensagem){
    //all *ptd = (all *) main;
    printf("[MANAGER] Verificando autenticação de usuário...\n");

    bool podeLogar = false;
    int posicao = -1;

    // Verificar se o usuário já está logado
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(&main->user[i].username, mensagem.user) == 0)
        {
            printf("[MANAGER] Usuário já logado: %s\n", mensagem.user);
            podeLogar = false;
            return;
        }
    }

    // Verificar espaço para logar
    for (int i = 0; i < 10; i++)
    {
        if (strlen(&main->user[i].username) == 0)
        {
            podeLogar = true;
            strcpy(&main->user[i].username, mensagem.user);
            strcpy(&main->user[i].rcvpipename, mensagem.pipe);
            printf("ENTROU NO ESPAÇO PARA LOGAR!!!\n\n\n username:%s\tpipe:%s\n\n\n",&main->user[i].username,&main->user[i].rcvpipename);
            posicao = i;
            break;
        }
    }


    for (int y = 0; y < 10; y++)
    printf("utilizadores ligados:%s\n",&main->user[y].username);


    if (posicao == -1)
    {
        printf("[MANAGER] Não há espaço para novos usuários.\n");
        podeLogar = false;
    }
    printf("\n<manager> pipe de login: %s\n", mensagem.pipe);

    // Enviar resposta para o cliente
    int fd = open(mensagem.pipe, O_WRONLY);
    if (fd == -1)
    {
        perror("[MANAGER] Erro ao abrir pipe de resposta");
        return;
    }

    char resposta[10] = {0};
    strcpy(resposta, podeLogar ? "true" : "false");
    printf("user:%s\n",mensagem.user);
    if (write(fd, resposta, sizeof(resposta)) == -1)
    {
        perror("[MANAGER] Erro ao enviar resposta para o cliente");
    }
    else
    {
        printf("[MANAGER] Resposta enviada para o cliente: %s\n", resposta);
        fflush(stdin);
    }

    close(fd);
    return;
}









void *getpipemessages(void *pall)
{

    
    all *ptd = (all *)pall;
    all aux, aux2;
    Mensagem mensagem;
     

    memset(&aux2, 0, sizeof(aux2));
    memset(&aux, 0, sizeof(aux2));
    printf("tamanho all: %d ", sizeof(ptd));
    aux.help.fd = ptd->help.fd;

    
    int total_bytes;

    while (1){
        total_bytes = 0;
        
        printf("chegou v2 \n");
        pthread_mutex_t thrmutex;

        pthread_mutex_init(&thrmutex,NULL);

        //total_bytes = read(ptd->help.fd, &aux2 , sizeof(all) );
        pthread_mutex_lock(&thrmutex);
        total_bytes = read(ptd->help.fd, &mensagem , sizeof(Mensagem) );
        pthread_mutex_unlock(&thrmutex);
        fflush(stdin);
        printf("chegou v3. tipo de mensagem: %d \n", mensagem.tipo);

        if (total_bytes <= 0){
            if (errno == EINTR){
                printf("chegou v4 %d \n", sizeof(aux2));
                continue; // Interrupções de sinal
            }
            perror("Erro na leitura do named pipe");
            printf("\nERRO NO PIPE, A ENCERRAR...\n");
            close(ptd->help.fd);
            unlink(FIFO_NAME);
            exit(EXIT_FAILURE);
        }
            
            
            for (int y = 0; y < 10; y++)
            printf("getpipe \t utilizadores ligados:%s\n",aux2.user[y].username);

        printf("total bytes???? %d \n",total_bytes);
        
        
        switch (mensagem.tipo){
            
            case 1: 
                printf("\ntipo: %d",mensagem.tipo);
                //pthread_mutex_lock(&thrmutex);
                processaEnvioMensagens(&aux2, mensagem);
                //pthread_mutex_unlock(&thrmutex);

                break;


            case 2: // login
                printf("\ntipo: %d",mensagem.tipo);
                //pthread_mutex_lock(&thrmutex);
                entradaUser(&aux2, mensagem);
                //pthread_mutex_unlock(&thrmutex);

                fflush(stdin);
                break;

            case 3://subscribe
                printf("subscribe");
                //Subscreve(&aux2, ptd);
                break;
                
            case 4: // unscribe
                printf("unscribe");
                //RemoveTopico(&aux2, ptd);
                break;

            case 5: //saida de feed
                /// <<<< COMO ENVIAR SINAL?!
                // vai ser neccessário de remover do array
                break;
            default:
                printf("ta num loop?!\n");
                break;
        }
    }
    printf("DAFUUUUUUCK=! \n");
}



int main()
{

    
    all st;

    initializeAll(&st);


    struct sigaction sa;
    sa.sa_handler = handle_signal;
    pid_t res_fork = fork();



    if (res_fork == 0)
    { // Processo Filho

        
    }
    else
    {
        // Processo Pai
        int status;
        // CRIA O FIFO
        if (mkfifo(FIFO_NAME, 0600) == -1)
        { // ve se tem erros na criacao do pipe
            if (errno != EEXIST)
            {
                perror("erro na criacao do named pipe");
                exit(EXIT_FAILURE);
            }
        }
        // abre o fifo em modo leitura
        
        
        int fd = open(FIFO_NAME, O_RDONLY);
        
        if (fd == -1)
        {
            perror("erro na abertura do named pipe para leitura");
            unlink(FIFO_NAME);
            exit(EXIT_FAILURE);
        }
        
        char buffer[BUFFER_SIZE];
        int nbytes;
        int tipo;
        fd_set read_fds;
        int max_fd = fd;
        
        // vai ler dados do pipe
        
        
        pthread_t tid[2];
        st.help.fd = fd;
        // Se o FIFO está pronto para leitura
        FD_ZERO(&st.help.read_fds);
        // Adicionar o FIFO ao conjunto de descritores
        FD_SET(st.help.fd, &st.help.read_fds);
        

        pthread_create(&tid[0], NULL, getpipemessages, &st);

        while (running)
        {

            /*char word1[25] = "", word2[25] = "";
            char comando[50];
            fgets(comando, sizeof(comando), stdin);
            comando[strcspn(comando, "\n")] = '\0';
            char *token = strtok(comando, " ");
            if (token != NULL)
            {
                strncpy(word1, token, sizeof(word1) - 1);
                word1[sizeof(word1) - 1] = '\0';

                token = strtok(NULL, " ");
                if (token != NULL)
                {
                    strncpy(word2, token, sizeof(word2) - 1);
                    word2[sizeof(word2) - 1] = '\0';
                }
            }
            if (strcmp(word1, "close") == 0)
            {
                break;
            }
            if (strcmp(word1, "lock") == 0)
            {
                if (strlen(word2) != 0)
                {
                    lockTopic(word2, &st);
                }
                else
                    printf("\nERRO na sintaxe");
            }

            if (strcmp(word1, "unlock") == 0)
            {
                if (strlen(word2) != 0)
                {
                    unlockTopic(word2, &st);
                }
                else
                    printf("\nERRO na sintaxe");
            }

            if(strcmp(word1, "topics") == 0){
                listTopics(&st);
            }
            if(strcmp(word1, "users") == 0){
                listUsers(&st);
            }*/
            

        }

        printf("A fechar...");
        close(fd);
        unlink(FIFO_NAME);
    }

}