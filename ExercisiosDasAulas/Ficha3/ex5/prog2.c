#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include  <sys/wait.h>
int main () 
{
int a = 10;
	pid_t res_fork = fork();

	if (res_fork == 0) {
		//Processo filho
  		printf("FILHO: O fork devolveu %d\n", res_fork);
		pid_t pid_filho = getpid(); //obtem o pai do processo
		printf("FILHO: O meu pid e %d\n,", pid_filho);
		pid_t pid_pai = getppid();
		printf("FILHO: O pid do meu pai e %d\n", pid_pai);
		a++;
}
		
	else {
		//Processo Pai
		printf("PAI: o fork devolveu %d\n", res_fork);
		printf("PAI: o pid do filho e %d\n", res_fork);
		pid_t pid_pai = getpid();
		printf("PAI: O meu pid e %d\n", pid_pai);
		int status;
		pid_t pid_filho = res_fork; 
		waitpid (pid_filho , &status , 0 );	
		a--;
}
	printf("A = %d\n" , a);
return 0; 
}
