#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

int contador = 0;

void handle_signal(int sig) {
    if (sig != SIGINT)
        return;

    if (contador++ < 5) {
        printf("Ai\n");
        return;
    }
    printf("Ok, prontos\n");
    exit(0);
}

int main() {
    char nome[20];
    //signal(SIGINT, handle_signal);
    struct sigaction sa; 
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT ,&sa , NULL);
    

    while (1) {
        printf("Introduza o nome: ");
        scanf("%19s", nome);  // Limitar a entrada a 19 caracteres

        if (strcmp(nome, "sair") == 0) {
            break;
        }

        printf("Ola %s\n", nome);
    }
    return 0;
}
