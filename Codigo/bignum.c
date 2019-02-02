#include "bignum.h"

typedef enum { A_Y_B_IGUALES , A_ES_MAYOR , B_ES_MAYOR , ERROR_MEM } comp_t;

comp_t comparar_modulo_bignum_e_igualar_largo(bignum_t*,bignum_t*);
error_t agregar_ceros_a_la_izquierda(bignum_t*,int);
error_t quitar_ceros_de_mas(bignum_t*);
void invertir_posicion_digitos(bignum_t*);
error_t multiplicar_por_potencia_de_10(bignum_t*,int);
bignum_t* multiplicar_por_una_cifra(bignum_t* ,unsigned short);

/* Suma los modulos de dos bignum. El resultado siempre es positivo.
 * Devuelve NULL si hay error de memoria.  */
bignum_t* add_bignum( const bignum_t* op1, const bignum_t* op2) {

    int acumulador,carry=0,a,b,i;
    bignum_t* resultado=NULL;

    resultado=armar_bignum();
    if (resultado==NULL) return NULL;

    /* sumar de derecha (digitos menos significativos)
     * a izquierda, usando carry*/
    for ( i=0 ; i<(*op1).length || i<(*op2).length || carry!=0 ; i++) {

        /* leer una cifra, pero evitando leer una direccion inexistente
         * (cuando excedemos la cantidad de digitos del operando)*/
        if ((*op1).length < (i+1)) {
            a=0;    /*entonces si no hay mas digitos, leer un cero*/
        } else {
            a=(desplazarse_n_nodos(op1->primernodo,(*op1).length-1-i))->digit;
        }

        /* lo mismo para op2*/
        if ((*op2).length < (i+1)) {
            b=0;
        } else {
            b=(desplazarse_n_nodos(op2->primernodo,(*op2).length-1-i))->digit;
        }

        /*hacer la suma de una cifra*/
        acumulador=a+b+carry;

        /*mover la decena al carry*/
        if (acumulador>=10) {
            carry=1;
            acumulador-=10;
        } else {
            carry=0;
        }

        (*resultado).length++;

        /*extender lista del resultado*/
        if ((error_t)agregar_nodos_al_comienzo(resultado,1)==(error_t)ERROR_MEMORY) return NULL;
        actualizar_primer_nodo(resultado);
        /*guardar la cifra*/
        resultado->primernodo->digit=acumulador;
    }

    /*invertir_posicion_digitos(*resultado);*/
    quitar_ceros_de_mas(resultado);
    (*resultado).sign=SIGNO_POSITIVO;

    return resultado;
}

/* Resta los modulos de dos bignum (|A|-|B|). El signo del resultado es calculado.
 * Devuelve NULL si hay un error de memoria.*/
bignum_t* substract_bignum ( const bignum_t* op1_in, const bignum_t* op2_in) {

    int i,j,invertir_signo=0;
    comp_t return_value_comp;
    bignum_t *resultado=NULL,*aux=NULL,*op1=NULL,*op2=NULL;

    /*usar bignum auxiliares para no modificar a los originales const.*/
    op1=copiar_bignum(op1_in);
    if (op1==NULL) return NULL;
    op2=copiar_bignum(op2_in);
    if (op2==NULL) return NULL;

    /*Inicializar el resultado*/
    resultado=armar_bignum();
    if (resultado==NULL) return NULL;

    /* Para simplificar, debemos restarle al numero mas grande
     * ,el num. mas chico, por lo tanto los ordenamos */
    return_value_comp=comparar_modulo_bignum_e_igualar_largo(op1,op2);
    switch (return_value_comp) {
        case ERROR_MEM:
            liberar_memoria_bignum(resultado);
            liberar_memoria_bignum(op1);
            liberar_memoria_bignum(op2);
            return NULL;
        case A_Y_B_IGUALES: /*devolver como resultado un bignum = 0 .*/
            liberar_memoria_bignum(op1);
            liberar_memoria_bignum(op2);
            return armar_bignum_cero();
        case A_ES_MAYOR:
            break;
        case B_ES_MAYOR:    /*intercambiar los operandos (las direcciones)*/
            aux=op1;
            op1=op2;
            op2=aux;
            invertir_signo=1;
            break;

    }

    /* hacer la resta op1-op2 , desde las cifras mas significativas hacia las menos.
     * Para ello, restar cifra por cifra "op1->length" veces.
     * (recordar que los dos operandos son de misma longitud)*/
    for (i=0 ; i<(*op1).length ; i++ ) {

        /*aumentar la memoria para el resultado*/
        (*resultado).length++;
        if (agregar_nodos_al_final(resultado,1)==ERROR_MEMORY) {    /*verificar error de memoria*/
            liberar_memoria_bignum(resultado);
            liberar_memoria_bignum(op1);
            liberar_memoria_bignum(op2);
            return NULL;
        }
        /*realizar un acarreo si corresponde*/
        if ( desplazarse_n_nodos(op1->primernodo,i)->digit < desplazarse_n_nodos(op2->primernodo,i)->digit ) {

            desplazarse_n_nodos(op1->primernodo,i)->digit+=10;
            /* ir acarreando cifra por cifra hacia la izquierda, hasta encontrar una distinta de 0. */
            j=i;
            do {
                j--;
                if (desplazarse_n_nodos(resultado->primernodo,j)->digit==0) {  /*evitar restarle 1 a un unsigned short=0.*/
                    desplazarse_n_nodos(resultado->primernodo,j)->digit=9;
                } else {
                    desplazarse_n_nodos(resultado->primernodo,j)->digit--;
                }

                 /*si la cifra se puso en 9, porque esta valía 0, sabemos que el acarreo debe continuar.*/
            } while (desplazarse_n_nodos(resultado->primernodo,j)->digit==9);
        }

        /*realizar la resta de las cifras [i]*/
        desplazarse_n_nodos(resultado->primernodo,i)->digit=desplazarse_n_nodos(op1->primernodo,i)->digit-desplazarse_n_nodos(op2->primernodo,i)->digit;

    }

    if (invertir_signo) invertir_signo_bignum(resultado);

    quitar_ceros_de_mas(resultado);
    liberar_memoria_bignum(op1);
    liberar_memoria_bignum(op2);
    return resultado;

}

bignum_t* multiply_bignum ( const bignum_t* op1_in, const bignum_t* op2_in) {

    int i;
    bignum_t *op1=NULL,*op2=NULL,*resultado=NULL,*aux=NULL,*aux2=NULL;
    error_t return_value;

    /*usar bignum auxiliares para no modificar a los originales const.*/
    op1=copiar_bignum(op1_in);
    if (op1==NULL) return NULL;
    op2=copiar_bignum(op2_in);
    if (op2==NULL) return NULL;

    resultado=armar_bignum_cero();  /*inicializar acumulador en 0.*/
    if (resultado==NULL) return NULL;

    for (i=0;i<(*op2).length;i++) {
        aux=multiplicar_por_una_cifra(op1,obtener_nodo_numero(op2->primernodo,(op2->length)-1-i)->digit);
        if (aux==NULL) return NULL;
        /* aux=aux(*10^i) */
        return_value=multiplicar_por_potencia_de_10(aux,i);
        if (return_value==ERROR_MEMORY) return NULL;
        /*acumular la suma de los productos intermedios*/
        aux2=add_bignum(aux,resultado);
        /*aux2 sera el nuevo acumulador*/
        liberar_memoria_bignum(resultado);
        liberar_memoria_bignum(aux);
        resultado=aux2;

    }

    /*calcular signo (xor)*/
    if ((*op1_in).sign == (*op2_in).sign) {
        (*resultado).sign=SIGNO_POSITIVO;
    } else {
        (*resultado).sign=SIGNO_NEGATIVO;
    }

    quitar_ceros_de_mas(resultado);
    liberar_memoria_bignum(op1);
    liberar_memoria_bignum(op2);
    return resultado;

}

void liberar_memoria_bignum ( bignum_t* bignum ) {

    digito_n *a=NULL,*b=NULL;

    if (bignum!=NULL) {
        /*liberar los digitos de un bignum*/
        if (bignum->primernodo!=NULL) {
            a=obtener_ultimo_nodo(bignum->primernodo);
            while (a->prev!=NULL) {
                b=a->prev;
                free(a);
                a=b;
            }
            free(a);
        }
        /*liberar el bignum*/
        free(bignum);
    }

    return;
}

comp_t comparar_modulo_bignum_e_igualar_largo(bignum_t* a,bignum_t* b) {

    int i;
    error_t return_value=NO_ERROR;

    /* hacer que los bignum tengan la misma cantidad de digitos,
     * agregandoles ceros a la izquierda*/
    if ((*a).length != (*b).length) {
        if ( (*a).length < (*b).length ) {
            return_value=agregar_ceros_a_la_izquierda(a,((*b).length)-(*a).length);
        } else {
            return_value=agregar_ceros_a_la_izquierda(b,((*a).length)-(*b).length);
        }
    }

    if (return_value==ERROR_MEMORY) return ERROR_MEM;

    /* Ahora las longitudes son iguales, comparar desde las cifras mas significativas
     * hacia las menos significativas hasta encontrar una cifra mayor que otra*/
    for (i=0;i<((*a).length);i++) {
        if ( obtener_nodo_numero(a->primernodo,i)->digit != obtener_nodo_numero(b->primernodo,i)->digit ) {

            if (obtener_nodo_numero(a->primernodo,i)->digit > obtener_nodo_numero(b->primernodo,i)->digit) {
                return A_ES_MAYOR;
            } else {
                return B_ES_MAYOR;
            }
        }
    }

    return A_Y_B_IGUALES;
}

error_t agregar_ceros_a_la_izquierda(bignum_t* num,int cantidad) {

    int i;

    /*cambiar la longitud y asignar memoria para nuevos digitos*/
    (*num).length+=cantidad;
    if ( agregar_nodos_al_comienzo (num,cantidad) == ERROR_MEMORY) return ERROR_MEMORY;
    actualizar_primer_nodo(num);

    return NO_ERROR;
}

/*desplaza los digitos a la izquierda para eliminar ceros a la izquierda*/
error_t quitar_ceros_de_mas(bignum_t* num) {

    int i;
    digito_n *a=NULL,*b=NULL;

    while ( (*num).length > 1 && num->primernodo->digit==0 ) {

        (*num).length--;
        a=obtener_primer_nodo(num->primernodo);
        b=a->sig;
        free(a);
        b->prev=NULL;
        num->primernodo=b;
    }

    return NO_ERROR;
}

void invertir_signo_bignum ( bignum_t* a) {

     switch((*a).sign) {
        case SIGNO_POSITIVO: (*a).sign=SIGNO_NEGATIVO;
            break;
        case SIGNO_NEGATIVO: (*a).sign=SIGNO_POSITIVO;
            break;
     }

    return;
}

/*devuelve NULL si hubo un error de memoria*/
bignum_t* copiar_bignum(const bignum_t* bn_in) {

    int i;
    bignum_t *resultado=NULL;

    resultado=armar_bignum();
    if (resultado==NULL) return NULL;

    resultado->length=(*bn_in).length;
    resultado->sign=(*bn_in).sign;
    resultado->primernodo=NULL;
    if (agregar_nodos_al_final(resultado,bn_in->length) == ERROR_MEMORY ) return NULL;
    for (i=0;i<(*resultado).length;i++) {
        desplazarse_n_nodos(resultado->primernodo,i)->digit=desplazarse_n_nodos(bn_in->primernodo,i)->digit;
    }

    return resultado;
}

/*devuelve NULL si hubo un error de memoria*/
bignum_t* armar_bignum_cero(void) {

    bignum_t* cero;

    cero=armar_bignum();
    if ( cero ==NULL) return NULL;
    (*cero).length=1;
    cero->primernodo=(digito_n*)malloc(sizeof(digito_n));
    cero->primernodo->digit=0;
    cero->primernodo->prev=NULL;
    cero->primernodo->sig=NULL;
    return cero;
}

/*devuelve NULL si hubo un error de memoria*/
bignum_t* armar_bignum(void) {

    bignum_t* num;

    num=(bignum_t*)malloc(sizeof(bignum_t));
    if ( num ==NULL) return NULL;
    (*num).length=0;
    (*num).sign=SIGNO_POSITIVO;
    num->primernodo = NULL;
    return num;
}

void invertir_posicion_digitos(bignum_t *input) {

    digito_n *nodo=NULL,*aux=NULL;
    bool_t seguir=TRUE;

    nodo=obtener_primer_nodo(input->primernodo);
    while (nodo!=NULL) {
        aux=nodo->prev; /*intercambiar prev y sig*/
        nodo->prev=nodo->sig;
        nodo->sig=aux;

        nodo=nodo->prev; /*ir al siguiente*/
    }

    actualizar_primer_nodo(input);
    return;
}

error_t multiplicar_por_potencia_de_10(bignum_t* num,int exp) {

    int i;

    /*cambiar la longitud y asignar memoria para nuevos digitos*/
    (*num).length+=exp;
    if (agregar_nodos_al_final(num,exp) == ERROR_MEMORY ) return ERROR_MEMORY;

    return NO_ERROR;
}

void actualizar_primer_nodo(bignum_t* bn) {

	bn->primernodo=obtener_primer_nodo(bn->primernodo);

	return;
}

error_t agregar_nodos_al_final (bignum_t *bn,int cantidad) {

	digito_n *nodo_nuevo=NULL,*nodo=NULL;
	int i;

	if (bn->primernodo==NULL) {
		nodo_nuevo=(digito_n*)malloc(sizeof(digito_n));
		if (nodo_nuevo==NULL) return ERROR_MEMORY;
		nodo_nuevo->prev=NULL;
		nodo_nuevo->sig=NULL;
		nodo_nuevo->digit=0; /*no borrar*/
		bn->primernodo=nodo_nuevo;
		cantidad--;
		/*return NO_ERROR;*/
	}
	nodo=obtener_ultimo_nodo(bn->primernodo);
	for (;cantidad>0;cantidad--) {
		nodo_nuevo=(digito_n*)malloc(sizeof(digito_n));
		if (nodo_nuevo==NULL) return ERROR_MEMORY;
		nodo_nuevo->prev=nodo;
		nodo_nuevo->digit=0; /*no borrar*/
		nodo_nuevo->sig=NULL;
		nodo->sig=nodo_nuevo;
		actualizar_primer_nodo(bn);
		nodo=nodo_nuevo;
	}

	return NO_ERROR;
}


bignum_t* multiplicar_por_una_cifra(bignum_t* num,unsigned short cifra) {

    bignum_t* resultado=NULL;
    int acumulador=0,carry=0,i;

    resultado=armar_bignum();
    if (resultado==NULL) return NULL;

    /*de derecha a izquierda*/
    for (i=(*num).length-1;i>=0;i--) {
        /*hacer producto*/
        acumulador=desplazarse_n_nodos(num->primernodo,i)->digit*cifra+carry;
        /*calcular digito final y proximo carry*/
        carry=acumulador/10;
        acumulador-=carry*10;
        /*aumentar memoria del resultado*/
        (*resultado).length++;
        agregar_nodos_al_final(resultado,1);
        /*guardar digito*/
        desplazarse_n_nodos(resultado->primernodo,resultado->length-1)->digit=acumulador;

    }

    /*guardar ultimo carry*/
    if (carry!=0) {
        /*aumentar memoria del resultado*/
        (*resultado).length++;
        if (agregar_nodos_al_final(resultado,1) == ERROR_MEMORY ) return NULL;
        /*guardar digito*/
        desplazarse_n_nodos(resultado->primernodo,resultado->length-1)->digit=carry;

    }

    /*invertir la posicion de los digitos para que el primero sea el mas significativo*/
    invertir_posicion_digitos(resultado);

    return resultado;
}

error_t agregar_nodos_al_comienzo (bignum_t *bn,int cantidad) {

	digito_n *nodo_nuevo=NULL,*nodo=NULL;
	int i;

	if (bn->primernodo==NULL) {
		nodo_nuevo=(digito_n*)malloc(sizeof(digito_n));
		if (nodo_nuevo==NULL) return ERROR_MEMORY;
		nodo_nuevo->prev=NULL;
		nodo_nuevo->sig=NULL;
		nodo_nuevo->digit=0; /*no borrar*/
		bn->primernodo=nodo_nuevo;
		cantidad--;
		/*return NO_ERROR;*/
	}
	nodo=obtener_primer_nodo(bn->primernodo);
	for (;cantidad>0;cantidad--) {
		nodo_nuevo=(digito_n*)malloc(sizeof(digito_n));
		if (nodo_nuevo==NULL) return ERROR_MEMORY;
		nodo_nuevo->sig=nodo;
		nodo_nuevo->digit=0; /*no borrar*/
		nodo_nuevo->prev=NULL;
		nodo->prev=nodo_nuevo;
		actualizar_primer_nodo(bn);
		nodo=nodo_nuevo;
	}

	return NO_ERROR;
}
