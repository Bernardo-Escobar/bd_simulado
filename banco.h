#ifndef BANCO_H
    #define BANCO_H

    #include <list>

    using namespace std;

    typedef struct {
        char operacao[20];
        int id;
        char nome[100];
    } Registro;

    extern list<Registro> bd;

#endif
   