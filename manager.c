
#include <estrutura.h>

#define FIFO_NAME "mainpipe"

/*
#####
                O MANAGER TERÁ QUE REMOVER AS MENSAGENS PERSISTENTES!!!

                TERÁ QUE FAZER LOAD AO INICIO DO FICHEIRO DE MENSAGENS PERSISTENTES E METE-LAS EM MEMORIA!!!
                GUARDAR A INFORMACAO SUPRACITADA NUMA STRUCT COM char topic[] e int TEMPO

                TERÁ QUE SER FEITA UMA VERIFICAO DE USERS REPETIDOS, NÃO PERMITINDO A LIGACAO DE UM SEGUNDO USER COM O MESMO NOME!
####

*/

/*

Listar utilizadores atualmente a usar a plataforma: users
●Eliminar um utilizador: remove <username>
Tem o mesmo efeito que o comando exit do programa feed. O utilizador em questão é informado, e o seu programa
feed deve terminar automaticamente. Adicionalmente, todos os outros feed devem ser informados que o utilizador
foi eliminado da plataforma.

●Listar os tópicos existentes na plataforma: topics
São apresentados o nome dos tópicos e o número de mensagens persistentes.

●Listar as mensagens existentes num determinado tópico: show <topico>
São apresentadas todas as mensagens persistentes de um determinado tópico.

●Bloquear um tópico: lock <topico>
Bloqueia o envio de novas mensagens para o tópico indicado. As mensagens persistentes continuam a existir até que
a sua duração termine, e continua a ser possível a um utilizador subscrever o tópico.

●Desbloquear um tópico: unlock <topico>

●Encerrar a plataforma: close
Encerra a plataforma e termina o manager. Todos os processos a correr o feed são notificados, devendo também
terminar. Os recursos do sistema em uso são libertados.









*/
int running = 1;
void handle_signal(int sig)
{
        running = 0;
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
                // vai ler dados do pipe
                while (running)
                {       //ler dados do pipe
                        nbytes = read(fd, tipo, BUFFER_SIZE);
                        if (nbytes == -1)
                        {
                                if (errno != EINTR)
                                {
                                        perror("ocorreu um erro na leitura do named pipe");
                                }
                                close(fd);
                                unlink(FIFO_NAME);
                                exit(EXIT_FAILURE);
                        }
                        printf("TESTE -> %d \n", tipo );
                }
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