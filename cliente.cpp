#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants
#include <fcntl.h>           // For O_* constants
#include <unistd.h>          // For ftruncate
#include <cstring>           // For memcpy
#include "banco.h"

using namespace std;

int main() {
    bool loop = true;
    int op;

    Registro r;

    const char* name = "meu_segmento";
    const int SIZE = sizeof(Registro);

    // Abrir memória compartilhada existente
    int shm_fd = shm_open(name, O_RDONLY, 0666);
    void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

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

                    memcpy(ptr, &r, SIZE);
    
                    loop = false;
                break;
        
                case 2:
                    r.operacao = "DELETE";
    
                    cout << "Id = ";
                    cin >> r.id;

                    memcpy(ptr, &r, SIZE);
    
                    loop = false;
                break;
        
                default:
                    cout << "Operacao não existe! \n" << endl;
            }
        }
        while (loop);
    }

    cout << r.operacao << " | " << r.id << " | " << r.nome << endl;
    
}