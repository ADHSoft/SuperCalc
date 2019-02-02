#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graficador.h"
#include "tipos_definidos.h"

#define TEXT_MENU_GRAPH "Escriba que función desea graficar. Las funciones disponibles son:\n\tTrigonométricas: seno ; coseno\n\tPolinómicas: grado0 ; grado1 ; grado2 ; grado3\n\tEspeciales: ln ; exp\n\nLos polinomios deberán estar ordenados, 'a' será el coeficiente de mayor grado, seguido de 'b' 'c' y 'd' como los demás coeficientes en orden (si corresponde).\n\nLa forma del logaritmo y de la exponencial será la siguiente:\nln:  ln( a*x + b) + c\nexp: exp( a*x + b) + c\n\nPor favor use sólo minúsculas.\n"
#define TEXT_INPUT_SENO "seno"
#define TEXT_INPUT_COSENO "coseno"
#define TEXT_INPUT_POLI_G0 "grado0"
#define TEXT_INPUT_POLI_G1 "grado1"
#define TEXT_INPUT_POLI_G2 "grado2"
#define TEXT_INPUT_POLI_G3 "grado3"
#define TEXT_MAX_LENGTH 10
#define TEXT_INPUT_LOGARITMO "ln"
#define TEXT_INPUT_EXPONENCIAL "exp"
#define TEXT_ERROR_PERIOD "Ha ingresado un número que no puede ser un período.\n"
#define TEXT_TRIGONOMETRIC_1 "Ingrese amplitud\n"
#define TEXT_TRIGONOMETRIC_2 "Ingrese período\n"
#define TEXT_TRIGONOMETRIC_3 "Ingrese la fase en radianes\n"
#define TEXT_ENTER_A "Ingrese a\n"
#define TEXT_ENTER_B "Ingrese b\n"
#define TEXT_ENTER_C "Ingrese c\n"
#define TEXT_ENTER_D "Ingrese d\n"
#define TEXT_GRAF_OK "  Gráfico guardado en el archivo %s .\n"
#define DEFAULT_FILENAME "grafico.pbm"

#define MATRIZ_ANCHO_PX 600    /* ancho del gráfico en píxeles */
#define MATRIZ_ORIGEN_X 0    /* coordenadas x del origen (en pixeles)*/
#define MATRIZ_ORIGEN_Y 300     /* coordenadas y del origen (en pixeles)*/
#define MATRIZ_X_MIN_REAL 0.0  /* la mínima coordenada real del eje X */
#define MATRIZ_X_MAX_REAL 9.9833333333  /* la máxima coordenada real del eje X */
#define MATRIZ_Y_MIN_REAL -4.9833333333  /* la mínima coordenada real del eje Y */
#define MATRIZ_Y_MAX_REAL 5.0  /* la máxima coordenada real del eje Y */
#define MATRIZ_ANCHO_REAL 10.0  /* delta x del gráfico (el que se recorre de un costado al otro)*/
#define MATRIZ_ALTO_REAL 10.0   /* delta y del gráfico */
#define PBM_HEADER "P1\n"       /* encabezado del formato PBM */
#define COLOR_BLANCO 0
#define COLOR_NEGRO 1
#define NUMBER_PI 3.14159265359          /*número pi*/
#define NUMBER_E 2.71828182846          /*número de neper*/

opt_t traducir_palabra_a_opcion(char * );
void inicializar_matriz_y_dibujar_ejes (char[][MATRIZ_ALTO_PX]);
int coordenadas_reales_a_coordenadas_pixel_y(double);
int coordenadas_reales_a_coordenadas_pixel_x(double);
double coordenadas_pixel_a_coordenadas_reales_x(int);
int graficar_funcion(opt_t,double,double,double,double,double,double,double,char[][MATRIZ_ALTO_PX]);
error_t exportar_grafico_pbm(char[][MATRIZ_ALTO_PX],char*);
void imprimir_menu_graph(void);

void graficador(bool_t* parametros_validos , bool_t* opcion_valida , char filename[FILENAME_MAX]){       /*Devuelve el estado de la operación*/

    /*Matriz de píxeles. Codificación: un char por píxel, monocromos, 0=blanco 1=negro*/
    char Matriz[MATRIZ_ANCHO_PX][MATRIZ_ALTO_PX];

    char texto_entrada[TEXT_MAX_LENGTH]; /*Donde guardar el texto del teclado*/
    opt_t opcion;           /*Función matemática elegida*/
    double fase,amplitud,periodo,a=0,b=0,c=0,d=0;  /*Parámetros de las funciones matemáticas*/
    error_t ret_value;

    *parametros_validos=TRUE;   /*Inicialmente en TRUE*/
    *opcion_valida=TRUE;

    if (strlen(filename)==0) filename=DEFAULT_FILENAME;

    inicializar_matriz_y_dibujar_ejes(Matriz);
    imprimir_menu_graph();
    scanf("%s",texto_entrada);      /*sin '&' porque ya es vector*/
    opcion=traducir_palabra_a_opcion(texto_entrada);

    switch (opcion) {
        case OPC_SENO:
        case OPC_COSENO:
            printf(TEXT_TRIGONOMETRIC_1);
            scanf("%lf",&amplitud);
            printf(TEXT_TRIGONOMETRIC_2);
            scanf("%lf",&periodo);
            printf(TEXT_TRIGONOMETRIC_3);
            scanf("%lf",&fase);
            if (periodo>0) {
                graficar_funcion(opcion,amplitud,periodo,fase,0,0,0,0,Matriz);
            } else {
                printf(TEXT_ERROR_PERIOD);
                *parametros_validos=FALSE;
            }
            break;

        case OPC_POLI_G3:   /*las funciones que necesitan un a, b, c y d*/
            printf(TEXT_ENTER_D);
            scanf("%lf",&d);
        case OPC_POLI_G2:   /*las funciones que solo necesitan un a, b y c*/
        case OPC_LOGARITMO:
        case OPC_EXPONENCIAL:
            printf(TEXT_ENTER_C);
            scanf("%lf",&c);
        case OPC_POLI_G1:       /*las funciones que solo necesitan un a y b*/
            printf(TEXT_ENTER_B);
            scanf("%lf",&b);
        case OPC_POLI_G0:
            printf(TEXT_ENTER_A);
            scanf("%lf",&a);
            graficar_funcion(opcion,0,0,0,a,b,c,d,Matriz);
            break;

        default:
            *opcion_valida=FALSE;
            *parametros_validos=FALSE;
            break;
    }

    if(*parametros_validos==TRUE && *opcion_valida==TRUE) ret_value=exportar_grafico_pbm(Matriz,filename);
    if (ret_value==ERROR_ARCHIVOS_IO) printf(TEXT_FILE_ERROR,filename);

    return;

}

opt_t traducir_palabra_a_opcion(char * palabra){

    if (strcmp(palabra,TEXT_INPUT_SENO) == 0) return OPC_SENO;
    if (strcmp(palabra,TEXT_INPUT_COSENO) == 0) return OPC_COSENO;
    if (strcmp(palabra,TEXT_INPUT_POLI_G0) == 0) return OPC_POLI_G0;
    if (strcmp(palabra,TEXT_INPUT_POLI_G1) == 0) return OPC_POLI_G1;
    if (strcmp(palabra,TEXT_INPUT_POLI_G2) == 0) return OPC_POLI_G2;
    if (strcmp(palabra,TEXT_INPUT_POLI_G3) == 0) return OPC_POLI_G3;
    if (strcmp(palabra,TEXT_INPUT_LOGARITMO)== 0) return OPC_LOGARITMO;
    if (strcmp(palabra,TEXT_INPUT_EXPONENCIAL) == 0) return OPC_EXPONENCIAL;

    return OPC_DESCONOCIDA;
}

void inicializar_matriz_y_dibujar_ejes (char matriz[][MATRIZ_ALTO_PX]){
    int x_pixel,y_pixel;
    double x_real,y_real;

    /*formatear la matriz en blanco*/
    for (y_pixel=0;y_pixel<MATRIZ_ALTO_PX;y_pixel++){       /*barrido y*/
        for (x_pixel=0;x_pixel<MATRIZ_ANCHO_PX;x_pixel++) { /*barrido x*/
            matriz[x_pixel][y_pixel]=COLOR_BLANCO;
        }
    }

    /*eje y + divisiones verticales*/
    for (y_pixel=0;y_pixel<MATRIZ_ALTO_PX;y_pixel++){   /*barrido y*/
        matriz[MATRIZ_ORIGEN_X][y_pixel]=COLOR_NEGRO;   /*dibujar eje y*/
        matriz[MATRIZ_ORIGEN_X+1][y_pixel]=COLOR_NEGRO; /*agregar un pixel mas de grosor al eje*/

        /*dibujar las divisiones verticales*/
        for (x_real=(MATRIZ_X_MIN_REAL+1);x_real<=MATRIZ_X_MAX_REAL;x_real++)
            matriz[coordenadas_reales_a_coordenadas_pixel_x(x_real)][y_pixel]=COLOR_NEGRO;

    }

    for (x_pixel=0;x_pixel<MATRIZ_ANCHO_PX;x_pixel++){
        matriz[x_pixel][MATRIZ_ORIGEN_Y]=COLOR_NEGRO;     /*dibujar eje x*/
        matriz[x_pixel][MATRIZ_ORIGEN_Y+1]=COLOR_NEGRO;   /*grosor del eje = 3px*/
        matriz[x_pixel][MATRIZ_ORIGEN_Y-1]=COLOR_NEGRO;

        /*dibujar las divisiones horizontales*/
        for (y_real=MATRIZ_Y_MAX_REAL;y_real>=MATRIZ_Y_MIN_REAL;y_real--)
            matriz[x_pixel][coordenadas_reales_a_coordenadas_pixel_y(y_real)]=COLOR_NEGRO;

    }

    return;
}

int graficar_funcion(opt_t funcion,double amplitud,double periodo,double fase,double a,double b,double c,double d,char matriz[][MATRIZ_ALTO_PX]){

    int x_pixel,y_pixel;    /*identifican coordenadas de la pantalla*/
    int grado_polinomio;
    double x_real,y_real;   /*identifican coordenadas de la función matemática*/

    /* hacer ajustes iniciales en las variables*/
    switch (funcion) {
        case OPC_COSENO:
            fase+=NUMBER_PI/2;  /*al coseno desfasarlo pi/2 y tomarlo como seno*/
        case OPC_SENO:
            if (periodo<=0) return EXIT_FAILURE;    /*esto igualmente ya había sido verificado en la función de input */
            break;
        case OPC_POLI_G0:
            grado_polinomio=0;
            b=0;
            c=0;
            d=0;
            break;
        case OPC_POLI_G1:
            grado_polinomio=1;
            c=0;
            d=0;
            break;
        case OPC_POLI_G2:
            grado_polinomio=2;
            d=0;
            break;
        case OPC_POLI_G3:
            grado_polinomio=3;
            break;
        default:
            break;
    }

    for (x_pixel=0;x_pixel<MATRIZ_ANCHO_PX;x_pixel++) {  /*hacer un barrido pixel por pixel en x*/

        /*transformar coord. X del pixel a coord. X matemáticas (reales)*/
        x_real=coordenadas_pixel_a_coordenadas_reales_x(x_pixel);

        y_real=0;   /* inicializar Y, para poder acumular en los polinomios */
        switch (funcion) {  /*calcular y=f(x)*/

            case OPC_SENO:
            case OPC_COSENO:    /*la fase para coseno fue previamente desfasada por pi/2 */
                y_real=amplitud*sin((x_real*(1/periodo))+fase);
                break;

            case OPC_POLI_G3:
                y_real=d*pow(x_real,grado_polinomio-3);
            case OPC_POLI_G2:
                y_real+=c*pow(x_real,grado_polinomio-2);
            case OPC_POLI_G1:
                y_real+=b*pow(x_real,grado_polinomio-1);
            case OPC_POLI_G0:
                y_real+=a*pow(x_real,grado_polinomio);
                break;

            case OPC_LOGARITMO:
                y_real=log( (a*x_real) +b) +c;
                break;
            case OPC_EXPONENCIAL:
                y_real=pow(NUMBER_E , ( (a*x_real) +b )) +c;
            default:
                break;
        }

        /*transformar coord. del Y resultante a coord. de la pantalla.*/
        y_pixel=coordenadas_reales_a_coordenadas_pixel_y(y_real);

        /*verificar que el pixel a dibujar estaría dentro de los límites de la matriz*/
        if ( ( y_pixel < MATRIZ_ALTO_PX ) && ( y_pixel >=0 ) )
            matriz[x_pixel][y_pixel]=COLOR_NEGRO;   /*colorear el píxel de negro */

    }

    return EXIT_SUCCESS;
}

int coordenadas_reales_a_coordenadas_pixel_y(double y_real){

    return y_real*(-(MATRIZ_ALTO_PX)/(double)MATRIZ_ALTO_REAL)+MATRIZ_ORIGEN_Y;
    /*además se le cambia el signo, porque en mapa de bits, el eje y es positivo hacia abajo.*/
}

int coordenadas_reales_a_coordenadas_pixel_x(double x_real) {

    return x_real*(MATRIZ_ANCHO_PX/(double)MATRIZ_ANCHO_REAL)+MATRIZ_ORIGEN_X;
}

double coordenadas_pixel_a_coordenadas_reales_x(int x_pixel){
    static bool_t primera_vez_que_se_llama_la_funcion=TRUE;
    static double valores_precalculados[MATRIZ_ANCHO_PX];
    int i;

    if (primera_vez_que_se_llama_la_funcion==TRUE ){
        for (i=0;i<MATRIZ_ANCHO_PX;i++)                /*calcular las imágenes de la función una sola vez*/
            valores_precalculados[i]=i*((double)MATRIZ_ANCHO_REAL/MATRIZ_ANCHO_PX)+MATRIZ_ORIGEN_X;

        primera_vez_que_se_llama_la_funcion=FALSE;
    }

    return valores_precalculados[x_pixel];


}

error_t exportar_grafico_pbm(char matriz[][MATRIZ_ALTO_PX],char *nombre_archivo){

    FILE *archivo=NULL;
    int x_pixel,y_pixel;

    archivo=fopen(nombre_archivo,"w");
    if (archivo==NULL) return ERROR_ARCHIVOS_IO;

    fputs(PBM_HEADER,archivo);                                 /* "P1\n" */
    fprintf(archivo,"%d %d\n",MATRIZ_ANCHO_PX,MATRIZ_ALTO_PX);/*tamaño*/
    for (y_pixel=0;y_pixel<MATRIZ_ALTO_PX;y_pixel++){           /*barrido y*/
        for (x_pixel=0;x_pixel<MATRIZ_ANCHO_PX;x_pixel++)       /*barrido x*/
            fprintf(archivo,"%d ",matriz[x_pixel][y_pixel]);     /*imprimir pixel*/

        fprintf(archivo,"\n");           /*agregar salto de línea al final de cada fila.*/
    }

    fclose(archivo);
    return NO_ERROR;
}

void imprimir_menu_graph(void) {


        printf(TEXT_MENU_GRAPH);

        return;

}
