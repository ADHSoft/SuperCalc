/*
	Universidad de Buenos Aires - FIUBA

	Algoritmos y Programación I (75.02/95.11)
	Segundo cuatrimestre de 2015

	Trabajo Práctico N°3 : Módulo aritmético de precisión arbitraria

	Alumnos:

    Herme, Alejandro David
		Padrón 97646

    Álvarez Windey, Juan
        Padrón 95242

    Clavijo, Jorge
        Padrón 95485

*/

/*

    Para compilar: ejecutar "make"

*/

/* Directivas de preprocesador */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "graficador.h"
#include "tipos_definidos.h"
#include "simplecalc.h"
#include "supercalc.h"

#define TEXT_ARGS_SIMPLECALC "simpleCalc"
#define TEXT_ARGS_SUPERCALC "superCalc"
#define TEXT_ARGS_SUPERCALC_PRECISION "-p"
#define TEXT_ARGS_INPUT "-input"
#define TEXT_ARGS_OUTPUT "-output"
#define TEXT_CLI_ERROR "Los parámetros introducidos por la línea de comandos no son válidos.\nLos parámetros posibles son: \"simpleCalc\", \"superCalc\" o \"superCalc -p xxxx\", donde xxxx es la precisión de los resultados.\n\"-input nombre_archivo_entrada\" y \"-output nombre_archivo_salida\" se pueden agregar para guardar resultados o leer operaciones.\n"

/*Prototipos de funciones*/
opt_t parse_args(int,char **, int *,char[FILENAME_MAX],char [FILENAME_MAX]);

/* Programa principal */
int main ( int argc, char *argv[] ) {

        int supercalc_precision=SUPERCALC_DEFAULT_PRECISION,return_value;
        opt_t opcion;
        /*Variable para indicar si los datos fueron validados. Es importante inicializarlo.*/
        bool_t parametros_validos=TRUE;
        char filename_out[FILENAME_MAX];
        char filename_in[FILENAME_MAX];

        strcpy(filename_in,"\0");
        strcpy(filename_out,"\0");

        /*comprobar los argumentos de línea de comandos*/
        opcion=parse_args(argc,argv,&supercalc_precision,filename_in,filename_out);

        switch (opcion) {
            case OPC_SIMPLECALC:
                parametros_validos=simplecalc(filename_out);
                break;
            case OPC_SUPERCALC:
                return_value=supercalculadora(supercalc_precision);
                switch (return_value) {
                    case EXIT_FAILURE: parametros_validos=FALSE;
                        break;
                    case EXIT_SUCCESS: parametros_validos=TRUE;
                        break;
                }
                break;
            default:
                printf(TEXT_CLI_ERROR);
                parametros_validos=FALSE;
                break;

        }


        if ( parametros_validos == TRUE ) {
                return EXIT_SUCCESS;    /* si se realizó una operacion con éxito.*/
        } else {
                return EXIT_FAILURE;    /* si se introdujo una opción invalida o parámetros inválidos.*/
        };

} /*  Fin de Main ---------------------*/


opt_t parse_args(int argc,char* argv[],int* supercalc_precision,char filename_in[FILENAME_MAX],char filename_out[FILENAME_MAX]){

    opt_t opcion=OPC_SIMPLECALC;
    int i;

    for (i=1;i<argc;i++) {
        if (strcmp(argv[i],TEXT_ARGS_SIMPLECALC)==0) opcion=OPC_SIMPLECALC;
        if (strcmp(argv[i],TEXT_ARGS_SUPERCALC)==0) opcion=OPC_SUPERCALC;
        if (strcmp(argv[i],TEXT_ARGS_SUPERCALC_PRECISION)==0 ) {
            i++;
            if (!(i<argc)) return OPC_DESCONOCIDA;
            *supercalc_precision=atoi(argv[i]);
        }
        if (strcmp(argv[i],TEXT_ARGS_INPUT)==0 ) {
            i++;
            strcpy(filename_in,argv[i]);
        }
        if (strcmp(argv[i],TEXT_ARGS_OUTPUT)==0 ) {
            i++;
            strcpy(filename_out,argv[i]);
        }
    /*printf("%s , %d",argv[i],opcion);*/
    }

    return opcion;
}
