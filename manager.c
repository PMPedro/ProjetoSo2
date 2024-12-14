#include "funcoesHelper.c"
#define FIFO_NAME "mainpipe"
int running = 1;

void handle_signal(int sig)
{
    printf("Adeus!\n");
    running = 0;
    // close(fd);
    unlink(FIFO_NAME);
}
void listMessage(all *aux, all *main)
{
    // printf("\n <MSG> %s", aux->topico[0].msg.message);
}

void entradaUser(all *aux, all *main)
{
    bool verifica = true;
    char resposta[20];

    bool space = false, alExist = false;
    int posicao = -1;

    // Verificar se o usuário já está logado
    for (int i = 0; i < 10; i++)
    {

        if (strcmp(main->user[i].username, aux->user[0].username) == 0)
        {
            printf("[MANAGER] Usuário já logado: %s\n", aux->user[0].username);
            verifica = false;
            break;
        }
    }
    if (verifica)
    {
        // Verificar espaço para logar
        for (int i = 0; i < 10; i++)
        {
            if (strlen(main->user[i].username) == 0)
            {

                strcpy(main->user[i].username, aux->user[0].username);
                strcpy(main->user[i].rcvpipename, aux->user[0].rcvpipename);
                posicao = i;
                break;
            }
        }
    }

    if (posicao == -1)
    {
        printf("[MANAGER] Não há espaço para novos usuários.\n");
        verifica = false;
    }

    // Enviar resposta para o cliente
    int fd = open(aux->user[0].rcvpipename, O_WRONLY);
    if (fd == -1)
    {
        perror("[MANAGER] Erro ao abrir pipe de resposta");
        return;
    }

    if (verifica)
    {
        strncpy(resposta, "true", sizeof(resposta) - 1);
    }
    else
    {
        strncpy(resposta, "false", sizeof(resposta) - 1);
    }

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
    all aux, aux2;
    memset(&aux2, 0, sizeof(aux2));
    memset(&aux, 0, sizeof(aux2));
    aux.help.fd = ptd->help.fd;
    int total_bytes;

    while (1)
    {
        total_bytes = 0;
        while (total_bytes < sizeof(aux2))
        {
            int nbytes = read(ptd->help.fd, ((char *)&aux2) + total_bytes, sizeof(aux2) - total_bytes);
            if (nbytes <= 0)
            {
                if (errno == EINTR)
                    continue; // Interrupções de sinal
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
        printf("\n TIPO %d", aux2.tipo);
        if (aux2.tipo == 1)
        {
            listMessage(&aux2, ptd);
            printf("entra na msg");
        }
        if (aux2.tipo == 2)
        {
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

      /*  int r = execl("feed", "feed", NULL);
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
        initializeAll(&st);
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
            }
            

        }

        printf("A fechar...");
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