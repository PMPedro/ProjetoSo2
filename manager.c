#include "helper.h"
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






//fazer uma funcao de threads para 
    -> receber dados 



*/
int running = 1;
void handle_signal(int sig)
{
    printf("Adeus!\n");
    running = 0;
        //close(fd);
        unlink(FIFO_NAME);
}


//pensar melhor nesta parte 

/*
ideias 
    ->FAzer struct gigante que envolva tudo , ver o tipo, ir buscar x daya 
    > Mandar mts estruturas 
    > 1 thread por estrutura (acho ma ideia)
    > pesquisar melhor
*/
void *getpipemessages (void *pall){
    all *ptd = (all*) pall;
    
    while (1)
    {
            int nbytes = read(ptd->help.fd, &(*ptd), sizeof(*ptd)); 
            if (nbytes == -1)
                {
                    if (errno != EINTR)
                    {
                        perror("ocorreu um erro na leitura do named pipe");
                    }
                    close(ptd->help.fd);
                    unlink(FIFO_NAME);
                    exit(EXIT_FAILURE);
                }
                printf("\n-> %s" , ptd->msg.message);
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
            scanf("%s", &aux); printf("(TESTE)%s", aux); 
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