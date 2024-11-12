#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>


#define PIPE_NAME "pipe_echo"
#define BUFFER_SIZE 1024
int running = 1;

void handle_signal()
{
	running = 0;
}



int main()
{
	setbuf(stdout, NULL);

	// Redefenir o Sinal
	struct sigaction sa;
	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	// Verificar Se o PIPE ja existe e se o eco ja se encontra a correr
	if (access(PIPE_NAME, F_OK) == 0)
	{
		printf("O Echo ja se encontra em Execucao\n");
		exit(1);
	}

	// Criar o namedPipe
	 if(mkfifo(PIPE_NAME, 0666) == -1){
	{
		perror("Erro na criacao de Named Pipe\n");
		exit(EXIT_FAILURE);
	}

	// Abertura do Named Pipe para leitura
	// Abir para leitura a escrita para precaver o caso que todos os produtos terminam e ficamos a ler 0 bytes do named pipe
	//um ciclo infinito e espera ativa 
	int fd = open(PIPE_NAME, O_RDWR);
	if (fd == -1)
	{
		perror("Erro na abertura do named pipe para leitura\n");
		unlink(PIPE_NAME);
		exit(EXIT_FAILURE);
	}

	// Ciclo
	char buffer[BUFFER_SIZE];
	int nbytes;
	while (running)
	{
		// Ler do Pipe
		nbytes = read(fd, buffer, BUFFER_SIZE);
		if (nbytes == -1)
		{
			if(errno != EINTR){
				perror("ocorreu um erro na leitura do Named Pipe");
			}
			perror("Ocorreu um Erro na leitura do namedPipe");
			close(fd); 
			unlink(PIPE_NAME);
			exit(EXIT_FAILURE);
		}
		printf("Leu %d nbytes do Named Pipe\n");
		if (nbytes > 0)
		{
			buffer[nbytes] = '\0';
			if (strncmp(buffer, "sair", 4) == 0)
			{
				running = 0;
			}
			else
			{
				printf("->%s\n", buffer);
			}
		}
	}

	// Fechar o Pipe
	close(fd);

	// Apagar o Pipe
	unlink(PIPE_NAME);

	return 0;

}
}
