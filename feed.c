#include "helper.h"
#define FIFO_NAME "mainpipe"

/*
#####

FAZER VERIFICAÇÃO AO INICIO SE O MANAGER ESTÁ LIGADO!!!!!


*/

/*
User vai interagir atraves de codigos
 -Enviar mensagem para um determinado tópico: msg <topico> <duração> <mensagem>
- Subscrever um determinado tópico: subcribe <topico>
- Deixar de subscrever um determinado tópico: unsubcribe <topico>
-Sair, encerrando o processo feed: comando exit
*/

int main()
{
    tipoMsg msg;
    msg.tipo = 1;
 
    if (access(FIFO_NAME, F_OK) != 0)
    {
        printf("O Manager nao se encontra em execucao!\n");
        exit(0);
    }

    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1)
    {
        perror("erro na abertura do named pipe para leitura");
        unlink(FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    ssize_t nbytes = write(fd, &msg, sizeof(msg)); 
    if (nbytes == -1)
    {
        perror("Erro na escrita no named pipe");
        close(fd);
        exit(EXIT_FAILURE);
    }
     close(fd);
}
