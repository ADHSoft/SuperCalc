#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "simplecalc.h"
#include "supercalc.h" /*para tambien poder ejecutarlo desde el menú*/

/* Definiciones para los textos de los mensajes*/
#define TEXT_TITLE "  Calculadora Graficadora\n"
#define TEXT_MENU_MAIN "  Menú principal.\n  Seleccione una de las siguientes operaciones:\n\n1- Suma de dos numeros reales\n2- Resta de dos numeros reales\n3- División de dos numeros reales\n4- Producto de dos numeros reales\n5- Potenciación de un numero real elevado a un num. entero\n6- Factorial de un numero natural\n7- Coeficiente binomial de dos naturales\n8- Número aleatorio dentro de un intervalo\n9- Graficar\n10-Ejecutar supercalculadora\n\n\nPor favor ingrese el número correspondiente a la función y luego presione ENTER.\n"
#define TEXT_INVALID_OPT "La opción ingresada es invalida.\n"
#define TEXT_INVALID_PARAM "Operación invalida para tales parametros.\n"
#define TEXT_PRINT_RESULT_DOUBLE "  Resultado: %.4f\n"      /* %.4f para imprimir 4 cifras después de la coma*/
#define TEXT_PRINT_RESULT_UNSIGNED_LONG "  Resultado: %lu\n"
#define TEXT_SUM_1 "Ingrese el primer sumando: "
#define TEXT_SUM_2 "Ingrese el segundo sumando: "
#define TEXT_RES_1 "Ingrese el minuendo: "
#define TEXT_RES_2 "Ingrese el sustraendo: "
#define TEXT_DIV_1 "Ingrese el dividendo: "
#define TEXT_DIV_2 "Ingrese el divisor: "
#define TEXT_MUL_1 "Ingrese el multiplicando: "
#define TEXT_MUL_2 "Ingrese el multiplicador: "
#define TEXT_POT_1 "Ingrese la base: "
#define TEXT_POT_2 "Ingrese el exponente, que debe ser entero: "
#define TEXT_FAC_1 "Introduzca el numero a factorizar: "
#define TEXT_COMB_1 "Introduzca n: "
#define TEXT_COMB_2 "Introduzca k: "
#define TEXT_RANDOM_1 "Introduzca el primer limite: "
#define TEXT_RANDOM_2 "Introduzca el segundo limite: "
#define TEXT_ASTERISKS "***********************\n"

typedef enum { TIPO_UNSIGNED_LONG , TIPO_DOUBLE , TIPO_GRAFICO } data_t;

void imprimir_resultado(double,unsigned long,data_t,bool_t);
bool_t hacer_un_calculo(opt_t,char[FILENAME_MAX]);
void imprimir_menu(void);

bool_t simplecalc(char filename[FILENAME_MAX]) {

    /*Variables para el menú*/
    int opcion_num=1;
    opt_t opcion;

    imprimir_menu();
    scanf("%d",&opcion_num);            /*Recibir por teclado el número de opción*/
    opcion=opcion_num;

    return hacer_un_calculo(opcion,filename);    /*Devolver parametros_validos*/
}

bool_t hacer_un_calculo(opt_t opcion,char filename[FILENAME_MAX]) {

    /*Parámetros de las operaciones matemáticas*/
   	double real_1=0,real_2=0,resultado_double=0;
	int entero_1=0,return_value;
	unsigned long long_1=0,long_2=0,resultado_unsigned_long=0;
	data_t tipo_de_resultado;
    bool_t parametros_validos=TRUE;		/*Variables para indicar si los datos fueron validados. Es importante inicializarlos.*/
    bool_t opcion_valida=TRUE;

	switch(opcion){
        case OPC_SUM:		/*Suma*/
            printf(TEXT_SUM_1);
            scanf("%lf",&real_1);
            printf(TEXT_SUM_2);
            scanf("%lf",&real_2);
            resultado_double=suma(real_1,real_2);
            tipo_de_resultado=TIPO_DOUBLE;
            break;

        case OPC_RES:		/*Resta*/
            printf(TEXT_RES_1);
            scanf("%lf",&real_1);
            printf(TEXT_RES_2);
            scanf("%lf",&real_2);
            resultado_double=resta(real_1,real_2);
            tipo_de_resultado=TIPO_DOUBLE;
            break;

        case OPC_DIV:		/*División*/
            printf(TEXT_DIV_1);
            scanf("%lf",&real_1);
            printf(TEXT_DIV_2);
            scanf("%lf",&real_2);
            if ( real_2 != 0 ) {            /*para la division , el dividendo no puede ser 0 .*/
                    resultado_double=division(real_1,real_2);
                    tipo_de_resultado=TIPO_DOUBLE;
            } else {
                    parametros_validos=FALSE;
            }
            break;

        case OPC_MUL:		/*Producto*/
            printf(TEXT_MUL_1);
            scanf("%lf",&real_1);
            printf(TEXT_MUL_2);
            scanf("%lf",&real_2);
            resultado_double=producto(real_1,real_2);
            tipo_de_resultado=TIPO_DOUBLE;
            break;

        case OPC_POT:		/*Potencia*/
            printf(TEXT_POT_1);
            scanf("%lf",&real_1);
            printf(TEXT_POT_2);
            scanf("%d",&entero_1);
                    /*Validar datos*/
            if ( real_1 == 0 && entero_1 < 1 ) {              /*Si la base es 0 , el exponente no puede ser menor a uno.*/
                    parametros_validos=FALSE;
            } else {
                    resultado_double=potencia(real_1,entero_1);
                    tipo_de_resultado=TIPO_DOUBLE;
            }
            break;

        case OPC_FAC:		/*Factorial*/
            printf(TEXT_FAC_1);
            scanf("%lu",&long_1);
            if (entero_1<0) {                                      /*No se puede factorizar un numero negativo.*/
                parametros_validos=FALSE;  /*Indicar que los parametros de la operacion son invalidos*/
            } else {
                resultado_unsigned_long=factorial(long_1);
                tipo_de_resultado=TIPO_UNSIGNED_LONG;
            }

            break;

        case OPC_COMB:		/*Número combinatorio*/
            printf(TEXT_COMB_1);
            scanf("%lu",&long_1);
            printf(TEXT_COMB_2);
            scanf("%lu",&long_2);
                /*Validar datos:*/
            if ( long_1 < 0 || long_2 < 0 || long_2 > long_1 ) {
                parametros_validos=FALSE;    /*ni 'n' ni 'm' pueden ser negativos, y m no puede ser mayor que n.*/
            } else {
                resultado_unsigned_long=num_combinatorio(long_1,long_2);
                tipo_de_resultado=TIPO_UNSIGNED_LONG;
            }

            break;

        case OPC_RANDOM:	/*Número aleatorio*/
            printf(TEXT_RANDOM_1);
            scanf("%lf",&real_1);
            printf(TEXT_RANDOM_2);
            scanf("%lf",&real_2);

            resultado_double=num_aleatorio(real_1,real_2);
            tipo_de_resultado=TIPO_DOUBLE;
            break;

        case OPC_GRAFICO:
            tipo_de_resultado=TIPO_GRAFICO;
            graficador(&parametros_validos,&opcion_valida,filename);

            break;
        case OPC_SUPERCALC:
            return_value=supercalculadora(SUPERCALC_DEFAULT_PRECISION);
            switch (return_value) {
                case EXIT_FAILURE: parametros_validos=FALSE;
                    break;
                case EXIT_SUCCESS: parametros_validos=TRUE;
                    break;
            }
            return parametros_validos;
        default:
            opcion_valida=FALSE;	/*Si el número ingresado es menor a uno o mayor a 8, no sera tomado como válido.*/
            parametros_validos=FALSE;
            break;
	}

	if (opcion_valida == TRUE ) {
		imprimir_resultado(resultado_double,resultado_unsigned_long,tipo_de_resultado,parametros_validos);	/*Imprimir resultado o error*/
	} else {
        printf(TEXT_INVALID_OPT);
	}

	return parametros_validos;

}

double suma(double n1,double n2) {

        return n1+n2;
}

double resta(double n1,double n2) {

        return n1-n2;
}

double division(double n1,double n2) {

        /*Como no se puede usar pasaje por puntero, solo podemos devolver un resultado,
        * sin un mensaje de error. Por lo tanto comprobar que el dividendo no sea 0
        * antes de llamar a esta función.*/
        return n1/n2;
}

double producto(double n1,double n2) {

        return n1*n2;
}

/* Funcion potencia :
 * Recibe un numero real y un entero.
 * Devuelve el resultado.
 */
double potencia(double base_1, int exponente) {

        int i = 0, exponente_negativo=0;
        double resultado = 1;						/*Se lo usa como acumulador.*/

        if ( exponente < 0 ) {						/*Si el exponente es negativo:*/
                exponente_negativo=1;               /*se anota que es negativo*/
                exponente=(exponente*(-1));         /*y se le cambia el signo.*/
        }

        for ( i=0 ; i < exponente ; i++ ) {			/*Ir multiplicando por la base el acumulador, "exponente" veces*/
                resultado = resultado*base_1;
        }

        if ( exponente_negativo ==0 ) {
                return (resultado);
        } else {
                return (1/resultado); /*Si el exponente era negativo, el resultado final será el acumulador elevado a (-1)*/
        }
 }

/* Funcion factorial :
 * Recibe un solo numero natural.
 * Devuelve el resultado.
*/
long factorial(long n) {

        int i;
        double fact = 1;		/*El acumulador se inicia en 1.*/

		for (i=1;i<=n;i++) {    /*hacer "n" veces*/
				fact = fact * i;        /*multiplicar fact por 1, por 2, por 3...*/
		}

        return fact;

}

/* Funcion num_combinatorio :
 * Recibe dos numeros enteros.
 * Devuelve el resultado, empleando la funcion factorial.
 */
long num_combinatorio (long n, long k) {

		return factorial(n) / ( factorial(k) * factorial(n-k) );

}

void imprimir_menu() {


        printf(TEXT_ASTERISKS);
        printf(TEXT_TITLE);
        printf(TEXT_ASTERISKS);
        printf(TEXT_MENU_MAIN);

        return;

}

double num_aleatorio (double limite_1, double limite_2) {

        /*Convierte los limites de la salida de drand a los que se requieran.*/
        return (drand() * (limite_2-limite_1)) + limite_1;

}

double drand() {

        srand(time (NULL));		/*Se utiliza el timestamp del sistema como semilla.*/

        return (double)rand()/(double)RAND_MAX;	/*Devuelve un número aleatorio entre 0 y 1*/

}

/* Función imprimir_resultado :
 * Lee si los parámetros de la operación matemática fueron válidos,
 * desde una variable de main, (usando un puntero).
 * Si hubo una operación matemática válida, entonces imprime el resultado.
 * Sino, imprime un error.
*/
void imprimir_resultado(double resultado_double,unsigned long resultado_long,data_t tipo_de_resultado,bool_t parametros_validos) {
    if ( parametros_validos == TRUE ) {
            printf(TEXT_ASTERISKS);
            switch (tipo_de_resultado) {
                case TIPO_DOUBLE:
                    printf(TEXT_PRINT_RESULT_DOUBLE,resultado_double);
                    break;
                case TIPO_UNSIGNED_LONG:
                    printf(TEXT_PRINT_RESULT_UNSIGNED_LONG,resultado_long);
                    break;
                case TIPO_GRAFICO:
                    break;
            }
            printf(TEXT_ASTERISKS);

    } else {
            printf(TEXT_INVALID_PARAM);
    }
}
