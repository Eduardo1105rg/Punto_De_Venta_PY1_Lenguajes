/**
 * 
 * Nombre: Manipular-Archivos
 * 
 * Descipcion: Se requiere del desarrollo de un programa que lea los datos de un archivo txt
 *  e imprima las palabras que conformar cada linea del archivo.
 * 
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * 
 * Nombre: separar_cadenas
 * 
 * Descipcion: Esta funcion permite separar una cadena de texto en cada uno de los caracteres 
 * que la conforman, ademas.
 * 
 * Entradas: 
 *  > char* cadena:
 *  > int* num_palabras:
 * 
 * Salidas:
 *  > char **lista_palabras:
 * 
 */
char** separar_cadenas(char* cadena, int* num_palabras) {

    // Estas son para lo que seria la formacion de cada cadema de caracteres.
    int tamano_lista = 2;
    int tamano_plabra = 16;
    int indice_palabra = 0;
    int indice_lista = 0;

    // Lista de palabras
    char** lista_palabras = (char **)malloc(tamano_lista * sizeof(char*));
    if (lista_palabras == NULL) {
        printf("Error al asignar memoria para la lista de palabras.\n");
        exit(1);
    }

    // Palaba
    char *palabra = (char *)malloc(tamano_plabra * sizeof(char)); // Se le asigna memoria a la cadena.
    if (palabra == NULL) {
        printf("Error al asignar memoria para una palabra.\n");
        free(lista_palabras);
        exit(1);
    }

    while (*cadena != '\0') {

        if (*cadena == ',') {

            palabra[indice_palabra] = '\0';

            // Revisar eñ espacio de la lista de palbaras
            if (indice_lista >= tamano_lista) {
                tamano_lista *= 2;
                char** nueva_lista = (char **)realloc(lista_palabras, tamano_lista * sizeof(char*));
    
                if (nueva_lista == NULL) {
                    printf("Error al reasignar memoria para la lista de palabras.\n");
                    free(lista_palabras);
                    exit(1);
                }

                lista_palabras = nueva_lista;
            } 

            lista_palabras[indice_lista++] = palabra;

            tamano_plabra = 16;

            palabra = (char*)malloc(tamano_plabra * sizeof(char));   
            if (palabra == NULL) {
                printf("Error al asignar memoria a la palabra nueva\n");
                exit(1);
            }
            indice_palabra = 0;

        } else {
            palabra[indice_palabra++] = *cadena;
        
            if (indice_palabra >= tamano_plabra) {
                tamano_plabra *=2;
                char *nueva_palabra = (char *)realloc(palabra, tamano_plabra * sizeof(char));
                
                if (nueva_palabra == NULL) {
                    printf("Error al reasignar memoria a la palabra. \n");
                    exit(1);
                }
                palabra = nueva_palabra;
            }
        }
        cadena++; // Avanza al siguiente carácter
    }

    // Agregar la última palabra (si existe)
    if (indice_palabra > 0) {
        palabra[indice_palabra] = '\0';

        if (indice_palabra >= tamano_plabra) {
            tamano_lista *= 2;
            lista_palabras = (char**)realloc(lista_palabras, tamano_lista * sizeof(char*));
        }
        lista_palabras[indice_lista++] = palabra;
    } else {
        free(palabra);
        
    }
    *num_palabras = indice_lista;
    return lista_palabras;
}

/**
 * 
 * Nombre: imprimir_lista_palabras
 * 
 * Descipcion: Imprimir los elementos de una lista de palabras (caracteres)
 * 
 * Entradas: char** listaPalabras, int numPalabras, int num_linea
 * 
 * Salidas: No posee.
 * 
 */
void imprimir_lista_palabras(char** listaPalabras, int numPalabras, int num_linea) {
    printf("Palabras separadas:\n");
    for (int i = 0; i < numPalabras; i++) {
        printf("Linea %i: %s\n", num_linea, listaPalabras[i]);
    }
    return;
}

/**
 * 
 * Nombre: leer_lineas
 * 
 * Descipcion: Esta funcion permite leer de forma dinamica las lineas de un archivo .txt.
 * 
 * Entradas: FILE *archivo, int num_linea
 * 
 * Salidas: No posee
 * 
 */
char* leer_lineas(FILE *archivo, int num_linea) {

    // Para lo que seria separar los lineas del archivo.
    char *linea = NULL; // La linea actual de forma dinamica.
    int tamano = 1;
    int indice = 0; 
    char caracter; // Cada caracter de la linea.


    // Memoria incial...
    linea = (char *)malloc(tamano * sizeof(char));
    if (linea == NULL) {
        printf("Error al asignar memoria a la linea. \n");
        exit(1);
    }

    // Leer caracteres hasta llegar al salto de linea \n

    while ((caracter = fgetc(archivo)) != EOF && caracter != '\n') 
    {
        linea[indice++] = caracter; // Se alacena el caracter.

        // Revisar tamno.
        if (indice >= tamano) {
            tamano *= 2;
            char *nueva_linea = (char *)realloc(linea, tamano * sizeof(char));
            if (nueva_linea == NULL) {
                printf("Error al reasigar memoria a la linea.\n");
                exit(1);
            }
            linea = nueva_linea;
        }
    }
    linea[indice] = '\0';
    printf("Datos de la linea que se leyo: %s\n", linea);

    return linea;
}

