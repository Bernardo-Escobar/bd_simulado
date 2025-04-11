#ifndef BANCO_H
    #define BANCO_H

    #include <list>

    using namespace std;

    // Definição da estrutura usada na comunicação
    typedef struct {
        char operacao[20]; // INSERT, DELETE, etc
        int id; // ID do registro
        char nome[100]; // Nome associado
    } Registro;

    // Lista global de registros (definida no servidor)
    extern list<Registro> bd;

#endif
   