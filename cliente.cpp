#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "banco.h"

using namespace std;

int main() {
    bool loop = true;
    int op = 0;

    Registro r;

    const char* fifo_path = "fifo";

    int fd = open(fifo_path, O_WRONLY);

    do {
        cout << "Qual operação desejada? 1-INSERT; 2-DELETE; 3-SAIR" << endl;
        cin >> op;
        cin.ignore();

        switch (op){
            case 1:
                strcpy(r.operacao, "INSERT");

                cout << "Id = ";
                cin >> r.id;
                cin.ignore();

                cout << "\nNome = ";
                cin.getline(r.nome, 100);

                write(fd, &r, sizeof(Registro));

                cout << "Enviado ao servidor!" << endl;

                cout << r.operacao << " | " << r.id << " | " << r.nome << endl;
            break;
    
            case 2:
                strcpy(r.operacao, "DELETE");

                cout << "Id = ";
                cin >> r.id;
                cin.ignore();

                write(fd, &r, sizeof(Registro));
            break;

            case 3:
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

    cout << r.operacao << " | " << r.id << " | " << r.nome << endl;
    
}