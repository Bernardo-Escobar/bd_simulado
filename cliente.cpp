#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "banco.h"

using namespace std;

int main() {
    Registro r;

    bool loop = true;
    int op = 0;

    const char* fifo_path = "fifo";
    int fd = open(fifo_path, O_WRONLY);

    do {
        cout << "Qual operação desejada? 1-INSERT; 2-DELETE; 3-UPDATE; 4-SELECT; 5-SAIR" << endl;
        cin >> op;
        cin.ignore();

        switch (op){
            case 1:
                strcpy(r.operacao, "INSERT");

                cout << "\nNome = ";
                cin.getline(r.nome, 100);

                write(fd, &r, sizeof(Registro));

                cout << "Enviado ao servidor!" << endl;

                cout << r.operacao << " | " << r.nome << endl;
            break;
    
            case 2:
                strcpy(r.operacao, "DELETE");

                cout << "Id = ";
                cin >> r.id;
                cin.ignore();

                write(fd, &r, sizeof(Registro));
            break;

            case 3:
                strcpy(r.operacao, "UPDATE");

                cout << "Id = ";
                cin >> r.id;
                cin.ignore();

                cout << "Novo nome = ";
                cin.getline(r.nome, 100);

                write(fd, &r, sizeof(Registro));
            break;

            case 4: {
                strcpy(r.operacao, "SELECT");

                int opcao;

                cout << "1 - Visualizar tudo\n2 - Visualizar por nome" << endl;
                cin >> opcao;
                cin.ignore();  // limpar o buffer

                switch(opcao){
                    case 1:
                        r.id = -1; // ID -1 indica "mostrar tudo"
                    break;

                    case 2:
                        r.id = -2;
                        cout << "Nome = ";
                        cin.getline(r.nome, 100);
                    break;
                }

                // Envia solicitação ao servidor
                write(fd, &r, sizeof(Registro));

                // FIFO de resposta
                const char* fifo_resposta = "fifo_resposta";
                mkfifo(fifo_resposta, 0666);
                int fd_resposta = open(fifo_resposta, O_RDONLY);

                
                Registro resposta;
                while (read(fd_resposta, &resposta, sizeof(Registro)) > 0) {
                    if (strcmp(resposta.operacao, "FIM") == 0) break;
                    cout << resposta.id << " | " << resposta.nome << endl;
                }

                close(fd_resposta);
            }
            break;

            case 5:
                strcpy(r.operacao, "SAIR");
                write(fd, &r, sizeof(Registro));
                cout << "Comando de encerramento enviado ao servidor." << endl;

                loop = false;
            break;
    
            default:
                cout << "Operacao não existe! \n" << endl;
        }
    } while (loop);
    

    close(fd);
}