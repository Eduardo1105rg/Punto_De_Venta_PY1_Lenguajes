#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/familia.h"
#include "../include/manipularArchivos.h"

// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2

/**
 * Nombre: crearNodoFamilia
 * 
 * Descripcion: Funcion que crea un nuevo nodo de un elemento, este nodo contendra los valores que define el struct que lo representa.
 * 
 * Entradas: const char *id, const char *descripcion: Datos del struct que lo representa.
 * 
 * Salidas:  NodoFamilia: Devuelve el nodo creado.
 * 
 */
NodoFamilia* crearNodoFamilia(const char *id, const char *descripcion) {
    NodoFamilia* nuevoNodo = (NodoFamilia*)malloc(sizeof(NodoFamilia));

    if (nuevoNodo == NULL) {
        printf("Error a asignar memoria al nodo.");
        exit(1);
    }

    // Asignar memoria para IdFamilia y Descripcion
    nuevoNodo->familia.IdFamilia = (char *)malloc((strlen(id) + 1) * sizeof(char));
    nuevoNodo->familia.Descripcion = (char *)malloc((strlen(descripcion) + 1) * sizeof(char));

    if (nuevoNodo->familia.IdFamilia == NULL || nuevoNodo->familia.Descripcion == NULL) {
        printf("Error al asignar memoria para las cadenas.\n");
        free(nuevoNodo); // Liberar la memoria del nodo si falla
        exit(1);
    }

    // En esta parte de copiarian los datos y se le asignarian a lo de 
    // este guardado en el nodo.
    strcpy(nuevoNodo->familia.IdFamilia, id);
    strcpy(nuevoNodo->familia.Descripcion, descripcion);


    //newNodo->familia.IdFamilia[alFinalDelString] = '\0';
    nuevoNodo->siguiente = NULL; //cambio realizado
    return nuevoNodo;
}


/**
 * Nombre: insertarelementoAlInicioFamilia
 * 
 * Descripcion: Funcion para insertar un nuevo nodo al inicio de la lista de nodos, tambien llama a la funcion encargada de crear el nodo.
 * 
 * Entradas: NodoFamilia** head: Puntero a la lista de nodos.
 * const char *id, const char *descripcion: Datos del nodo.
 * 
 * Salidas: No posee.
 * 
 */
void insertarelementoAlInicioFamilia(NodoFamilia** head, const char *id, const char *descripcion) {
    NodoFamilia* nuevoNodo = crearNodoFamilia(id, descripcion);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}


/**
 * Nombre: insertarElementoAlFinalFamilia
 * 
 * Descripcion: Funcion para insertar un nuevo nodo al final de la lista de nodos, tambien llama a la funcion encargada de crear el nodo.
 * 
 * Entradas: NodoFamilia** head: Puntero a la lista de nodos.
 * const char *id, const char *descripcion: Datos del nodo.
 * Salidas: No posee.
 * 
 */
void insertarElementoAlFinalFamilia(NodoFamilia** head, const char *id, const char *descripcion) {

    //Cambio 
    if (head == NULL) {
        printf("Error: El puntero 'head' es NULL.\n");
        return;
    }

    NodoFamilia * nuevoNodo = crearNodoFamilia(id, descripcion);

    if (nuevoNodo == NULL) {
        printf("Error al crear un nuevo nodo.\n");
        return;
    }    

    if (*head == NULL) {
        *head = nuevoNodo;
        return;
    }

    NodoFamilia* actual = *head;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevoNodo;
    return;
}


/**
 * Nombre: eliminarPorIdFamilia
 * 
 * Descripcion: Funcion para eliminar un elementos especifico de la lista de nodos.
 * 
 * Entradas:  NodoFamilia** head: Puntero a la lista de nodos.
 *  char * idProducto: Id de la familia a eliminar a eliminar.
 * 
 * Salidas: No posee.
 * 
 */
void eliminarPorIdFamilia(NodoFamilia** head, char * id) {

    NodoFamilia* actual = *head;
    NodoFamilia* anterior = NULL;

    while (actual != NULL && strcmp(actual->familia.IdFamilia, id) != 0)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("La familia con el indice %s no ha sido encontrada.", id);
        return;
    }

    if (anterior == NULL)
    {
        *head = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    
    free(actual);
    return;
}


/**
 * Nombre: buscarPorIdFamilia
 * 
 * Descripcion: Funcion para buscar la exitencia de un elementos en la lista de nodos, retorna un entero indicando la exitencia del producto en la lista.
 * 
 * Entradas: NodoFamilia* head: Puntero a la lista de nodos.
 *  const char* idProducto: Id del producto a buscar.
 * 
 * Salidas: Un entero que representa la exitencia del elemento buscado, 0 (No existe), 1 (Existe).
 * 
 */
int buscarPorIdFamilia(NodoFamilia* head, const char* id) {
    NodoFamilia* actual = head;

    while (actual != NULL)
    {
        if (strcmp(actual->familia.IdFamilia, id) == 0) {
            return 1;
        }
        actual = actual->siguiente;
    }

    return 0;
}


/**
 * Nombre: imprimirListaNodosFamilia
 * 
 * Descripcion: Funcion para imprimir los datos de una lista de nodos.
 * 
 * Entradas: NodoFamilia* head: Puntero a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
void imprimirListaNodosFamilia(NodoFamilia* head) {
    NodoFamilia *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:\n");
        printf("Id: %s, \nDescripcion: %s \n\n", actual->familia.IdFamilia, actual->familia.Descripcion);
        actual = actual->siguiente;
    }
    return;
}


/**
 * Nombre: liberarListaFamilia
 * 
 * Descripcion: Funcion que se encarga de liberar la memoria asignada a los nodos y datos de la lista.
 * 
 * Entradas: NodoFamilia* head: Puntero a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
void liberarListaFamilia(NodoFamilia* head) {
    NodoFamilia *actual;
    while (head != NULL)
    {
        actual = head;
        head = head->siguiente;

        free(actual->familia.IdFamilia);
        free(actual->familia.Descripcion);
        free(actual);
    }
    return;
}


// ==============Este seria el apartado para cargar los datos de una familia des un archivo y su guardado en la base de datos.

/**
 * Nombre: cargarFamiliasDesdeArchivo
 * 
 * Descripcion: Esta funcion se encarga de cargar los datos desde un archivo, validarlo y posteriormente los alamacena en una lista de punteros de nodos que almacenan la 
 * informacion del archivo.
 * 
 * Entradas: char * nombreArchivo: Nombre del archivo del que se estan leyendo lo datos.
 *  NodoFamilia** listaDeFamilias: Lista de punteros para la lista de nodos en la que se estan guardando los datos.
 * 
 * Salidas: El estado de la carga de datos.
 * 
 */
int cargarFamiliasDesdeArchivo(char * nombreArchivo, NodoFamilia** listaDeFamilias) {
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    int num_linea = 0;

    while (!feof(archivo))
    {
        num_linea++;
        char *linea = leer_lineas(archivo, num_linea);
        if (linea == NULL) {
            printf("Error: leer_lineas devolvio NULL en linea %d\n", num_linea);
            continue;
        }

        int num_palabras;
        char** lista_palabras = separar_cadenas(linea, &num_palabras);
        if (lista_palabras == NULL) {
            printf("Error: `separar_cadenas` devolvio NULL en la linea %d\n", num_linea);
            free(linea);
            continue;
        }
        //printf("Elementos de la lista: %s ---- %s", lista_palabras[0], lista_palabras[1]);

        insertarElementoAlFinalFamilia(listaDeFamilias, lista_palabras[0], lista_palabras[1]);

        //imprimir_lista_palabras(lista_palabras, num_palabras, num_linea);

        if (num_palabras < 2 || lista_palabras[0] == NULL || lista_palabras[1] == NULL) {
            printf("Error en la linea %d: No hay suficientes elementos\n", num_linea);
            free(linea);
            continue;
        }

        for (int i = 0; i < num_palabras; i++) {
            if (lista_palabras[i] == NULL) {
                printf("Advertencia: `lista_palabras[%d]` es NULL, no se liberará\n", i);
            } else {
                //printf("Liberando memoria de: %s\n", lista_palabras[i]);
                free(lista_palabras[i]);
            }
        }
        free(lista_palabras);
        free(linea);
    }
    

    fclose(archivo);
    return 0;
}


/**
 * Nombre:agregarFamilias
 * 
 * Descripcion: Agrega familias a la base de datos
 * 
 * Funcionamiento: En si recibe ciertos identificadores necesarios con los cuales podra realizar inserciones en la
 tabla de FamiliaProductos enviando así los datos correctamente y haciendo que estos se vean reflejados en la base de datos
 * 
 * Entradas: Una conexion MYSQL, un puntero de tipo char constante para el id y la descripcion de la familia
 * 
 * Salidas: Es vacia
 * 
 */
void agregarFamilias(MYSQL *conexion, const char *idFamilia, const char *descripcion) {
    // Verificamos que nada sea nulo
    if (idFamilia == NULL || descripcion == NULL) {
        printf("Error: idFamilia o descripcion no pueden ser NULL.\n");
        return;
    }

    //Esto es para ver lo que tenia jsjsj
    printf("IdFamilia: %s\n", idFamilia);
    printf("Descripcion: %s\n", descripcion);
    int textoFijo = strlen("INSERT INTO FamiliaProductos(IdFamilia, Descripcion) VALUES ('', '');");
    int consultaTamano = strlen(idFamilia) + strlen(descripcion) + textoFijo; 
    char* consulta = malloc((consultaTamano + 1) * sizeof(char)); //Hacemos el calculo de la memoria

    if (consulta == NULL) {
        printf("Error: No se pudo asignar memoria para la consulta.\n");
        return;
    }

    // aqui lo que hacemos es primero guardar el string en consulta, con el tamaño calculado anteriormente, y lo que hago
    //es agregarle los valores que necesita la consulta
    snprintf(consulta, consultaTamano, "INSERT INTO FamiliaProductos(IdFamilia, Descripcion) VALUES ('%s', '%s');", idFamilia, descripcion);


    if (mysql_query(conexion, consulta)) {
        printf("%s\n", mysql_error(conexion));
        printf("No se pudo agregar %s pues esta familia ya se encuentra registrada\n", idFamilia);
        free(consulta);
        return;
    }

    printf("Producto agregado correctamente.\n");
    free(consulta);
    return;
}

/**
 * Nombre:guardarFamiliasEnDB
 * 
 * Descripcion: En si se puede ver como la funcion principal de guardar las familias en la base de datos
 * 
 * Funcionamiento: Recibe datos como la conexion MYSQL y el nodoFamilia la cual contiene los datos que el usuario agrego
 en la aplicación de forma que se extraen para luego pasarlos a una función que modifique realmente agregue esto en la base
 de datos.
 * 
 * Entradas: Un puntero a un tipo de dato MYSQL, un puntero a un struct 
 * 
 * Salidas: No tiene
 * 
 */
void guardarFamiliasEnDB(MYSQL *conexion, NodoFamilia* head) {

    NodoFamilia *actual = head;
    while (actual != NULL)
    {

        char *id_familia;
        char *descripcion;

        id_familia = (char *)malloc((strlen(actual->familia.IdFamilia) + 1) * sizeof(char));
        descripcion = (char *)malloc((strlen(actual->familia.Descripcion) + 1) * sizeof(char));

        if (id_familia == NULL || descripcion == NULL) {
            printf("Error al asignar memoria para las cadenas.\n");
            //free(nuevoNodo); // Liberar la memoria del nodo si falla
            free(id_familia);
            free(descripcion);
            exit(1);
        }
        strcpy(id_familia, actual->familia.IdFamilia);
        strcpy(descripcion, actual->familia.Descripcion);    
        //actual->familia.IdFamilia
        //actual->familia.Descripcion

        agregarFamilias(conexion, id_familia, descripcion);

        free(id_familia);
        free(descripcion);

        actual = actual->siguiente;

    }
    return;
}


