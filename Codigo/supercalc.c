#include "supercalc.h"

#define TEXT_CALCULATE "#calculate"
#define TEXT_CALCULATE_2 "#"
#define TEXT_UNKNOWN_ERROR "Error desconocido.\n"
#define TEXT_ERROR_MEMORY "Error al intentar pedir memoria al sistema.\n"
#define TEXT_ERROR_NO_DIGIT_OPERAND "No se pudieron encontrar los digitos de un operando.\n"
#define TEXT_ERROR_INVALID_OPERATION "No pudo encontrarse una operación matemática válida.\n"
#define TEXT_ERROR_END_OF_STRING "Se encontró una expresión posiblemente correcta pero con un final inesperado.\n"
#define TEXT_OVERFLOW "Infinito / Overflow"

#define CHAR_NEGATIVE_SIGN '-'
#define CHAR_OP_SUM '+'
#define CHAR_OP_RES '-'
#define CHAR_OP_MUL '*'
#define ASCII_NUMBER_ZERO '0'
#define ASCII_NULL '\0'
#define ASCII_RETURN '\n'

typedef struct {
    bignum_t *op1, *op2 , *result;
    opt_t op;
    long precision;
    bool_t overflow;
} operation_t;

void liberar_memoria_de_supercalc(operation_t**,int);
error_t convertir_string_a_operacion(char*, operation_t*);
error_t guardar_un_operando(bignum_t* ,char* ,int* );
error_t realizar_operacion(operation_t*);

int supercalculadora(const int supercalc_presicion) {

    /*tamaño para que entren dos operandos, 3 signos y \n\0*/
    char expresion_ingresada[(OP_MAX_LENGTH*2+5)];
    operation_t* operaciones=NULL;
    int cantidad_operaciones=0,i,j;
    error_t return_value;

    /*repetir hasta que se introduzca #calculate */
    do {

        /*leer hasta un salto de línea o espacio*/
        scanf("%s",expresion_ingresada);

        /*si no se introdujo #calculate:*/
        if (strcmp(expresion_ingresada,TEXT_CALCULATE)!=0 && strcmp(expresion_ingresada,TEXT_CALCULATE_2)!=0 ) {

            /*nueva operacion*/
            cantidad_operaciones++;

            /*aumentar la memoria de los punteros a operaciones*/
            operaciones=(operation_t*)realloc(operaciones,sizeof(operation_t)*cantidad_operaciones);
            if (operaciones==NULL) {
                printf(TEXT_ERROR_MEMORY);
                liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
                return EXIT_FAILURE;
            }

            return_value=convertir_string_a_operacion(expresion_ingresada,&operaciones[cantidad_operaciones-1]);
            switch (return_value) {
                case NO_ERROR:
                    break;
                case ERROR_END_OF_STRING:
                    printf(TEXT_ERROR_END_OF_STRING);
                    liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
                    return EXIT_FAILURE;
                case ERROR_INVALID_OPERATION:
                    printf(TEXT_ERROR_INVALID_OPERATION);
                    liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
                    return EXIT_FAILURE;
                case ERROR_NO_DIGIT_OPERAND:
                    printf(TEXT_ERROR_NO_DIGIT_OPERAND);
                    liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
                    return EXIT_FAILURE;
                case ERROR_MEMORY:
                    printf(TEXT_ERROR_MEMORY);
                    liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
                    return EXIT_FAILURE;
                case ERROR_DESCONOCIDO:
                default:
                    printf(TEXT_UNKNOWN_ERROR);
                    liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
                    return EXIT_FAILURE;
            }
        }

    } while(strcmp(expresion_ingresada,TEXT_CALCULATE)!=0 && strcmp(expresion_ingresada,TEXT_CALCULATE_2)!=0 && return_value==NO_ERROR);

    /* calcula las operaciones*/

    for (i=0; i<cantidad_operaciones; i++) {
        operaciones[i].precision=supercalc_presicion;
        return_value=realizar_operacion(&operaciones[i]);
        if (return_value==ERROR_MEMORY) {
            printf(TEXT_ERROR_MEMORY);
            liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
            return EXIT_FAILURE;
        }

    }

    /*muestra los resultados*/
    for (i=0; i<cantidad_operaciones; i++) {
        if ((operaciones[i]).overflow == FALSE) {
            if ((*(operaciones[i].result)).sign == SIGNO_NEGATIVO) printf("-");
            for (j=0;j<((*(operaciones[i].result)).length);j++){
                printf("%hu",obtener_nodo_numero((operaciones[i].result)->primernodo,j)->digit);
            }
        } else {
            printf(TEXT_OVERFLOW);  /*imprimir infinito si hay overflow*/
        }
        printf("\n");
    }

    liberar_memoria_de_supercalc(&operaciones,cantidad_operaciones);
    return EXIT_SUCCESS;
}

error_t realizar_operacion(operation_t* operacion) {

    bool_t invertir_signo=FALSE;

    switch ((*operacion).op) {
        case OPC_SUM:   /*Suma*/

            if ((*(*operacion).op1).sign == SIGNO_NEGATIVO) invertir_signo=TRUE;
            /*signos distintos*/
            if ((*(*operacion).op1).sign != (*(*operacion).op2).sign) {

                (*operacion).result=substract_bignum((*operacion).op1,(*operacion).op2);
            } else {    /*signos iguales*/
                (*operacion).result=add_bignum((*operacion).op1,(*operacion).op2);
            }
            break;

        case OPC_RES:

            if ((*(*operacion).op1).sign == SIGNO_NEGATIVO) invertir_signo=TRUE;
            /*signos distintos*/
            if ((*(*operacion).op1).sign != (*(*operacion).op2).sign) {

                (*operacion).result=add_bignum((*operacion).op1,(*operacion).op2);
            } else {    /*signos iguales*/
                (*operacion).result=substract_bignum((*operacion).op1,(*operacion).op2);
            }
            break;

        case OPC_MUL:
            (*operacion).result=multiply_bignum((*operacion).op1,(*operacion).op2);
            break;

        default:
            printf(TEXT_UNKNOWN_ERROR);
            break;
    }

    /*indicar si hubo un error de memoria*/
    if ((*operacion).result == NULL) return ERROR_MEMORY;

    /*corregir signo*/
    if (invertir_signo) invertir_signo_bignum((*operacion).result);

    /*guardar overflow*/
    if ((*(*operacion).result).length > (*operacion).precision) {
        (*operacion).overflow=TRUE;
    } else {
        (*operacion).overflow=FALSE;
    }

    return NO_ERROR;
}

void liberar_memoria_de_supercalc(operation_t** puntero_a_array_de_operaciones,int cantidad_operaciones) {

    int i;

    if (puntero_a_array_de_operaciones!=NULL){
        for (i=0;i<cantidad_operaciones;i++) {
            if (&((*puntero_a_array_de_operaciones)[i])!=NULL) {
                liberar_memoria_bignum(((*puntero_a_array_de_operaciones)[i]).op1);
                liberar_memoria_bignum(((*puntero_a_array_de_operaciones)[i]).op2);
                liberar_memoria_bignum(((*puntero_a_array_de_operaciones)[i]).result);
            }
        }
        free(*puntero_a_array_de_operaciones);  /*liberar el array de operaciones*/
    };


    return;
}

error_t convertir_string_a_operacion(char* expresion_ingresada, operation_t* operacion) {

    int expresion_ingresada_puntero,return_value;

    /*Inicializar bignum*/
    (*operacion).op1=NULL;
    (*operacion).op2=NULL;
    (*operacion).result=NULL;

    /*asignar memoria a nuevos bignum*/
    (*operacion).op1=armar_bignum();
    if ((*operacion).op1==NULL) return ERROR_MEMORY;
    (*operacion).op2=armar_bignum();
    if ((*operacion).op2==NULL) return ERROR_MEMORY;
    /*a el resultado se le asignara memoria durante el calculo.*/

    /*Ir al inicio de la expresión*/
    expresion_ingresada_puntero=0;

    /*guardar el primer operando*/
    return_value=guardar_un_operando((*operacion).op1,expresion_ingresada,&expresion_ingresada_puntero);

    if (return_value==ERROR_NO_DIGIT_OPERAND) return ERROR_NO_DIGIT_OPERAND;

    /*leer la operación*/
    switch (expresion_ingresada[expresion_ingresada_puntero]) {
        case CHAR_OP_SUM:
            (*operacion).op=OPC_SUM;
            break;
        case CHAR_OP_RES:
            (*operacion).op=OPC_RES;
            break;
        case CHAR_OP_MUL:
            (*operacion).op=OPC_MUL;
            break;
        default:
            return ERROR_INVALID_OPERATION;

    }

    /*correr el puntero a la derecha de la operación*/
    expresion_ingresada_puntero++;

    /*guardar el segundo operando*/
    return_value=guardar_un_operando((*operacion).op2,expresion_ingresada,&expresion_ingresada_puntero);
    if (return_value==ERROR_NO_DIGIT_OPERAND) return ERROR_NO_DIGIT_OPERAND;

    /*Por último, verificar que la expresión termine con \n o \0 .*/
    if (expresion_ingresada[expresion_ingresada_puntero]!=ASCII_RETURN && expresion_ingresada[expresion_ingresada_puntero]!=ASCII_NULL) {

        return ERROR_END_OF_STRING;

    }

    return NO_ERROR;
}

/*
 * Guardar_un_operando: Analiza la expresión a partir de (expresion_ingresada_puntero) , que
 * debe tener la posición donde empezaría el operando.
 *
 * Salidas:
 * operando-> sign ; length ; digits. (también se encarga de asignar memoria para los dígitos)
 * *expresion_ingresada_puntero : devuelve la posición donde terminó el operando.
 * Salida común tipo int: devuelve EXIT_SUCCESS o un error (hay varios tipos de error)
 */
error_t guardar_un_operando(bignum_t* operando,char* expresion_ingresada,int* expresion_ingresada_puntero) {

    /*primer carácter: buscar un signo '-'*/
    if (expresion_ingresada[*expresion_ingresada_puntero]==CHAR_NEGATIVE_SIGN) {
        (*operando).sign=SIGNO_NEGATIVO;
        /*apuntar al inicio del operando*/
        (*expresion_ingresada_puntero)++;
    } else {
        if (isdigit(expresion_ingresada[*expresion_ingresada_puntero])==0) {
            /*si el primer carácter no es '-' ni es un numero*/
			return ERROR_NO_DIGIT_OPERAND;
        }
        /*si el primer carácter es un numero*/
        (*operando).sign=SIGNO_POSITIVO;
    }

    /*guardar los dígitos
    operando->primernodo=(digito_n*)malloc(sizeof(digito_n));
    if (operando->primernodo==NULL) return ERROR_MEMORY;*/

    while(isdigit(expresion_ingresada[*expresion_ingresada_puntero])) {

        (*operando).length++;
        /*agregar un nodo*/
        agregar_nodos_al_final(operando,1);
        /* convertir de char ascii a un digito, y guardarlo */
        obtener_ultimo_nodo(operando->primernodo)->digit=expresion_ingresada[*expresion_ingresada_puntero]-ASCII_NUMBER_ZERO;
        /*enlazar los nodos*/


        (*expresion_ingresada_puntero)++;
    }

    /*Si no se encontraron dígitos*/
    if ((*operando).length==0) {
        return ERROR_NO_DIGIT_OPERAND;
    }

    return NO_ERROR;
}
