#include "helper.h"

#pragma

#define FIFO_NAME "mainpipe"
#define RCVPIPENAME "pipe%d"

typedef struct
{
  int continuar;
  // bool logado = false;
  char fifoname[30];
  pthread_t threadid;
  pthread_mutex_t *ptrinco;

} THREADDATA;

void *ReadFromPipe(void *pdata)
{
  THREADDATA *pthreaddata = (THREADDATA *)pdata;
  
  
  /**#######
   vai ser necessário meter o pthreaddata->all->user->rcvname = RCVPIPENAME
  #####
   */

  char buffer[300];
  int fdrcvpipe, nbytes;

  if (access(FIFO_NAME, F_OK) != 0)
  {
    printf("O Manager nao se encontra em execucao!\n");
    exit(0);
  }

  mkfifo(RCVPIPENAME,0600);
  fdrcvpipe = open (RCVPIPENAME,O_RDWR);
  printf("ta aqui?");
  while (pthreaddata->continuar == 1)
  {

    printf("e aqui?");
    nbytes = read(fdrcvpipe, buffer, sizeof(buffer) - 1);
    
    
    if (nbytes > 0)
    {

      buffer[nbytes] = '\0';
      printf("Feed leitura ---- meter aqui PID do feed  ----- \t Recebeu a seguinte mensagem: %s", buffer);

      if (strcmp(buffer, "sai") == 0)
      {
        pthread_kill(pthreaddata->threadid, SIGUSR1);
      }

      perror("ERRO de leitura: Feed leitura ---- meter aqui PID do feed  ----- ");
      break;
    }
  }
  printf("chegou onde nao devia???");
  close(fdrcvpipe);
  unlink(RCVPIPENAME);

  pthread_exit(NULL);
}

void QuitFeed(int num, siginfo_t *si, void *uc)
{
  printf("recebeu sinal!!!!");
  return;
}

int main(int argc, char *agrv[])
{

  char topico[300], mensagem[300], buffer[300];
  int duracao;
  int fdmainpipe, fdrcvpipe;
  int nbytes;
  char *token;
  char buffercopy[300];

  Mensagem message;

  THREADDATA thrdatareceiver;
  char username[100];
  char rcvpipename[100];

  thrdatareceiver.continuar = 1;

  // ##### THREADS & THREAD MUTEX
  

  pthread_t readpipethread, writepipethread;

  // ##### SIGNAL
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = QuitFeed;
  sigaction(SIGUSR1, &sa, NULL);
  // ############

  int continuar;
  // bool logado = false;
  char fifoname[30];
  pthread_t threadid;
  pthread_mutex_t *ptrinco;

  pid_t pidreceiver = getpid();

  thrdatareceiver.threadid = pthread_self();

  

  // trata do Pipe principal

  // Verificar se o FIFO (named pipe) está disponível
  if (access(FIFO_NAME, F_OK) != 0)
  {
    printf("O Manager nao se encontra em execucao!\n");
    exit(0);
  }

  // Abrir o FIFO para escrita
  int fd = open(FIFO_NAME, O_WRONLY);
  if (fd == -1)
  {
    perror("Erro na abertura do named pipe para escrita");
    unlink(FIFO_NAME);
    exit(EXIT_FAILURE);
  }

 
  //////trata da entrada do user///////////
  // prepara-se para criar pipe de leitura
  // PARTE COM ERROS COMECA AQUI

  
  pid_t pid = getpid();
  snprintf(rcvpipename, sizeof(rcvpipename), RCVPIPENAME, pid); // Cria o nome do pipe com PID

  
  if (mkfifo(rcvpipename, 0666) == -1)
  { // ve se tem erros na criacao do pipe
    if (errno != EEXIST)
    {
      perror("erro na criacao do named pipe");
      exit(EXIT_FAILURE);
    }
  }

  
  // abre o fifo em modo leitura
  printf("pipe: |%s|\n", rcvpipename); // ERRO A PARTIR DAQUI

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
    unlink(FIFO_NAME);
    exit(EXIT_FAILURE);
  }

  fflush(stdout);
  if (argc < 2)
  {

    fflush(stdout);
    // pedir dados ao user
    printf("\nDiga o seu nome de utilizador :  ");
    fgets(username, sizeof(username), stdin);
    // Remove o caractere '\n' que fgets pode adicionar
    username[strcspn(username, "\n")] = '\0';
  }
  else
  {
    strcpy(username, agrv[1]);
  }

  fflush(stdout);

  // preencher struct com dados user e do seu pipe
  strcpy(message.user, username);
  message.tipo = 2;
  strcpy(message.pipe, rcvpipename);

  int bytesn;

  fflush(stdout);
  // enviar ao servidor

  printf("tamanho all: %d ", sizeof(all));

  bytesn = write(fd, &message, sizeof(message));
  printf("byte number : %d ", bytesn);

  if (bytesn == -1)
  {
    perror("Erro na escrita no named pipe");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // int nbytesaux = read(fd2, &auxaux, sizeof(auxaux));

  int nbytesaux;

  pthread_create(&readpipethread, NULL, ReadFromPipe, (void *)&thrdatareceiver);
  // ####################################################3

  do
  {
    // Solicitar a mensagem ao usuário
    printf("\n<MSG>-> ");
    fgets(buffer, sizeof(buffer), stdin);
    strcpy(buffercopy, buffer);
    // Remove o caractere '\n' que fgets pode adicionar
    buffer[strcspn(buffer, "\n")] = '\0';
    //strcpy(buffercopy, buffer);
    printf("o buffer é: %s\n\n", buffer);
    printf("a copia é: %s", buffercopy);
    token = strtok(buffer, " ");
    //strncpy( firstword, token, sizeof(firstword) -1 );
    //printf("firstword: %s\n",firstword);

    while (token != NULL){

      if(strcmp(token, "msg") == 0 || strcmp(token, "subscribe") == 0 ||
         strcmp(token, "unsubcribe") == 0 || strcmp(token, "exit") == 0) {

          if(strcmp(token, "msg") == 0){
            if (sscanf(buffercopy, "msg %s %d %[^\n]", topico, &duracao, mensagem) == 3){
              
              strcpy(message.message, mensagem);
              strcpy(message.nometopico, topico);
              printf("\nnome do topico: %s", message.nometopico);
              // st.topico->msg.duracao = duracao;
              message.tipo = 1;
              printf("\nMSG escrita no message-> %s", message.message);
              nbytes = write(fd, &message, sizeof(Mensagem));
              if (nbytes == -1)
              {
                perror("Erro na escrita no named pipe");
                close(fd);
                exit(EXIT_FAILURE);
              }
              if (nbytes != sizeof(message))
              {
                fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
              }
              break;
            }



          }else if (strcmp(token, "subscribe") == 0){

            message.tipo = 3;
            strcpy(message.message, token);
            printf("\nnome do comando: %s", message.message);
            nbytes = write(fd, &message, sizeof(Mensagem));

            if (nbytes == -1)
            {
              perror("Erro na escrita no named pipe");
              close(fd);
              exit(EXIT_FAILURE);
            }
            if (nbytes != sizeof(message))
            {
              fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
            }
            break;

          }else if (strcmp(token, "unsubcribe") == 0){

            strcpy(message.message, token);
            message.tipo = 4;

            printf("\nnome do comando: %s", message.message);

            nbytes = write(fd, &message, sizeof(Mensagem));

            if (nbytes == -1)
            {
              perror("Erro na escrita no named pipe");
              close(fd);
              exit(EXIT_FAILURE);
            }
            if (nbytes != sizeof(message))
            {
              fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
            }
            break;

          }else if (strcmp(token, "exit")){

            strcpy(message.message, token);
            message.tipo = 5;

            printf("\nnome do comando: %s", message.message);

            nbytes = write(fd, &message, sizeof(Mensagem));

            if (nbytes == -1)
            {
              perror("Erro na escrita no named pipe");
              close(fd);
              exit(EXIT_FAILURE);
            }
            if (nbytes != sizeof(message))
            {
              fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
            }

            break;

          }

      }else{

        printf("comando invalido");
        break;

      }
    }


    nbytes = write(fd, &message, sizeof(Mensagem));

    if (nbytes == -1)
    {
      perror("Erro na escrita no named pipe");
      close(fd);
      exit(EXIT_FAILURE);
    }

    // Verificar se a quantidade de dados escritos é a esperada
    if (nbytes != sizeof(message))
    {
      fprintf(stderr, "Erro: Nem todos os dados foram enviados\n");
    }

  } while (strcmp("sair", buffer) != 0); // Continuar até o comando "sair"

  printf("Adeus <feed> ");
  // close(fd);
  return 0;
}