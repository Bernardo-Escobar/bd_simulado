#ifndef BANCO_H
    #define BANCO_H

    #include <list>
    #include <string>

    using namespace std;

    typedef struct {
        string operacao;
        int id;
        string nome;
    } Registro;

    extern list<Registro> bd;

#endif
   