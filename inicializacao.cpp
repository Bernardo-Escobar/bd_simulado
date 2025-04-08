#include "banco.h"

list<Registro> bd;
pthread_mutex_t mutex_bd = PTHREAD_MUTEX_INITIALIZER;