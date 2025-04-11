#include <iostream>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "banco.h"

using namespace std;

Registro r; // Struct global usada para comunicação com as threads

list<Registro> bd; // Banco de dados em memória

int next_id = 0; // Contador para IDs automáticos

const char* fifo_resposta = "fifo_resposta"; // Caminho da FIFO de resposta

pthread_mutex_t mutex_bd = PTHREAD_MUTEX_INITIALIZER; // Mutex para controle de concorrência

// Thread para inserção
void *th_insert(void *param){
    pthread_mutex_lock(&mutex_bd); // Trava acesso à base

    r.id = next_id++; // Gera novo ID

    bd.push_back(r); // Adiciona registro

    cout << "Registro com ID " << r.id << " e NOME " << r.nome << " inserido com sucesso!\n" << endl;

    pthread_mutex_unlock(&mutex_bd); // Libera acesso
	pthread_exit(0);
}

// Thread para remoção
void *th_delete(void *param){
    pthread_mutex_lock(&mutex_bd);

    bool encontrado = false;

    // Busca o registro e remove, se encontrar
    for (auto it = bd.begin(); it != bd.end(); ++it) {
        if (it->id == r.id) {
            bd.erase(it);

            cout << "Registro com ID " << r.id << " deletado com sucesso!\n" << endl;

            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Registro com ID " << r.id << " não encontrado para remoção.\n" << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

// Thread para atualização
void *th_update(void *param){
    pthread_mutex_lock(&mutex_bd);

    bool encontrado = false;

    // Busca e atualiza nome se o ID for encontrado
    for (auto& it : bd) {
        if (it.id == r.id) {
            strcpy(it.nome, r.nome); // Atualiza o nome

            cout << "Registro com ID " << r.id << " atualizado com sucesso!\n" << endl;

            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Registro com ID " << r.id << " não encontrado para atualização.\n" << endl;
    }

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

// Thread para SELECT (visualização)
void *th_select(void *param){
    pthread_mutex_lock(&mutex_bd);

    // Abre FIFO para escrita (resposta ao cliente)
    int fd_resposta = open(fifo_resposta, O_WRONLY);

    if(r.id == -1){ // Visualizar todos os registros
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

    // Envia sinal de término da resposta
    Registro fim;
    strcpy(fim.operacao, "FIM");
    write(fd_resposta, &fim, sizeof(Registro));

    close(fd_resposta);
    pthread_mutex_unlock(&mutex_bd);
	pthread_exit(0);
}

int main(){

    pthread_t tid_insert, tid_delete, tid_update, tid_select;
	pthread_attr_t attr;

    const char* fifo_path = "fifo";

    mkfifo(fifo_path, 0666); // Cria FIFO se necessário

    cout << "abrindo FIFO..." << endl;
    int fd = open(fifo_path, O_RDONLY); // Abre FIFO para leitura (bloqueia até o cliente abrir)
    cout << "FIFO aberto com sucesso!" << endl;

    pthread_attr_init(&attr); // Inicializa atributos da thread


    while(true){
        read(fd, &r, sizeof(Registro)); // Lê operação do cliente

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
    unlink(fifo_path); // Remove FIFO principal
    unlink(fifo_resposta); // Remove FIFO de resposta
}