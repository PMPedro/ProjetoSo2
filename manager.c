#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include  <sys/wait.h>


int main() { 

    	pid_t res_fork = fork();

        	if (res_fork == 0) {
                //Processo Filho

                int r  = execl("feed", "feed", NULL) ; 

	            if (r == -1) 
                    perror("Erro a Executar");
            }
            else{
                //Processo Pai
                int status;
                pid_t pid_filho = res_fork; //Vai buscar PID do filhi
		        waitpid (pid_filho , &status , 0 );	//Espera Pelo Filho
            }


}