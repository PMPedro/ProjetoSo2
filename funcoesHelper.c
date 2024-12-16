#include "helper.h"


void lockTopic ( char topic[25], all *st ){
    bool found = false;
    for(int i = 0; i < 20; i++){
        if(strcmp(st->topico[i].nomeTopico , topic) == 0){
            st->topico[i].locked = 1;
            found = true;
            break;
        }
    }
        if(!found)
        printf("<manager> Topico nao encontrado\n");
}



void unlockTopic ( char topic[25], all *st ){
    bool found = false;
    for(int i = 0; i < 20; i++){
        if(strcmp(st->topico[i].nomeTopico , topic) == 0){
            st->topico[i].locked = 0;
            found = true;
            break;
        }
    }
    if(!found)
        printf("<manager> Topico nao encontrado\n");
}

void listTopics ( all *st ) {
    bool found = false;
    for(int i = 0; i < 20 ; i++){
        if(strlen(st->topico[i].nomeTopico) > 0){
            printf("\n <topico %d> %s", i , st->topico[i].nomeTopico);
            found = true;
        }
    }
    if(!found) {
        printf("\n <manager> Ainda nao existem Topicos! ");
    }
}

void listUsers ( all *st ) {
    bool found = false;
    if(st == NULL ) {
        printf("NAO INICIALIZADO");
    }
    for(int i = 0; i < 20 ; i++){
        if(strlen(st->user[i].username) > 0){
            printf("\n <user %d> %s", i , st->user[i].username);
            found = true;
        }
    }
    if(!found) {
        printf("\n <manager> nenhum user! ");
    }
}

void initializeAll(all *st) {
    if (st == NULL) {
        return; // Evitar problemas caso o ponteiro seja nulo.
    }

    // Inicializar tópicos gerais criados
    for (int i = 0; i < 20; i++) {
        memset(st->topico[i].nomeTopico, 0, sizeof(st->topico[i].nomeTopico));
        memset(st->topico[i].msg.message, 0, sizeof(st->topico[i].msg.message));
        st->topico[i].msg.duracao = 0;
        for (int j = 0; j < 5; j++) {
            memset(st->topico[i].msgPersistente[j].message, 0, sizeof(st->topico[i].msgPersistente[j].message));
            st->topico[i].msgPersistente[j].duracao = 0;
        }
        st->topico[i].locked = 0;
    }

    // Inicializar usuários
    for (int i = 0; i < 10; i++) {
        memset(st->user[i].username, 0, sizeof(st->user[i].username));
        memset(st->user[i].rcvpipename, 0, sizeof(st->user[i].rcvpipename));
        for (int j = 0; j < 20; j++) {
            memset(st->user[i].topicosInscritos[j].nomeTopico, 0, sizeof(st->user[i].topicosInscritos[j].nomeTopico));
            memset(st->user[i].topicosInscritos[j].msg.message, 0, sizeof(st->user[i].topicosInscritos[j].msg.message));
            st->user[i].topicosInscritos[j].msg.duracao = 0;
            for (int k = 0; k < 5; k++) {
                memset(st->user[i].topicosInscritos[j].msgPersistente[k].message, 0, sizeof(st->user[i].topicosInscritos[j].msgPersistente[k].message));
                st->user[i].topicosInscritos[j].msgPersistente[k].duracao = 0;
            }
            st->user[i].topicosInscritos[j].locked = 0;
        }
    }

    // Inicializar o helper
    for (int i = 0; i < 10; i++) {
        st->help.cont[i] = 0;
    }
    FD_ZERO(&st->help.read_fds); // Zerar o conjunto de descritores
    st->help.fd = -1;

    // Inicializar outros campos
    st->tipo = 0;
    memset(st->userRemove, 0, sizeof(st->userRemove));
}





