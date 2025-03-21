#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include de archivos del programa.

#include "../include/administrativas.h"
#include "../include/generales.h"


void menu_principal() {

    char opcion;
    do {
        printf("\nBienvenido al menu principal del sistema Punto de ventan:\n");

        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Opciones administrativas: \n");
        printf(">> B) Opciones generales:\n");
        printf(">> S) Salir\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Opciones administrativas.
            case 'a' :
                menu_principal_administrativos();
                break;
            case 'A':
                menu_principal_administrativos();
                break;

            // ========== Opciones generales.
            case 'b':
                menu_principal_generales();
                break;

            case 'B':
                menu_principal_generales();
                break;

            // ===== Salir de programa.
            case 's':
                printf("Saliendo del programa punto de venta...\n");
                break;
            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    return;
}