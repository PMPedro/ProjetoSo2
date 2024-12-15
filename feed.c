#include "helper.h"
#pragma

#define _FIFO_NAME "mainpipe"
#define RCVPIPENAME "pipe%d"


typedef struct{
  int continuar;
  //bool logado = false;
  char fifoname[30];
  pthread_t threadid;
  pthread_mutex_t *ptrinco;
  
}THREADDATA;




void *WriteToPipe(void *pdata){
  
  THREADDATA *pthreaddata = (THREADDATA *) pdata;
  //char *mensagem[];
  all message, st; 


  bool logado = false;

  int fdmainpipe;

  char buffer[500];
  char *command;
  char *leftover;

  char username[50];
  char topico[20];
  char mensagem[300];
  int duracao;

  


  strncpy(pthreaddata->fifoname, _FIFO_NAME,sizeof(pthreaddata->fifoname)-1);
  mkfifo(pthreaddata->fifoname,0666);
  fdmainpipe = open(pthreaddata->fifoname, O_WRONLY);

  if(fdmainpipe){
        perror("Erro na abertura do named pipe para escrita");
        unlink(pthreaddata->fifoname);
        exit(EXIT_FAILURE);
    }



















  while(pthreaddata->continuar){
    if (!logado){
      printf("Insira o seu nome: ");
      fgets( username,sizeof(username), stdin ); // Limite o tamanho para evitar estouro
      
      printf("\nO seu nome de utlizador é: %s", username);
      write(fdmainpipe, &username,sizeof(username));
      fflush(stdout);
      fflush(stdin);
    }else{


      printf("O que deseja fazer?\t ");
      //scanf("%s",mensagem);
      fgets(buffer, sizeof(buffer), stdin);
      // Remove o caractere '\n' que fgets pode adicionar
      buffer[strcspn(buffer, "\n")] = '\0';
      //command= strtok(mensagem, " ")





      if ( strcmp(command,"topics") ){
        
        write(fdmainpipe, &command,sizeof(command));


      }else if ( strcmp(command,"exit") ){
        write(fdmainpipe, &command,sizeof(command));
        //sair
      }else if ( strcmp(command,"subscribe") ){

        //write(fdmainpipe, &command,sizeof(command));


      }else if ( strcmp(command,"unscribe") ){

        write(fdmainpipe, &command,sizeof(command));

      }else if ( sscanf(buffer, "msg %s %d %[^n]", topico, &duracao, mensagem) == 3 ){
        
        for (int i = 0 ; i<20; i++){//percore numero de topicos
          //if(strlen(message.topico[i].nomeTopico) == 0) <<< VAI DAR JEITO PARA O MANAGER
          if ( strcmp(topico, message.topico[i].nomeTopico) ){

            if ( duracao < 0 ){
              printf("duracao invalida!");
              fflush(stdout);
              break;

            }else{
              memset(&message, 0, sizeof(message));

              strcpy(message.topico[i].msg.message, buffer);
              strcpy(message.topico->nomeTopico, topico);

              message.topico->msg.duracao = duracao;
              message.tipo = 1;

              printf("\nMSG escrita -> %s", message.topico[i].msg.message);
              printf("\nMSG escritaBuffer -> %s \n", buffer);
              size_t message_size = sizeof(message);
              int n_bytes;
              n_bytes = write(fdmainpipe,&message,sizeof(message));

              if (n_bytes == -1){
                perror("Erro na escrita no named pipe");
                close(fdmainpipe);
                exit(EXIT_FAILURE);
              }
            }
          }         
        }

      }else{
        printf("INVALIDO!!!!");
        fflush(stdout);
      }
    }
  }
}



void *ReadFromPipe(void *pdata){
  THREADDATA *pthreaddata = (THREADDATA *) pdata;


  /**#######
   vai ser necessário meter o pthreaddata->all->user->rcvname = RCVPIPENAME
  ##### 
   */


  char buffer[100];
  int fdrcvpipe, nbytes;

  if (access(_FIFO_NAME, F_OK) != 0){
    printf("O Manager nao se encontra em execucao!\n");
    exit(0);
  }

  
  //mkfifo(pthreaddata->fifoname,O_RDONLY);
  fdrcvpipe = open (pthreaddata->fifoname,0600);
  
  printf("recepcao??????\n");

  while(pthreaddata->continuar){
    pthread_mutex_lock(pthreaddata->ptrinco);
    nbytes = read( fdrcvpipe, buffer, sizeof(buffer)- 1);

    if (nbytes > 0) {

      buffer[nbytes] = '\0';
      printf("Feed leitura ---- meter aqui PID do feed  ----- \t Recebeu a seguinte mensagem: %s",buffer);

      if( strcmp( buffer,"sai") == 0 ){
          pthread_kill(pthreaddata->threadid, SIGUSR1);
      }


      perror("ERRO de leitura: Feed leitura ---- meter aqui PID do feed  ----- ");
      break;

    }
  }

  close(fdrcvpipe);
  unlink(pthreaddata->fifoname);

  pthread_exit(NULL);

}


void QuitFeed(int num, siginfo_t *si, void *uc){
    printf("recebeu sinal!!!!");
    return;
}





int main(int argc, char *agrv[]){
    
  char topico[300], mensagem[300], buffer[300];
  int duracao;
  int fdmainpipe, fdrcvpipe;
  int nbytes;
    
  all st; //estrutura principal
  THREADDATA thrdatasender, thrdatareceiver;
  char username[100];
  char rcvpipename[100];


  //##### THREADS & THREAD MUTEX
  pthread_mutex_t trinco;
  pthread_t readpipethread, writepipethread;
    
    
  //##### SIGNAL
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = QuitFeed;
  sigaction(SIGUSR1, &sa, NULL);
  //############

  int continuar;
  //bool logado = false;
  char fifoname[30];
  pthread_t threadid;
  pthread_mutex_t *ptrinco;
  
  pid_t pidreceiver = getpid(); 
  
  thrdatareceiver.threadid = pthread_self();
  thrdatasender.threadid = pthread_self();
  
  snprintf(rcvpipename, sizeof(rcvpipename), RCVPIPENAME, pidreceiver);
  strcpy(st.user[0].rcvpipename, rcvpipename);
  
  strcpy(thrdatareceiver.fifoname,RCVPIPENAME);


  //###### PIPES

  if (access(_FIFO_NAME, F_OK) != 0)
  {
    printf("O Manager nao se encontra em execucao!\n");
    exit(0);
  }

  // Abrir o FIFO para escrita
  int fd = open(_FIFO_NAME, O_WRONLY);
  if (fd == -1)
  {
    perror("Erro na abertura do named pipe para escrita");
    unlink(_FIFO_NAME);
    exit(EXIT_FAILURE);
  }
  

    fflush(stdout);
  //////trata da entrada do user///////////
  // prepara-se para criar pipe de leitura
  // PARTE COM ERROS COMECA AQUI

  
  fflush(stdout);
  pid_t pid = getpid();
  snprintf(rcvpipename, sizeof(rcvpipename), RCVPIPENAME, pid); // Cria o nome do pipe com PID
  
  fflush(stdout);
  if (mkfifo(rcvpipename, 0666) == -1)
  { // ve se tem erros na criacao do pipe
    if (errno != EEXIST)
    {
      perror("erro na criacao do named pipe");
      exit(EXIT_FAILURE);
    }
  }
  
  fflush(stdout);
  // abre o fifo em modo leitura
  printf("|%s|", rcvpipename); // ERRO A PARTIR DAQUI
  fflush(stdout);
  if (access(rcvpipename, F_OK) != 0) // VERIFICA SE O PIPE EXISTE
  {
    printf("ERRO a aceder a pipe %s!\n", rcvpipename);
    exit(0);
  }

  int fd2 = open(rcvpipename, O_RDWR);

  // st.user->thd.fd = fd2;
  
  fflush(stdout);

  if (fd2 == -1)
  {
    perror("erro na abertura do named pipe para leitura");
    unlink(_FIFO_NAME);
    exit(EXIT_FAILURE);
  }
  



  //########################################################3
  //##################### LOGIN 
  fflush(stdout);
  fflush(stdin);
  if (argc < 2){
      
    // pedir dados ao user
    printf("\nDiga o seu nome de utilizador :  ");
    scanf("%s", username); // Limite o tamanho para evitar estouro
    printf("\nO seu nome de utlizador é: %s", username);
      
    }else{

      strcpy(username, agrv[1]);

    }


  printf("<feed> 4.5 ");
  fflush(stdout);
  memset(&st, 0, sizeof(st));

  // preencher struct com dados user e do seu pipe
  strcpy(st.user[0].username, username);
  st.tipo = 2;
  
  int bytesn;
  printf("<feed> 5 ");
  fflush(stdout);

  // envia nome utilizador ao manager
  bytesn = write(fdmainpipe, &st, sizeof(st));
  if (bytesn == -1){
    perror("Erro na escrita no named pipe");
    close(fdmainpipe);
    unlink(_FIFO_NAME);
    exit(EXIT_FAILURE);
  }
    printf("<feed> 6 ");
    fflush(stdout);
  fflush(stdin);

  //strncpy(pthreaddata->fifoname, _FIFO_NAME,sizeof(pthreaddata->fifoname)-1);
  pthread_create(&readpipethread, NULL, ReadFromPipe, (void *) &thrdatareceiver);
  //pthread_create(&writepipethread,NULL, WriteToPipe,(void *) &thrdatasender);


  //####################################################3

    do{
        // Solicitar a mensagem ao usuário
        printf("\n<MSG>-> ");
        fgets(buffer, sizeof(buffer), stdin);
        // Remove o caractere '\n' que fgets pode adicionar
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("o buffer é: %s",buffer);
          if (sscanf(buffer, "msg %s %d %[^\n]", topico, &duracao, mensagem) == 3)
          {
              printf("Tópico: %s\n", topico);
              printf("Duração: %d\n", duracao);
              printf("Mensagem: %s\n", mensagem);
          }
          else
          {
              printf("Formato inválido. Tente novamente.\n");
          }
        // Preencher a estrutura com a mensagem
        memset(&st, 0, sizeof(st));
        strcpy(st.topico[0].msg.message, buffer);
        strcpy(st.topico[0].nomeTopico, topico);
        printf("\nnome do topico: %s",st.topico->nomeTopico);
        st.topico->msg.duracao = duracao;
        st.tipo = 1;
        printf("\nMSG escrita -> %s", st.topico[0].msg.message);
        printf("\nMSG escritaBuffer -> %s \n", buffer);

      /*
      ##### faço aqui a escrita para o pipe
      
       */


        // Escrever a estrutura inteira no pipe
        size_t message_size = sizeof(st);
        nbytes = write(fdmainpipe, &st, sizeof(st));
        if (nbytes == -1)
        {
            perror("Erro na escrita no named pipe");
            close(fdmainpipe);
            exit(EXIT_FAILURE);
        }

        // Verificar se a quantidade de dados escritos é a esperada
        if (nbytes != sizeof(st))
        {
            fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
        }

    } while (strcmp("sair", buffer) != 0); // Continuar até o comando "sair"






    // trata do Pipe principal

    // Verificar se o FIFO (named pipe) está disponível
    /*if (access(_FIFO_NAME, F_OK) != 0)
    {
        printf("O Manager nao se encontra em execucao!\n");
        exit(0);
    }

    // Abrir o FIFO para escrita
    int fd = open(_FIFO_NAME, O_WRONLY);
    if (fd == -1)
    {
        perror("Erro na abertura do named pipe para escrita");
        unlink(_FIFO_NAME);
        exit(EXIT_FAILURE);
    }
    printf("Faz1");
    fflush(stdout);
    //////trata da entrada do user///////////
    // prepara-se para criar pipe de leitura
    //PARTE COM ERROS COMECA AQUI
    
    printf("<feed> 0 ");
    fflush(stdout);
    pid_t pid = getpid();                                       
    snprintf(rcvpipename, sizeof(rcvpipename), RCVPIPENAME, pid); // Cria o nome do pipe com PID
    printf("<feed> 1 ");
    fflush(stdout);
    if (mkfifo(rcvpipename, 0666) == -1){ // ve se tem erros na criacao do pipe
        
        if (errno != EEXIST){
          perror("erro na criacao do named pipe");
          exit(EXIT_FAILURE);
        }

      }


    printf("<feed> 2 ");
    fflush(stdout);
    // abre o fifo em modo leitura e escrita
    printf("|%s|" , rcvpipename); //ERRO A PARTIR DAQUI 
    fflush(stdout);
    if (access(rcvpipename, F_OK) != 0){  //VERIFICA SE O PIPE EXISTE
        printf("ERRO a aceder a pipe %s!\n",rcvpipename);
        exit(0);
      }


    int fd2 = open(rcvpipename, O_RDWR);
         
    // st.user->thd.fd = fd2;
    printf("<feed> 3 ");
    fflush(stdout);

    if (fd2 == -1){
      perror("erro na abertura do named pipe para leitura");
      unlink(_FIFO_NAME);
      exit(EXIT_FAILURE);
    }


    printf("<feed> 4 ");
    fflush(stdout);
    fflush(stdin);
    if (agrc < 2){
      
      // pedir dados ao user
      printf("\nDiga o seu nome de utilizador :  ");
      scanf("%s", username); // Limite o tamanho para evitar estouro
      printf("\nO seu nome de utlizador é: %s", username);
      
      }else{

        strcpy(username, agrv[1]);

      }


    printf("<feed> 4.5 ");
    fflush(stdout);
    memset(&st, 0, sizeof(st));

    // preencher struct com dados user e do seu pipe
    strcpy(st.user[0].username, username);
    st.tipo = 2;
    strcpy(st.user[0].rcvpipename, rcvpipename);
    int bytesn;
    printf("<feed> 5 ");
    fflush(stdout);

    // envia nome utilizador ao manager
    bytesn = write(fd, &st, sizeof(st));
    if (bytesn == -1){
      perror("Erro na escrita no named pipe");
      close(fd);
      unlink(_FIFO_NAME);
      exit(EXIT_FAILURE);
    }
    printf("<feed> 6 ");
    fflush(stdout);

        char auxaux[10];


        //int nbytesaux = read(fd2, &auxaux, sizeof(auxaux));
        int total_bytes = 0;
        int nbytesaux;
        while (total_bytes < sizeof(auxaux)) {
                 nbytesaux = read(fd2, ((char*)&auxaux) + total_bytes, sizeof(auxaux) - total_bytes);
                if (nbytesaux <= 0) {
                    if (errno == EINTR) continue; // Interrupções de sinal
                    perror("Erro na leitura do named pipe");
                    printf("\nERRO NO PIPE, A ENCERRAR...\n");
                    close(fd2);
                    unlink(_FIFO_NAME);
                    exit(EXIT_FAILURE);
                }
                total_bytes += nbytesaux;
            }



        if (nbytesaux != sizeof(auxaux))
        {
            fprintf(stderr, "Erro: Dados incompletos recebidos\n");
        }
        printf("<feed> 7 ");
          fflush(stdout);
        if (strcmp(auxaux, "false") == 0)
        {
            printf("\nERRO NA AUTENTICACAO, a encerrar...");
            exit(0);
        }
        printf("<feed> 8 ");
  fflush(stdout);*/
    

    //ACABA AQUI ERRO
    //MAIS VERIFICAR FUNCAO entradaUser()  no manager

    // trata das msg ////////////
   /* ssize_t nbytes;
    char topico[300], mensagem[300], buffer[300];
    int duracao;
    printf("Entra no ciclo");
    do
    {
        // Solicitar a mensagem ao usuário
        printf("\n<MSG>-> ");
        fgets(buffer, sizeof(buffer), stdin);
        // Remove o caractere '\n' que fgets pode adicionar
        buffer[strcspn(buffer, "\n")] = '\0';

        /*  if (sscanf(buffer, "msg %s %d %[^\n]", topico, &duracao, mensagem) == 3)
          {
              printf("Tópico: %s\n", topico);
              printf("Duração: %d\n", duracao);
              printf("Mensagem: %s\n", mensagem);
          }
          else
          {
              printf("Formato inválido. Tente novamente.\n");
          }
        // Preencher a estrutura com a mensagem
        memset(&st, 0, sizeof(st));
        strcpy(st.topico[0].msg.message, buffer);
        strcpy(st.topico->nomeTopico, topico);
        st.topico->msg.duracao = duracao;
        st.tipo = 1;
        printf("\nMSG escrita -> %s", st.topico[0].msg.message);
        printf("\nMSG escritaBuffer -> %s \n", buffer);

        // Escrever a estrutura inteira no pipe
        size_t message_size = sizeof(st);
        nbytes = write(fd, &st, sizeof(st));
        if (nbytes == -1)
        {
            perror("Erro na escrita no named pipe");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Verificar se a quantidade de dados escritos é a esperada
        if (nbytes != sizeof(st))
        {
            fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
        }

    } while (strcmp("sair", buffer) != 0);*/ // Continuar até o comando "sair"

    printf("Adeus <feed> ");
    //close(fd);
    return 0;
}
