#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/t.h>
#include<sys/types.h>
#include<sys/wait.h>


#define PIPE_NAME "pipe_eco"
#define BUFFER_SIZE 1024

int running = 1;

void handle_signal(int sig) {
        running = 0;
}

int main(){
        setbuf(stdout, NULL);

        // Redefinir o sinal SIGINT
        struct sigaction sa;
        sa.sa_handler = handle_signal;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, NULL);

        // Verificar se o pipe ja existe e se o eco ja se encontra a correr
        if(access(PIPE_NAME, F_OK) == 0){
                printf("O eco ja se encontra em execucao\n");
                exit(0);
        }
        // Criar o named pipe
        if(mkfifo(PIPE_NAME, 0666) == -1){
                if(errno != EEXIST) {
                        perror("erro na criacao do named pipe");
                        exit(EXIT_FAILURE);
                }
        }

        // Abertura do named pipe para leitura
        int fd = open(PIPE_NAME, O_RDONLY);
        if(fd == -1){
                perror("erro na abertura do named pipe para leitura");
                unlink(PIPE_NAME);
                exit(EXIT_FAILURE);
        }

        // Ciclo
        char buffer[BUFFER_SIZE];
        int nbytes;
        while(running){
                // Ler do pipe
                nbytes = read(fd, buffer, BUFFER_SIZE);
                if (nbytes == -1){
                        perror("ocorreu um erro na leitura do named pipe");
                        exit(EXIT_FAILURE);
                }
                printf("Leu %d nbytes do named pipe\n", nbytes);
                if(nbytes > 0){
                        buffer[nbytes] = '\0';
                        if(strncmp(buffer, "sair", 4) == 0){
                                running = 0;
                        } else {
                                printf("%s", buffer);
                        }
                }
        }

        // Fechar o pipe
        close(fd);
        // Apagar o pipe
        unlink(PIPE_NAME);

        return 0;
}