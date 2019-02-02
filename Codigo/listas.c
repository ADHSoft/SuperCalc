#include "listas.h"

digito_n* obtener_ultimo_nodo(digito_n *nodo) {
    while (nodo->sig!=NULL) {
        nodo=nodo->sig;
    }

    return nodo;
}

digito_n* obtener_primer_nodo(digito_n *nodo) {
    while (nodo->prev!=NULL) {
        nodo=nodo->prev;
    }

    return nodo;
}

digito_n* desplazarse_n_nodos(digito_n *nodo,int despl) {

    int i;

    if (despl>0) {
        for (i=0;i<despl;i++)
            nodo=nodo->sig;
    } else {
        for (i=0;i>despl;i--)
            nodo=nodo->prev;
    }

    return nodo;
}

int dar_longitud_lista(digito_n *nodo){

    int i=0;

    nodo=obtener_primer_nodo(nodo);
    while (nodo->sig!=NULL) {
        nodo=nodo->sig;
        i++;
    }

    return i;
}

digito_n* obtener_nodo_numero(digito_n *nodo,int num) {

    nodo=obtener_primer_nodo(nodo);
    nodo=desplazarse_n_nodos(nodo,num);

    return nodo;
}
