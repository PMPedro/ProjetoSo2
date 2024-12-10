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
     //printf("\n <MSG> %s", ptd->msg.message);

}

void entradaUser(all *aux , all *main){
    printf("entra no veri user no manager");
    //all aux;
    bool baux = true;
    int pos;
    //verifica se o user ja esta logado 
    for(int i = 0; i < 10; i ++ ){ 
        if(strcmp(main->user[i].username, aux->user[0].username) == 0){
                baux = false;     
                break;  
        }
    }
    //verifica se ele se pode logar
    for(int i = 0; i < 10; i ++ ){ 
        if(strcmp(main->user[i].username, "\0") == 0){
            baux = true;
            strcpy(main->user[i].username , aux->user[0].username);
            strcpy(main->user[i].rcvpipename , aux->user[0].rcvpipename);  
            pos = i;
            break;
        }else
            baux = false;
        
    }
    
 

    // Abrir o FIFO para escrita
    printf("<manager>REENVIAR DADOS PARA %s" ,main->user[pos].rcvpipename );
    int fd = open(main->user[pos].rcvpipename, O_WRONLY);
    if (fd == -1)
    {
        perror("Erro na abertura do named pipe para escrita");
        unlink(FIFO_NAME);
        exit(EXIT_FAILURE);
    }
    char fts[10];
    if(baux == false) strcpy(fts,"false" ) ;
            else strcpy(fts,"true");
     int bytesn = write(fd, &fts , sizeof(fts));


}


void *getpipemessages(void *pall)
{
    all *ptd = (all *)pall;
    all aux , aux2; 
    aux.help.fd = ptd->help.fd;

    while (1)
    {
        int nbytes = read(ptd->help.fd, &aux2, sizeof(aux2));
        ptd->help.fd = aux.help.fd;
        if (nbytes != sizeof(aux2))
        {
            fprintf(stderr, "Erro: Dados incompletos recebidos\n");
        }
        if (nbytes == -1)
        {
            if (errno != EINTR)
            {
                perror("ocorreu um erro na leitura do named pipe");
            }
            printf("\nERRO NO PIPE, A ENCERRAR...");
            memset(&aux2, 0, sizeof(aux2));
            close(ptd->help.fd);
            unlink(FIFO_NAME);
            exit(EXIT_FAILURE);
        }
      //  printf("\n-> %s", ptd->msg.message);
      printf("%d" , aux2.tipo);
      if(aux2.tipo == 1){
        listMessage(&aux2, ptd);
      }
      if(aux2.tipo == 2){
        entradaUser(&aux2, ptd);
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