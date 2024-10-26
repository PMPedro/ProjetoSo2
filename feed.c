#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include  <sys/wait.h>

int main() { 
    char username[100];

    printf("Bem vindo Ao chat!\n");
    printf("Qual o seu Username: ");
    fflush(stdin); fgets(username, sizeof(username), stdin);


}