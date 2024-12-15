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

  char buffer[100];
  int fdrcvpipe, nbytes;

  if (access(FIFO_NAME, F_OK) != 0)
  {
    printf("O Manager nao se encontra em execucao!\n");
    exit(0);
  }

  // mkfifo(pthreaddata->fifoname,O_RDONLY);
  // fdrcvpipe = open (pthreaddata->fifoname,0600);

  while (pthreaddata->continuar == 1)
  {

    // pthread_mutex_lock(pthreaddata->ptrinco);
    nbytes = read(pthreaddata->fifoname, buffer, sizeof(buffer) - 1);

    // pthread_mutex_unlock(pthreaddata->ptrinco);
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
  unlink(pthreaddata->fifoname);

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

  Mensagem message;

  THREADDATA thrdatareceiver;
  char username[100];
  char rcvpipename[100];

  thrdatareceiver.continuar = 1;

  // ##### THREADS & THREAD MUTEX
  pthread_mutex_t trinco;
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

  fflush(stdout);

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
  printf("pipe: |%s|\n", rcvpipename); // ERRO A PARTIR DAQUI
  fflush(stdin);
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
    // Remove o caractere '\n' que fgets pode adicionar
    buffer[strcspn(buffer, "\n")] = '\0';
    printf("o buffer é: %s", buffer);

    /*if (sscanf(buffer, "msg %s %d %[^\n]", topico, &duracao, mensagem) == 3)
    {
        printf("Tópico: %s\n", topico);
        printf("Duração: %d\n", duracao);
        printf("Mensagem: %s\n", mensagem);
    }
    else
    {
        printf("Formato inválido. Tente novamente.\n");
    }*/

    // Preencher a estrutura com a mensagem
    // memset(&st, 0, sizeof(st));
    strcpy(message.message, "mensagemteste");
    strcpy(message.nometopico, "topico");
    printf("\nnome do topico: %s", message.nometopico);
    // st.topico->msg.duracao = duracao;
    message.tipo = 1;
    printf("\nMSG escrita no message-> %s", message.message);
    printf("\nMSG escritaBuffer -> %s \n", buffer);

    /*
    ##### faço aqui a escrita para o pipe

     */

    // Escrever a estrutura inteira no pipe

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
