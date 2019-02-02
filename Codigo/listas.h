#ifndef LISTAS_H_INCLUDED
#define LISTAS_H_INCLUDED

#include <stdio.h>

typedef struct NODO_DIGITO {
    unsigned short digit;
    struct NODO_DIGITO *sig;
    struct NODO_DIGITO *prev;
} digito_n;

digito_n* obtener_ultimo_nodo(digito_n*);
digito_n* obtener_primer_nodo(digito_n*);
digito_n* desplazarse_n_nodos(digito_n*,int);
int dar_longitud_lista(digito_n*);
digito_n* obtener_nodo_numero(digito_n*,int);

#endif
