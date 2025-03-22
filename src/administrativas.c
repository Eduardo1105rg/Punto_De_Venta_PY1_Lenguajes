#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Include de archivos del programa.
#include "../include/administrativas.h"
#include "../include/familia.h"
#include "../include/db.h"
#include "../include/manipularArchivos.h"



void incluir_familia_de_productos() {

    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        return; 
    }

    // Lista de familias.
    NodoFamilia *listaDeFamilias = NULL;

    // Solicitar la ruta y el nombre del archivo que se quiere usar.
    char *rutasArchivo;
    printf("\nIngresa la ruta y el nombre del archivo a usar, solo se pueden usar archivos .txt (E.J: data/familias.txt): ");
    leerCaracteresDeFormadinamica(&rutasArchivo);
    printf("\n");

    // Aqui se harian validaciones para lo del archivo.

    // Primero se cargan los datos desde el archivo y se almacenan en la lista.
    cargarFamiliasDesdeArchivo(rutasArchivo, &listaDeFamilias);

    //Imprimimos los datos guardados en la lista.
    imprimirListaNodosFamilia(listaDeFamilias);
  
    // Los intentamos almacenar en la base de datos.
    guardarFamiliasEnDB(conexion,listaDeFamilias);

    // Liberamos la memoria asignada a la lista.
    liberarListaFamilia(listaDeFamilias);

    return;
}




void menu_principal_administrativos() {

    char opcion;
    do {
        printf("\nBienvenido al apartado de Opciones Administrativas. \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Registro de familias de productos. \n");
        printf(">> B) Registro de productos.\n");
        printf(">> C) Cargar inventario.\n");
        printf(">> D) Consulta de facturas.\n");
        printf(">> E) Estadísticas .\n");

        printf(">> S) Volver al menu principal.\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        scanf(" %c", &opcion); 
        getchar(); 

        switch (opcion) {
            // ========== Registro de familias de productos.
            case 'a' :
                incluir_familia_de_productos();
                break;
            case 'A':
                incluir_familia_de_productos();
                
                break;

            // ========== Registro de productos.
            case 'b':
                
                break;

            case 'B':
                break;

            // ========== Cargar inventario.
            case 'c' :
                
                break;
            case 'C':
                
                break;

            // ========== Consulta de facturas.
            case 'd':
                
                break;

            case 'D':
                break;
                
             // ========== Estadísticas.
             case 'e' :
                
             break;

            case 'E':
                
                break;

             
            // ======== Salir de la seccion:
            case 's':
                printf("Saliendo de la seccion de opciones administrativas...\n");
                break;

            case 'S':
                printf("Saliendo de la seccion de opciones administrativas...\n");
                break;


            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    return;
}