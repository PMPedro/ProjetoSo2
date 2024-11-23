#include <estrutura.h>

#define FIFO_NAME "mainpipe"

/*
#####

FAZER VERIFICAÇÃO AO INICIO SE O MANAGER ESTÁ LIGADO!!!!!


*/

/*
User vai interagir atraves de codigos
 -Enviar mensagem para um determinado tópico: msg <topico> <duração> <mensagem>
- Subscrever um determinado tópico: subcribe <topico>
- Deixar de subscrever um determinado tópico: unsubcribe <topico>
-Sair, encerrando o processo feed: comando exit
*/

int main()
{
    /*


    FAZER VERIFICAO DE MANAGER LOGO AO INICIO E SAIR SE NAO EXISTIR MANAGER EM EXECUCAO



    */
    char username[100];

    printf("Bem vindo Ao chat!\n");
    printf("Qual o seu Username: ");
    fflush(stdin);
    fgets(username, sizeof(username), stdin);
}