#include <iostream>
#include "banco.h"

using namespace std;

int main() {
    bool loop = true;
    int op;
    Registro r;

    do {
        cout << "Qual operação desejada? 1-INSERT; 2-DELETE" << endl;
        cin >> op;

        switch (op){
            case 1:
                r.operacao = "INSERT";

                cout << "Id = ";
                cin >> r.id;

                cout << "\nNome = ";
                cin >> r.nome;

                loop = false;
            break;
    
            case 2:
                r.operacao = "DELETE";

                cout << "Id = ";
                cin >> r.id;
                
                loop = false;
            break;
    
            default:
                cout << "Operacao não existe! \n" << endl;
        }
    }
    while (loop);

    cout << r.id << "    " << r.nome << endl;
    
}