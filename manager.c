#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <string.h>


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



int main() { 
        /*
        #########
        !!!!!!!!!!!!!!! AS MENSAGENS DOS TOPICOS TERÃO QUE INCLUIR ESPAÇOS!!!
        #########
        */
        struct{
              char message[300]; // TERÁ QUE INCLUIR ESPAÇOS!!!!
              int ntopics[20];// limite maximo topicos
              char topics[20];//nome topicos
              char username[32];
        }manager_info; //nome por melhorar!!!!
        int pipe_fd[2];
        int ret_val;

        char mensagemtesteparapipes[]="sojhdgioshgdfikhbgfj";



        //#########
        // fazer num futuro tipo uma lista ligada de structs
        //#########


        ret_val = pipe(pipe_fd);
        //### verificacao de abertura pipe
        if (ret_val != 0){
                perror("Erro ao abrir pipe!");
                return 0;
        }
        // perror("Erro");


    	pid_t res_fork = fork();

        	if (res_fork == 0) { //Processo Filho
                
                int r  = execl("feed", "feed", NULL) ;
                if (r == -1){ 
                    perror("Erro a Executar");
                }
                close(pipe_fd[0]); // fecha pipe na leitura (unistd)
                // Fecha a parte de leitura
                // int nbytes = write(canal_re[1], s, strlen(s));
                ret_val = write( pipe_fd[1],mensagemtesteparapipes,strlen(mensagemtesteparapipes) );

                 

                /*if (ret_val != strlen( mensagemtesteparapipes) ){
                        perror("write did not return expected value\n");
                        exit(2);
                }*/

	            
            }
            else{
                //Processo Pai
                int status;

                close(pipe_fd[1]); // fecha pipe na leitura (unistd)
                // Fecha a parte de Escrita

                ret_val = read(pipe_fd[0],mensagemtesteparapipes,strlen(mensagemtesteparapipes)); /* Read from pipe */

                  if (ret_val != strlen( mensagemtesteparapipes) ){
                        perror("write did not return expected value\n");
                        exit(2);
                }

                
                pid_t pid_filho = res_fork; //Vai buscar PID do filho
		        waitpid (pid_filho , &status , 0 );	//Espera Pelo Filho
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