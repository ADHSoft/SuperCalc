#ifndef TIPOS_DEFINIDOS_H_INCLUDED
#define TIPOS_DEFINIDOS_H_INCLUDED

#define TEXT_FILE_ERROR "Error al intentar guardar/abrir un archivo de nombre \"%s\".\n"

/* Definiciones de tipos enumerados */
/* Es importante conservar el orden de los primeros 10 elementos de opt_t*/
typedef enum { FALSE , TRUE } bool_t;
typedef enum { OPC_DESCONOCIDA , OPC_SUM , OPC_RES , OPC_DIV , OPC_MUL , OPC_POT , OPC_FAC , OPC_COMB , OPC_RANDOM , OPC_GRAFICO , OPC_SUPERCALC , OPC_SENO , OPC_COSENO , OPC_POLI_G0 , OPC_POLI_G1 , OPC_POLI_G2 , OPC_POLI_G3 , OPC_LOGARITMO , OPC_EXPONENCIAL , OPC_SIMPLECALC } opt_t;
typedef enum { SIGNO_POSITIVO , SIGNO_NEGATIVO } sign_t;
typedef enum { NO_ERROR , ERROR_INVALID_OPERATION , ERROR_NO_DIGIT_OPERAND , ERROR_END_OF_STRING , ERROR_MEMORY , ERROR_ARCHIVOS_IO , ERROR_DESCONOCIDO } error_t;

#endif
