#include <stdio.h> 

extern char  **environ; 

int main (int agrc, char *argv[], char envp[])
{

//cada posicao do array envp contem uma var de ambiente 
//no formato key = value


// for(int i = 0; envp[i] != NULL; i++) {
//		printf("%s\n",envp[i]); 
//	 }



//	for(int i = 0; environ[i] != NULL; i++) {
//		printf("%s\n",environ[i]); 
//	 }


char *valor = getenv("VAR");

	if(valor == NULL ) 
		printf("Variavel de ambiente nao encontrada"); 
 	else
		printf("VAR = %s\n", valor); 

return 0; 
}
