#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "banco.h"

using namespace std;

int main() {
    Registro r; // Struct para armazenar os dados da operação

    bool loop = true;
    int op = 0;

    const char* fifo_path = "fifo"; // Caminho do FIFO usado para comunicação
    int fd = open(fifo_path, O_WRONLY); // Abre o FIFO para escrita (cliente -> servidor)

    do {
        cout << "Qual operação desejada? 1-INSERT; 2-DELETE; 3-UPDATE; 4-SELECT; 5-SAIR" << endl;
        cin >> op;
        cin.ignore(); // Limpa o caractere '\n' do buffer

        switch (op){
            case 1:
                strcpy(r.operacao, "INSERT"); // Define a operação

                cout << "\nNome = ";
                cin.getline(r.nome, 100); // Lê nome do usuário

                write(fd, &r, sizeof(Registro)); // Envia struct para o servidor

                cout << "\nEnviado ao servidor!\n" << endl;
            break;
    
            case 2:
                strcpy(r.operacao, "DELETE");

                cout << "Id = ";
                cin >> r.id;
                cin.ignore();

                write(fd, &r, sizeof(Registro));

                cout << "\nEnviado ao servidor!\n" << endl;
            break;

            case 3:
                strcpy(r.operacao, "UPDATE");

                cout << "Id = ";
                cin >> r.id;
                cin.ignore();

                cout << "Novo nome = ";
                cin.getline(r.nome, 100);

                write(fd, &r, sizeof(Registro));

                cout << "\nEnviado ao servidor!\n" << endl;
            break;

            case 4: {
                strcpy(r.operacao, "SELECT");

                int opcao;

                cout << "\n1 - Visualizar tudo\n2 - Visualizar por nome" << endl;
                cin >> opcao;
                cin.ignore();  // limpar o buffer

                switch(opcao){
                    case 1:
                        r.id = -1; // ID -1 sinaliza visualização geral
                    break;

                    case 2:
                        r.id = -2; // ID -2 sinaliza visualização por nome
                        cout << "Nome = ";
                        cin.getline(r.nome, 100);
                    break;
                }

                // Envia solicitação SELECT ao servidor
                write(fd, &r, sizeof(Registro));

                // Cria FIFO para resposta e abre para leitura
                const char* fifo_resposta = "fifo_resposta";
                mkfifo(fifo_resposta, 0666);
                int fd_resposta = open(fifo_resposta, O_RDONLY);

                cout << endl;

                // Lê e imprime os registros retornados
                Registro resposta;
                while (read(fd_resposta, &resposta, sizeof(Registro)) > 0) {
                    if (strcmp(resposta.operacao, "FIM") == 0) break;
                    cout << resposta.id << " | " << resposta.nome << endl;
                }

                cout << endl;

                close(fd_resposta); // Fecha FIFO de resposta
            }
            break;

            case 5:
                strcpy(r.operacao, "SAIR"); // Define operação de encerramento
                write(fd, &r, sizeof(Registro));
                cout << "Comando de encerramento enviado ao servidor." << endl;

                loop = false; // Encerra loop
            break;
    
            default:
                cout << "Operacao não existe! \n" << endl;
        }
    } while (loop);
    

    close(fd); // Fecha FIFO principal
}