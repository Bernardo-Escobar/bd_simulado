#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pthread.h>
#include "banco.h"

using namespace std;

void *th_insert(void *param){
    pthread_mutex_lock(&mutex_bd); // trava

	ofstream arquivo("banco.txt", ios::app); // abre no modo append (acrescentar)

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;

        pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
        pthread_exit(0);
    }

    for (const auto& r : bd) {
        if (r.operacao == "INSERT") {
            // Escreve: id;nome;
            arquivo << r.id << "; " << r.nome << ";" << endl;
        }
    }

    arquivo.close();

    pthread_mutex_unlock(&mutex_bd); // destrava antes de sair
	pthread_exit(0);
}

void *th_delete(void *param){
    pthread_mutex_lock(&mutex_bd);

    ifstream entrada("banco.txt");
    ofstream saida("temp.txt");

    if (!entrada.is_open() || !saida.is_open()) {
        cerr << "Erro ao abrir os arquivos!" << endl;
        pthread_exit(0);
    }

	for (const auto& r : bd) {
        if (r.operacao == "DELETE") {
            string linha;
            while (getline(entrada, linha)) {
                stringstream ss(linha);
                string campo;
                int id_lido;

                getline(ss, campo, ';');  // pega o ID
                id_lido = stoi(campo);    // converte pra int

                if (id_lido != r.id) {
                    saida << linha << endl;  // mantém a linha
                }
            }
        }
    }

    entrada.close();
    saida.close();

    remove("banco.txt");
    rename("temp.txt", "banco.txt");

	pthread_exit(0);
}
 /*
void *th_delete(void *param) {
    pthread_mutex_lock(&mutex_bd);  // 🔒 trava a lista para acesso seguro

    // 1. Procurar o último DELETE na lista bd
    int id_para_deletar = -1;
    for (auto it = bd.rbegin(); it != bd.rend(); ++it) {
        if (it->operacao == "DELETE") {
            id_para_deletar = it->id;
            break;
        }
    }

    // Se não encontrou nenhum DELETE, libera o mutex e sai
    if (id_para_deletar == -1) {
        cout << "Nenhum DELETE encontrado na lista." << endl;
        pthread_mutex_unlock(&mutex_bd);  // 🔓 destrava
        pthread_exit(0);
    }

    // 2. Abrir arquivos
    ifstream entrada("banco.txt");
    ofstream saida("temp.txt");

    if (!entrada.is_open() || !saida.is_open()) {
        cerr << "Erro ao abrir os arquivos!" << endl;
        pthread_mutex_unlock(&mutex_bd);  // 🔓 destrava
        pthread_exit(0);
    }

    // 3. Copiar todas as linhas, exceto a que tem o ID para deletar
    string linha;
    while (getline(entrada, linha)) {
        stringstream ss(linha);
        string campo;
        int id_lido;

        getline(ss, campo, ';');  // lê o ID da linha
        try {
            id_lido = stoi(campo);
        } catch (...) {
            continue;  // ignora linha malformada
        }

        if (id_lido != id_para_deletar) {
            saida << linha << endl;  // mantém a linha
        }
    }

    entrada.close();
    saida.close();

    // 4. Substituir o arquivo original
    remove("banco.txt");
    rename("temp.txt", "banco.txt");

    cout << "Registro com ID " << id_para_deletar << " deletado com sucesso." << endl;

    pthread_mutex_unlock(&mutex_bd);  // 🔓 destrava
    pthread_exit(0);
}
    */

int main(){
    pthread_t tid_insert, tid_delete; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */

    pthread_attr_init(&attr);

    pthread_create(&tid_insert, &attr, th_insert, NULL);
    pthread_create(&tid_delete, &attr, th_delete, NULL);

    pthread_join(tid_insert, NULL);
    pthread_join(tid_delete, NULL);
}