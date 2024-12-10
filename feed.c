#include "helper.h"
#define FIFO_NAME "mainpipe"
#define RCVPIPENAME "pipe%d"

int main(int agrc, char *agrv[])
{
    all st;
    char username[100] = {0};
    char rcvpipename[100];
    fflush(stdout);
    int fd2;

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
    pid_t pid = getpid();                                         // ID do processo do usuário
    snprintf(rcvpipename, sizeof(rcvpipename), RCVPIPENAME, pid); // Cria o nome do pipe com PID
    printf("<feed> 1 ");
    if (mkfifo(rcvpipename, 0666) == -1)
    { // ve se tem erros na criacao do pipe
        if (errno != EEXIST)
        {
            perror("erro na criacao do named pipe");
            exit(EXIT_FAILURE);
        }
    }
    printf("<feed> 2 ");
    // abre o fifo em modo leitura
    fd2 = open(rcvpipename, O_RDWR);
    // st.user->thd.fd = fd2;
    printf("<feed> 3 ");

    if (fd2 == -1)
    {
        perror("erro na abertura do named pipe para leitura");
        unlink(FIFO_NAME);
        exit(EXIT_FAILURE);
    }
    printf("<feed> 4 ");
    if (agrc < 2)
    {
        // pedir dados ao user
        
        fflush(stdin);   
        printf("\nDiga o seu nome de utilizador :  ");
        scanf("%99s", username); // Limite o tamanho para evitar estouro

        printf("%s", username);
    }
    else
    {
        strcpy(username, agrv[1]);
    }
    printf("<feed> 4 ");
    memset(&st, 0, sizeof(st));

    // preencher struct com dados user e do seu pipe
    strcpy(st.user[0].username, username);
    st.tipo = 2;
    strcpy(st.user[0].rcvpipename, rcvpipename);
    int bytesn;
    printf("<feed> 5 ");
    // enviar ao servidor
    bytesn = write(fd, &st, sizeof(st));
    if (bytesn == -1)
    {
        perror("Erro na escrita no named pipe");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("<feed> 6 ");

    char auxaux[10];
    int nbytesaux = read(fd2, &auxaux, sizeof(auxaux));
    if (nbytesaux != sizeof(auxaux))
    {
        fprintf(stderr, "Erro: Dados incompletos recebidos\n");
    }
    printf("<feed> 7 ");

    if (strcmp(auxaux, "false") == 0)
    {
        printf("\nERRO NA AUTENTICACAO, a encerrar...");
        exit(0);
    }
    printf("<feed> 8 ");

    // trata das msg ////////////
    ssize_t nbytes;
    char topico[300], mensagem[300], buffer[300];
    int duracao;
    printf("Entra no ciclo");
    do
    {
        // Solicitar a mensagem ao usuário
        printf("\n<MSG>-> ");
        scanf("%s", buffer);
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
        strcpy(st.topico->msg.message, mensagem);
        strcpy(st.topico->nomeTopico, topico);
        st.topico->msg.duracao = duracao;

        // Escrever a estrutura inteira no pipe
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

    } while (strcmp("sair", buffer) != 0); // Continuar até o comando "sair"

    printf("Adeus <feed> ");
    close(fd);
    return 0;
}
