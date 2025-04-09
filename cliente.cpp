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
    int oper;
    char opcao;

    Registro r;

    const char* fifo_path = "fifo";

    int fd = open(fifo_path, O_WRONLY);

    do {
        cout << "Qual operação desejada? 1-INSERT; 2-DELETE" << endl;
        cin >> oper;
        cin.ignore();

        switch (oper){
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

                loop = false;
            break;
    
            default:
                cout << "Operacao não existe! \n" << endl;
        }

        cout << "\n\nDeseja realizar mais uma operacao? S - Sim; N - Nao" << endl;
        cin >> opcao;
    }
    while (opcao == 'S' || opcao == 's');
    

    close(fd);

    cout << r.operacao << " | " << r.id << " | " << r.nome << endl;
    
}