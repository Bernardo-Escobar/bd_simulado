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

int next_id = 0;

const char* fifo_resposta = "fifo_resposta";

pthread_mutex_t mutex_bd = PTHREAD_MUTEX_INITIALIZER;

void *th_insert(void *param){
    pthread_mutex_lock(&mutex_bd); // trava

    r.id = next_id++;

    bd.push_back(r);

    for (const auto& item : bd) {
        cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

void *th_delete(void *param){
    pthread_mutex_lock(&mutex_bd);

    bool encontrado = false;

    // Encontra o registro com o id correspondente e remove
    for (auto it = bd.begin(); it != bd.end(); ++it) {
        if (it->id == r.id) {
            bd.erase(it);

            cout << "Registro com ID " << r.id << " deletado com sucesso!" << endl;

            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Registro com ID " << r.id << " não encontrado para remoção." << endl;
    }

    for (const auto& item : bd) {
        cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

void *th_update(void *param){
    pthread_mutex_lock(&mutex_bd);

    bool encontrado = false;

    // Encontra o registro com o id correspondente e atualiza
    for (auto& it : bd) {
        if (it.id == r.id) {
            strcpy(it.nome, r.nome); // Atualiza o nome

            cout << "Registro com ID " << r.id << " atualizado com sucesso!" << endl;

            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Registro com ID " << r.id << " não encontrado para atualização." << endl;
    }

    for (const auto& item : bd) {
        cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

void *th_select(void *param){
    pthread_mutex_lock(&mutex_bd);

    // Abrir FIFO da reposta do SELECT
    int fd_resposta = open(fifo_resposta, O_WRONLY);

    if(r.id == -1){ // Visualizar tudo
        for(const auto& item : bd){
            write(fd_resposta, &item, sizeof(Registro));
        }
    }
    else if(r.id == -2){ // Visualizar por nome
        for(const auto& item : bd){
            if(strcmp(item.nome, r.nome) == 0){
                write(fd_resposta, &item, sizeof(Registro));
            }
        }
    }

    // Enviar sinal de fim
    Registro fim;
    strcpy(fim.operacao, "FIM");
    write(fd_resposta, &fim, sizeof(Registro));

    close(fd_resposta);
    pthread_mutex_unlock(&mutex_bd);
	pthread_exit(0);
}

int main(){

    pthread_t tid_insert, tid_delete, tid_update, tid_select; /* the thread identifier */
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
        else if(strcmp(r.operacao, "DELETE") == 0){
            pthread_create(&tid_delete, &attr, th_delete, NULL);
        }
        else if(strcmp(r.operacao, "UPDATE") == 0){
            pthread_create(&tid_update, &attr, th_update, NULL);
        }
        else if(strcmp(r.operacao, "SELECT") == 0){
            pthread_create(&tid_select, &attr, th_select, NULL);
        }
        else if (strcmp(r.operacao, "SAIR") == 0) {
            cout << "Comando SAIR recebido. Encerrando servidor..." << endl;
            break;
        }
    }

    close(fd);
    unlink(fifo_path); // remove o arquivo FIFO
    unlink(fifo_resposta);
}