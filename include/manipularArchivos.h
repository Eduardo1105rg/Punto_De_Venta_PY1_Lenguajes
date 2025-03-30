#ifndef MANIPULAR_ARCHIVOS_H
#define MANIPULAR_ARCHIVOS_H

#include <stdio.h>

char** separar_cadenas(char* cadena, int* num_palabras);

char* leer_lineas(FILE *archivo, int num_linea);

void imprimir_lista_palabras(char** listaPalabras, int numPalabras, int num_linea);

void leerCaracteresDeFormadinamica(char** cadena);

int leerNumeroDinamico();

int leeNumeroDinamicoV2();

#endif