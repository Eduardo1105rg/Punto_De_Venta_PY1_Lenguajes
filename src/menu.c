#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <openssl/sha.h>

// Include de archivos del programa.

#include "../include/administrativas.h"
#include "../include/generales.h"
#include "../include/menu.h"  
#include "../include/db.h"  


/**
 * Nombre:menu_principal
 * 
 * Descripcion:Es el primer menu de la aplicación
 * 
 * Funcionamiento: En si nos da tres opciones el cual el usuario puede elegir y dependiendo de lo que seleccione
 se le mostraran otro tipo de menus
 * 
 * Entradas: No recibe
 * 
 * Salidas: No devuelve nada
 * 
 */
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
                if (verificacionAdministrativo() == 1){
                    menu_principal_administrativos();
                    break;
                }else {
                    break;
                }

            case 'A':
            if (verificacionAdministrativo() == 1){
                menu_principal_administrativos();
                break;
            }else {
                break;
            }

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


/**
 * Nombre:sha256
 * 
 * Descripcion:
 * 
 * Funcionamiento: Primero le damos un char sin asignar que realmente es una lista con el largo de los bits a asignar
 Luego llamamos a la función SHA256 la cual calcula la clave del hash a este le pasamos igual char sin asignar, la
 contraseña y el hash, donde una vez hecho esto calculamos de manera dinamica la cadena final para ir asignando valores
 hexadecimales y por ultimo asignarle un char de terminación.
 * 
 * Entradas: const char *contra
 * 
 * Salidas: un puntero a la cadena
 * 
 */
//Paginas de ayuda para crear el codigo https://sha256hash.org/es/c-sha256/ 
// https://stackoverflow.com/questions/22880627/sha256-implementation-in-c
char* EncriptacionSha256(const char *contra) {
    unsigned char hash[SHA256_DIGEST_LENGTH]; // Aqui le damos un char sin asignar de 256 bits 
                                        //Suficiente para un SHA que usa 32 bytes
    SHA256((unsigned char *)contra, strlen(contra), hash);
    //Llamamos a la funcion SHA256, le enviamos un puntero a datos binarios
    // el tamaño de la contraseña que queremos encriptar
    //Por ultimo el arreglo donde se guarda el resultado

    // Asignamos memoria dinamica para el hash en formato hexadecimal
    char *cadenaFinal = (char*)malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (cadenaFinal == NULL) {
        printf("Error al asignar memoria\n");
        return NULL;
    }

    //Por cada byte lo haremos representado en hexadecimal
    //y lo haremos paso a paso
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(cadenaFinal + (i * 2), "%02x", hash[i]);
    }
//Lo que hacemos es al ultimo byte darle un caracter de terminacion
    cadenaFinal[SHA256_DIGEST_LENGTH * 2] = '\0'; 
    return cadenaFinal; // Devolver el puntero a la cadena
}


/**
 * Nombre:verificacionAdministrativo
 * 
 * Descripcion:
 * 
 * Funcionamiento: Lo que hace es pedirle el usuario y la contraseña verifica que sea igual al de la base de datos
 si este resulta ser distinto no le concede el paso al otro menu
 * 
 * Entradas: No recibe
 * 
 * Salidas: Un 1 si resulto exitoso sino un 0
 * 
 */


int verificacionAdministrativo() {
    char nombre[50];
    char contrasena[65];
    printf("Antes de ingresar a este menu debe loguearse como usuario\n");
    printf("Primero escriba el nombre de usuario\n");
    scanf("%s", nombre);
    printf("Por ultimo escriba su contraseña\n");
    scanf("%s", contrasena);

    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        return 0;
    }
    printf("Se logró conectar\n");

    char *consulta = NULL;
    int largoCon = asprintf(&consulta, "select NombreUsuario, ContraseñaUsuario from Usuario");
    if (mysql_query(conexion, consulta)) {
        printf("%s\n", mysql_error(conexion));
        free(consulta);
        return 0;
    }
    free(consulta); 

    MYSQL_RES *resultado4 = mysql_store_result(conexion);
    MYSQL_ROW fila;
    while ((fila = mysql_fetch_row(resultado4)) != NULL) {

        //Realizamos la comparación de strings
        if (strcmp(nombre, fila[0]) == 0) {
            //Almacenamos el resultado de manera local
            char *contraHash = EncriptacionSha256(contrasena);
            if (contraHash != NULL) {
                //Si las contraseñas encriptadas coinciden liberamos y retornamos 1
                if (strcmp(contraHash, fila[1]) == 0) {
                    free(contraHash); 
                    mysql_free_result(resultado4); 
                    return 1; 
                }
                free(contraHash); 
            }
        }
    }

    mysql_free_result(resultado4);
    return 0;
}