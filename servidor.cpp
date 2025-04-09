#include <iostream>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "banco.h"

using namespace std;

Registro r;

list<Registro> bd;

pthread_mutex_t mutex_bd = PTHREAD_MUTEX_INITIALIZER;

void *th_insert(void *param){
    pthread_mutex_lock(&mutex_bd); // trava

    auto it = bd.begin();
    while (it != bd.end() && it->id < r.id) {
        ++it;
    }
    bd.insert(it, r);

    for (const auto& item : bd) {
        cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

/*
void *th_delete(void *param){
    pthread_mutex_lock(&mutex_bd);



    for (const auto& item : bd) {
        cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}
*/

int main(){

    pthread_t tid_insert, tid_delete; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */

    const char* fifo_path = "fifo";

    // Criar FIFO (caso não exista)
    mkfifo(fifo_path, 0666);

    cout << "abrindo FIFO..." << endl;
    // Abrir FIFO para leitura
    int fd = open(fifo_path, O_RDONLY);
    cout << "FIFO aberto com sucesso!" << endl;

    pthread_attr_init(&attr);


    while(true){
        read(fd, &r, sizeof(Registro));

        if(strcmp(r.operacao, "INSERT") == 0){
            pthread_create(&tid_insert, &attr, th_insert, NULL);
        }
        // else if(r.processo == "DELETE"){
            //pthread_create(&tid_delete, &attr, th_delete, NULL);
        // }
        else if (strcmp(r.operacao, "SAIR") == 0) {
            cout << "Comando SAIR recebido. Encerrando servidor..." << endl;
            break;
        }
    }

    close(fd);
    unlink(fifo_path); // remove o arquivo FIFO
}