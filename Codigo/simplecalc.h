#ifndef SIMPLECALC_H_INCLUDED
#define SIMPLECALC_H_INCLUDED

#include "tipos_definidos.h"
#include "graficador.h"

bool_t simplecalc(char[FILENAME_MAX]);
double suma(double,double);
double resta(double,double);
double division(double,double);
double producto(double,double);
double potencia(double,int);
long factorial(long);
long num_combinatorio (long,long);
double num_aleatorio (double,double);
double drand();

#endif
