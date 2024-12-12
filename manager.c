#include "helper.h"
#define FIFO_NAME "mainpipe"
int running = 1;

void handle_signal(int sig)
{
    printf("Adeus!\n");
    running = 0;
    // close(fd);
    unlink(FIFO_NAME);
}
void listMessage(all *aux , all *main){
     //printf("\n <MSG> %s", aux->topico[0].msg.message);

}

void entradaUser(all *aux, all *main)
{
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
    }

    close(fd);
}


void *getpipemessages(void *pall)
{
    all *ptd = (all *)pall;
    all aux , aux2; 
    memset(&aux2, 0, sizeof(aux2));
    memset(&aux, 0, sizeof(aux2));
    aux.help.fd = ptd->help.fd;
    int total_bytes;

    while (1)
    {
        total_bytes = 0;
        while (total_bytes < sizeof(aux2)) {
            int nbytes = read(ptd->help.fd, ((char*)&aux2) + total_bytes, sizeof(aux2) - total_bytes);
            if (nbytes <= 0) {
                if (errno == EINTR) continue; // Interrupções de sinal
                perror("Erro na leitura do named pipe");
                printf("\nERRO NO PIPE, A ENCERRAR...\n");
                close(ptd->help.fd);
                unlink(FIFO_NAME);
                exit(EXIT_FAILURE);
            }
            total_bytes += nbytes;
        }
      
      
      aux2.topico[0].msg.message[sizeof(aux2.topico[0].msg.message) - 1] = '\0';

       printf("\n <MSG> %s", aux2.topico[0].msg.message);
      printf("\n TIPO %d" , aux2.tipo);
      if(aux2.tipo == 1){
        listMessage(&aux2, ptd);
       printf("entra na msg");
      }
      if(aux2.tipo == 2){
        entradaUser(ptd, ptd);
      }
    }
}




int main()
{
    /*
    #########
    !!!!!!!!!!!!!!! AS MENSAGENS DOS TOPICOS TERÃO QUE INCLUIR ESPAÇOS!!!
    #########
    */
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    pid_t res_fork = fork();

    if (res_fork == 0)
    { // Processo Filho

        int r = execl("feed", "feed", NULL);
        if (r == -1)
        {
            perror("Erro a Executar");
        }
    }
    else
    {
        // Processo Pai
        int status;
        // CRIA O FIFO
        if (mkfifo(FIFO_NAME, 0666) == -1)
        { // ve se tem erros na criacao do pipe
            if (errno != EEXIST)
            {
                perror("erro na criacao do named pipe");
                exit(EXIT_FAILURE);
            }
        }
        // abre o fifo em modo leitura
        int fd = open(FIFO_NAME, O_RDWR);
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
        all st;
        // vai ler dados do pipe

        pthread_t tid[2];
        st.help.fd = fd;
        // Se o FIFO está pronto para leitura
        FD_ZERO(&st.help.read_fds);
        // Adicionar o FIFO ao conjunto de descritores
        FD_SET(st.help.fd, &st.help.read_fds);

        pthread_create(&tid[0], NULL, getpipemessages, &st);

        while (running)
        { // ler dados do pipe
            char aux[20];
            scanf("%s", &aux);
            // printf("(TESTE)%s", aux);
            // Se o FIFO está pronto para leitura
            FD_ZERO(&read_fds);
            // Adicionar o FIFO ao conjunto de descritores
            FD_SET(fd, &read_fds);

            //  nbytes = read(fd, &tipo, sizeof(tipo)); //erro aqui
            //  printf("NBTES -> %d\n", nbytes);
            /*if (nbytes == -1)
            {
                if (errno != EINTR)
                {
                    perror("ocorreu um erro na leitura do named pipe");
                }
                close(fd);
                unlink(FIFO_NAME);
                exit(EXIT_FAILURE);
            }
            else if (nbytes == 0)
            {
                printf("Esperando por dados ... \n");
                usleep(500000);

            }
            else
            {
                printf("TESTE -> %d \n", tipo);

            } */
        }
        close(fd);
        unlink(FIFO_NAME);
    }

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