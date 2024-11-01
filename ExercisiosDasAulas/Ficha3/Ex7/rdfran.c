// rdfran.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N_PALAVRAS 5

int main(int argc, char *argv[]){
    char palavra[20];
    printf("rdfran: Insira a palavra: ");
    scanf("%s", palavra);
    char palavras_portugues[N_PALAVRAS][20] = {"casa", "escola", "carro", "pai", "mae"};
    char palavras_traduzidas[N_PALAVRAS][20] = {"maison","ecole", "voiture", "pere", "mere"};

    for(int i = 0; i < N_PALAVRAS; i++){
        if(strcmp(palavra, palavras_portugues[i]) == 0){
            printf("%s\n", palavras_traduzidas[i]);
            exit(0);
        }
    }
    printf("unknown\n");
    return 0;
}
