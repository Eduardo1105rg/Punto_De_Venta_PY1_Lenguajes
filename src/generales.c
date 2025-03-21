#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include de archivos del programa.
#include "../include/generales.h"


void menu_principal_generales() {

    char opcion;
    do {
        printf("\nBienvenido al apartado de Opciones generales. \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Consulta de catalogo. \n");
        printf(">> B) Cotizar.\n");
        printf(">> C) Modificar cotización.\n");
        printf(">> D) Facturar.\n");
        printf(">> S) Volver al menu principal.\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
                
                break;
            case 'A':
                
                break;

            // ========== Cotizar..
            case 'b':
                
                break;

            case 'B':
                break;

            // ========== Modificar cotización.
            case 'c' :
                
                break;
            case 'C':
                
                break;

            // ========== Facturar.
            case 'd':
                
                break;

            case 'D':
                break;      

            // ========== Salir del menu.
            case 's':
                printf("Saliendo de la seccion de opciones generales...\n");
                break;
            case 'S':
                printf("Saliendo de la seccion de opciones generales...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    return;
}