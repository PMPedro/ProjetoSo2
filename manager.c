//#include "helper.h"
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
#pragma
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
   
    
}all;

typedef struct{
    char user[100];
    char message[300];
    char nometopico[100];
    char pipe[30];
    int tipo;
}Mensagem;






void listMessage(all *aux, all *main)
{
    // printf("\n <MSG> %s", aux->topico[0].msg.message);
}



void processaEnvioMensagens(all *estruturaprincipal, all dadosaenviar){
    all *ptd = (all *) estruturaprincipal;
    

    printf("ENTROU\n");
    fflush(stdout);

    //printf("\ntamanho: %d\n",sizeof(&ptd->topico[0].nomeTopico));fflush(stdout);
    
    //aux2.topico[i].nomeTopico

    for(int i = 0;i <20; i++){

        if( strcmp(&ptd->topico[i].nomeTopico, ".") == 0 ){
            //printf("encontrou slot vazia\n");
            strcpy(&ptd->topico[i].nomeTopico, dadosaenviar.topico[0].nomeTopico) ;
            //printf("\nall topico %s\n", &ptd->topico[i].nomeTopico);
            //fflush(stdout);
            return;
        }else if ( strcmp(&ptd->topico[i].nomeTopico, ".") != 0){ // se encontrar um topico diferente, quebra o ciclo e procura enviar a mensagem
            //printf("encontrou um topico diferente!!!!");
            //fflush(stdout);
            break;
        }

    }
        
    for (int xusers = 0 ; xusers < 10; xusers++){
        printf("\n &ptd->user[xusers].username: %s\n", &ptd->user[xusers].username);
        fflush(stdout);
        if ( strcmp ( &ptd->user[xusers].username, dadosaenviar.user[0].username ) != 0){
            for (int yuserregisteredtopics = 0; yuserregisteredtopics < 20; yuserregisteredtopics++){

            //compara se o nome do utilizador xusers é igual ao que enviou a mensagem
                
                if ( strcmp(  &ptd->user[xusers].topicosInscritos[yuserregisteredtopics].nomeTopico, dadosaenviar.topico[0].nomeTopico ) == 0 ){
                    printf("entrou para enviar mensagem!!!!!!!!\n");
                    fflush(stdout);
                }  

            } // 
            
    //write();

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




void entradaUser(all *aux, all *main){
    printf("[MANAGER] Verificando autenticação de usuário...\n");

    bool podeLogar = false;
    int posicao = -1;

    // Verificar se o usuário já está logado
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(main->user[i].username, aux->user[0].username) == 0)
        {
            printf("[MANAGER] Usuário já logado: %s\n", aux->user[0].username);
            podeLogar = false;
            break;
        }
    }

    // Verificar espaço para logar
    for (int i = 0; i < 10; i++)
    {
        if (main->user[i].username[0] == '\0')
        {
            podeLogar = true;
            strcpy(main->user[i].username, aux->user[0].username);
            strcpy(main->user[i].rcvpipename, aux->user[0].rcvpipename);
            posicao = i;
            break;
        }
    }

    if (posicao == -1)
    {
        printf("[MANAGER] Não há espaço para novos usuários.\n");
        podeLogar = false;
    }
    printf("\n<manager> pipe de login: %s\n", aux->user[0].rcvpipename);

    // Enviar resposta para o cliente
    int fd = open(aux->user[0].rcvpipename, O_WRONLY);
    printf("abriu indevidamente???");
  
    if (fd == -1)
    {
        perror("[MANAGER] Erro ao abrir pipe de resposta");
        return;
    }

    char resposta[10] = {0};
    strcpy(resposta, podeLogar ? "true" : "false");

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

        //total_bytes = read(ptd->help.fd, &aux2 , sizeof(all) );
        total_bytes = read(ptd->help.fd, &mensagem , sizeof(Mensagem) );

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
            
            
        

        printf("total bytes???? %d \n",total_bytes);
        
        
        switch (mensagem.tipo){
            
            case 1: 
                printf("\ntipo: %d",aux2.tipo);
                processaEnvioMensagens(ptd, aux2);
                break;


            case 2: // login
                printf("\ntipo: %d",aux2.tipo);
                entradaUser(&aux2, ptd);
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
    /*
    #########
    !!!!!!!!!!!!!!! AS MENSAGENS DOS TOPICOS TERÃO QUE INCLUIR ESPAÇOS!!!
    #########
    */
    
    all st;
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    pid_t res_fork = fork();

    /*for (int i = 0; i<20; i++){
            strcpy(st.topico[i].nomeTopico,".");
    }*/
    

    /*for (int x = 0; x<10 ; x++){
        strcpy(st.user[x].rcvpipename,".");
        strcpy(st.user[x].username,".");
    }*/


    if (res_fork == 0)
    { // Processo Filho

        /*int r = execl("feed", "feed", NULL);
        if (r == -1)
        {
            perror("Erro a Executar");
        }*/
        
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
/*
            char word1[25] = "", word2[25] = "";
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
    printf("acabou?");
    /*
    ##############
    GUARDAR MENSAGENS PERMANENTES QUE AINDA TENHAM TEMPO DE VIDA, EM FICHEIRO!!!
    SERÁ FICHEIRO TEXTO!!!!!
    FORMATO DO FICHEIRO:
    <NOME TOPICO> <USERNAME> <TEMPO DE VIDA RESTANTE> <CORPO DE MENSAGEM>

    O NOME TOPICO, USER E TEMPO DE VIDA CONTARÃO COMO UMA ÚNICA PALAVRA!!! USAR SPRINTF!!!

    OBRIGATORIO : O NOME DO FICHEIRO SERÁ GUARDADO NUMA VARIAVEL DE AMBIENTE MSG_FICH

    #######
     */
}