#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/mman.h>
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

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

/*
void *th_delete(void *param){
    pthread_mutex_lock(&mutex_bd);


    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}
*/

int main(){

    bool op = true;

    pthread_t tid_insert, tid_delete; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */

    const char* name = "shared_memory";
    const int SIZE = sizeof(Registro);

    // Criar ou abrir memória compartilhada
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        cerr << "Erro ao criar memória compartilhada.\n";
        return 1;
    }

    // Definir o tamanho da memória compartilhada
    if (ftruncate(shm_fd, SIZE) == -1) {
        cerr << "Erro ao definir tamanho da memória.\n";
        return 1;
    }

    // Mapear para o espaço do processo
    void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        cerr << "Erro ao mapear memória.\n";
        return 1;
    }

    Registro r;
    memcpy(&r, ptr, SIZE);

    pthread_attr_init(&attr);

    pthread_create(&tid_insert, &attr, th_insert, NULL);
    //pthread_create(&tid_delete, &attr, th_delete, NULL);

    while(op){
        if(r.operacao == "INSERT"){
            pthread_join(tid_insert, NULL);

            op = false;
        }
        // else if(r.processo == "DELETE"){
        //     pthread_join(tid_delete, NULL);
        // }

        for (const auto& item : bd) {
            cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
        }
    }
    

    // Remover memória
    shm_unlink(name);
}