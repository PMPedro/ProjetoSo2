#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main () {
	char str[20];
	scanf("%s",&str);
	int r  = execl(str, str, NULL) ; 
	if (r == -1) perror("Erro a Executar");
	
//	/if(strcmp("port", str ) == 0 ) {
//	execlp("port.o" , NULL , NULL ) ;	
//	
//	 }else if(strcmp("ing", str) == 0) {
//	execlp("ing.o" , NULL, NULL) ; 
//	}

	printf("Missao Comprida\n");

} 
