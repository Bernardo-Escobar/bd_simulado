#include <iostream>
#include "banco.h"

using namespace std;

int main() {
    bool loop = true;
    int op;

    list<Registro>::iterator it;

    Registro r;

    for(int i=0; i<3; i++){
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
    
                    // Inserção ordenada:
                    it = bd.begin();
                    while (it != bd.end() && it->id < r.id) {
                        ++it;
                    }
                    bd.insert(it, r);
    
                    loop = false;
                break;
        
                case 2:
                    r.operacao = "DELETE";
    
                    cout << "Id = ";
                    cin >> r.id;
                    
                    bd.push_back(r);
    
                    loop = false;
                break;
        
                default:
                    cout << "Operacao não existe! \n" << endl;
            }
        }
        while (loop);
    }

    for(const auto& item : bd) {
        cout << item.operacao << " | " << item.id << " | " << item.nome << endl;
    }
    
}