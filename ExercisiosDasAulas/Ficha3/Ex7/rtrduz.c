// rtraduz.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main() {

  char op;
  char palavra[20];
  printf("Escolha a opcao: ");
  op = getchar();
  if (op == 'x') {
    return 0;
  }
  if (op != 'i' && op != 'f') {
    printf("opcao invalida\n");
    getchar(); // Ler o \n
    return 1;
  }
  printf("Insira a palavra: ");
  scanf("%s", palavra);
  getchar(); // Ler o \n

    int canal_re[2];
    pipe(canal_re);


  if (fork() == 0) {
    close(0);
    dup(canal_re[0]);
    close(canal_re[0]);
    close(canal_re[1]);



    if (op == 'i') {
      execl("./rding", "rding", NULL);
      perror("Erro a executar ding");
      exit(1);
    } else if (op == 'f') {
      execl("./rdfran", "rdfran", NULL);
      perror("Erro a executar dfran");
      exit(1);
    }
  } else {
    close(canal_re[0]);
    char s[20];
    sprintf(s, "%s\n",palavra);
    int nbytes = write(canal_re[1], s, strlen(s));
    if(nbytes ==-1)
        perror("Write");
    

    int status;
    wait(&status);
    close(canal_re[1]);
  }
  return 0;
}
