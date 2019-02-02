#ifndef BIGNUM_H_INCLUDED
#define BIGNUM_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tipos_definidos.h"
#include "listas.h"

typedef struct {
    digito_n *primernodo;
    sign_t sign;
    unsigned int length;
} bignum_t ;

/*bignum functions */
void liberar_memoria_bignum ( bignum_t* );
void invertir_signo_bignum ( bignum_t*) ;
bignum_t* armar_bignum(void);
bignum_t* armar_bignum_cero(void);
bignum_t* copiar_bignum(const bignum_t*);
void actualizar_primer_nodo(bignum_t*);
error_t agregar_nodos_al_comienzo(bignum_t*,int);
error_t agregar_nodos_al_final (bignum_t*,int);

/*bignum arithmetic operations*/
bignum_t* add_bignum( const bignum_t*, const bignum_t*) ;
bignum_t* substract_bignum ( const bignum_t*, const bignum_t*) ;
bignum_t* multiply_bignum ( const bignum_t*, const bignum_t*) ;

#endif
