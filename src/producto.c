#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/producto.h"
#include "../include/manipularArchivos.h"


// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2

/**
 * Nombre: crearNodoProducto
 * 
 * Descripcion: Funcion que crea un nuevo nodo de un elemento, este nodo contendra los valores que define el struct que lo representa.
 * 
 * Entradas: const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad: Datos del struct que lo representa.
 * 
 * Salidas:  NodoProducto: Devuelve el nodo creado.
 * 
 */
NodoProducto* crearNodoProducto(const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad) {

    NodoProducto* nuevoNodo = (NodoProducto*)malloc(sizeof(NodoProducto));

    if (nuevoNodo == NULL) {
        printf("Error a asignar memoria al nodo.");
        exit(1);
    }

    // Asignar memoria para las cadenas de caracteres.
    nuevoNodo->producto.IdProducto = (char *)malloc((strlen(idProducto) + 1) * sizeof(char));
    nuevoNodo->producto.Nombre = (char *)malloc((strlen(nombre) + 1) * sizeof(char));
    nuevoNodo->producto.IdFamilia = (char *)malloc((strlen(idFamilia) + 1) * sizeof(char));

    // Validar si se asigno de forma correcta la memoria...
    if (nuevoNodo->producto.IdProducto == NULL || nuevoNodo->producto.Nombre == NULL || nuevoNodo->producto.IdFamilia == NULL) {
        printf("Error al asignar memoria para las cadenas.\n");
        free(nuevoNodo);
        exit(1);
    }

    // Seccion para copiar las cadenas de caracteres y guardar los demas datos del nuevo dato.
    // >> copiar y guardar las cadenas de caracteres.
    strcpy(nuevoNodo->producto.IdProducto, idProducto);
    strcpy(nuevoNodo->producto.Nombre, nombre);
    strcpy(nuevoNodo->producto.IdFamilia, idFamilia);
    
    // >> Guardar los datos numericos.
    nuevoNodo->producto.Costo = costo;
    nuevoNodo->producto.Precio = precio;
    nuevoNodo->producto.Cantidad = cantidad;

    // Se retorna el nodo recien creado.
    nuevoNodo->siguiente = NULL; //cambio realizado
    return nuevoNodo;
}


/**
 * Nombre: insertarelementoAlInicioProducto
 * 
 * Descripcion: Funcion para insertar un nuevo nodo al inicio de la lista de nodos, tambien llama a la funcion encargada de crear el nodo.
 * 
 * Entradas: NodoProducto** head: Puntero a la lista de nodos.
 * const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad: Datos del nodo.
 * 
 * Salidas: No posee.
 * 
 */
void insertarelementoAlInicioProducto(NodoProducto** head, const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad) {
    NodoProducto* nuevoNodo = crearNodoProducto(idProducto, nombre, idFamilia, costo, precio, cantidad);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}


/**
 * Nombre: insertarElementoAlFinalProducto
 * 
 * Descripcion: Funcion para insertar un nuevo nodo al final de la lista de nodos, tambien llama a la funcion encargada de crear el nodo.
 * 
 * Entradas: NodoProducto** head: Puntero a la lista de nodos.
 * const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad: Datos del nodo.
 * Salidas: No posee.
 * 
 */
void insertarElementoAlFinalProducto(NodoProducto** head, const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad) {

    NodoProducto * nuevoNddo = crearNodoProducto(idProducto, nombre, idFamilia, costo, precio, cantidad);
    if (*head == NULL) {
        *head = nuevoNddo;
        return;
    }

    NodoProducto* actual = *head;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevoNddo;
    return;
}


/**
 * Nombre: eliminarPorIdProducto
 * 
 * Descripcion: Funcion para eliminar un elementos especifico de la lista de nodos.
 * 
 * Entradas:  NodoProducto** head: Puntero a la lista de nodos.
 *  char * idProducto: Id del prodcuto a eliminar.
 * 
 * Salidas: No posee.
 * 
 */
void eliminarPorIdProducto(NodoProducto** head, char * idProducto) {

    NodoProducto* actual = *head;
    NodoProducto* anterior = NULL;

    while (actual != NULL && strcmp(actual->producto.IdProducto, idProducto) != 0)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("La familia con el indice %s no ha sido encontrada.", idProducto);
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
 * Nombre: buscarPorIdProducto
 * 
 * Descripcion: Funcion para buscar la exitencia de un elementos en la lista de nodos, retorna un entero indicando la exitencia del producto en la lista.
 * 
 * Entradas: NodoProducto* head: Puntero a la lista de nodos.
 *  const char* idProducto: Id del producto a buscar.
 * 
 * Salidas: Un entero que representa la exitencia del elemento buscado, 0 (No existe), 1 (Existe).
 * 
 */
int buscarPorIdProducto(NodoProducto* head, const char* idProducto) {
    NodoProducto* actual = head;

    while (actual != NULL)
    {
        if (strcmp(actual->producto.IdProducto, idProducto) == 0) {
            return 1;
        }
        actual = actual->siguiente;
    }
    return 0;
}


/**
 * Nombre: imprimirListaNodosProducto
 * 
 * Descripcion: Funcion para imprimir los datos de una lista de nodos.
 * 
 * Entradas: NodoProducto* head: Puntero a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
void imprimirListaNodosProducto(NodoProducto* head) {
    NodoProducto *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:\n");
        printf("Id: %s \nNombre: %s \nIdFamilia: %s \nCosto: %f \nPrecio: %f \nCantidad: %i \n\n", actual->producto.IdProducto, actual->producto.Nombre, actual->producto.IdFamilia, actual->producto.Costo, actual->producto.Precio, actual->producto.Cantidad);
        actual = actual->siguiente;
    }
    return;
}


/**
 * Nombre: liberarListaProducto
 * 
 * Descripcion: Funcion que se encarga de liberar la memoria asignada a los nodos y datos de la lista.
 * 
 * Entradas: NodoProducto* head: Puntero a la lista de nodos.
 * 
 * Salidas: No posee.
 * 
 */
void liberarListaProducto(NodoProducto* head) {
    NodoProducto *actual;
    while (head != NULL)
    {
        actual = head;
        head = head->siguiente;

        free(actual->producto.IdProducto);
        free(actual->producto.Nombre);
        free(actual->producto.IdFamilia);
        free(actual);
    }
    return;
}


// ==============Este seria el apartado para cargar los datos de una familia des un archivo y su guardado en la base de datos.

/**
 * Nombre: cargarProductosDesdeArchivo
 * 
 * Descripcion: Esta funcion se encarga de cargar los datos desde un archivo, validarlo y posteriormente los alamacena en una lista de punteros de nodos que almacenan la 
 * informacion del archivo.
 * 
 * Entradas: char * nombreArchivo: Nombre del archivo del que se estan leyendo lo datos.
 *  NodoProducto** listaDeFamilias: Lista de punteros para la lista de nodos en la que se estan guardando los datos.
 * 
 * Salidas: El estado de la carga de datos.
 * 
 */
int cargarProductosDesdeArchivo(char * nombreArchivo, NodoProducto** listaDeFamilias) {
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

        int num_palabras;
        char** lista_palabras = separar_cadenas(linea, &num_palabras);
        
        //printf("Elementos de la lista: %s ---- %s", lista_palabras[0], lista_palabras[1]);

        float costo = atof(lista_palabras[3]);
        float precio = atof(lista_palabras[4]);
        int cantidad = atoi(lista_palabras[5]);


        insertarElementoAlFinalProducto(listaDeFamilias, lista_palabras[0], lista_palabras[1], lista_palabras[2], costo, precio, cantidad);

        //imprimir_lista_palabras(lista_palabras, num_palabras, num_linea);

        // char* idProducto = lista_palabras[0];
        // char* nombreProducto = lista_palabras[1];
        // char* costoEnTexto = lista_palabras[2];
        // char* precioEnTexto = lista_palabras[3];
        // char* cantidadEnTexto = lista_palabras[4];
        
        // float costo = strtof(costoEnTexto, NULL);   // Convertir costo a float
        // float precio = strtof(precioEnTexto, NULL); // Convertir precio a float
        // int cantidad = atoi(cantidadEnTexto);       // Convertir cantidad a int

        for (int i = 0; i < num_palabras; i++) {
            free(lista_palabras[i]);
        }
        free(lista_palabras);


        free(linea);
    }
    

    fclose(archivo);
    return 0;
}


/**
 * Nombre:agregarProductos
 * 
 * Descripcion: Es la función que realiza toda la inserción de los datos a Productos
 * 
 * Funcionamiento: Esta función recibe todos los datos necesarios para poder agregar todos los productos que se le envien a la base
 de datos, de forma que se agrega constantemente y sin generar fallos
 * 
 * Entradas: MYSQL *conexion: un puntero a un tipo de datos mysql, un char constante de id_producto, nombre, id_familia, float en costo, precio 
 y un entero para su precio
 * 
 * Salidas: No tiene
 * 
 */
void agregarProductos(MYSQL *conexion, const char *id_producto, const char *nombre, const char *id_familia, float costo, float precio, int cantidad) {
    // Verificamos que nada sea nulo
    printf("soy idProducto: %s\n",id_familia);
    if (id_producto == NULL || nombre == NULL || id_familia == NULL) {
        printf("Error: Ningún campo puede ser nulo\n");
        return;
    }

    //asprintf me da la memoria necesaria para la consulta
    char *consulta = NULL;
    int largoConsulta = asprintf(&consulta, "INSERT INTO Productos(IdProducto, Nombre, IdFamilia, Costo, Precio, Cantidad) VALUES ('%s', '%s', '%s', '%f', '%f', '%d');",
             id_producto, nombre, id_familia, costo, precio, cantidad);


    // Ejecutamos la consulta
    if (mysql_query(conexion, consulta)) {
        printf("%s\n", mysql_error(conexion)); //Esto lo podemos quitar
        printf("No se agrego el %s pues no puede repetir identificadores de productos que ya existen\n",id_producto);
        free(consulta);
        return;
    }

    printf("Producto agregado correctamente.\n");
    free(consulta);
    return;
}


/**
 * Nombre:guardarProductosEnDB
 * 
 * Descripcion: Guarda productos en la base de datos
 * 
 * Funcionamiento: En si se puede ver como un puente pues recibe los datos pasados por el usuario en variables las cuales
 luego se van a pasar a una funcion donde realmente sí se agreguen para despues nada mas volver y terminar su ejecucion
 * 
 * Entradas: un puntero a un dato MYSQL, un puntero a un struct
 * 
 * Salidas: No tiene
 * 
 */
void guardarProductosEnDB(MYSQL *conexion, NodoProducto* head) {

    NodoProducto *actual = head;
    while (actual != NULL)
    {


        char *id_producto;
        char *nombre;
        char *id_familia;

        id_familia = (char *)malloc((strlen(actual->producto.IdProducto) + 1) * sizeof(char));
        nombre = (char *)malloc((strlen(actual->producto.Nombre) + 1) * sizeof(char));
        id_producto = (char *)malloc((strlen(actual->producto.IdFamilia) + 1) * sizeof(char));


        if (id_familia == NULL || nombre == NULL || id_producto == NULL) {
            printf("Error al asignar memoria para las cadenas.\n");
            //free(nuevoNodo); // Liberar la memoria del nodo si falla
            free(id_producto);
            free(nombre);
            free(id_familia);
            exit(1);
        }

        strcpy(id_producto, actual->producto.IdProducto);
        strcpy(nombre, actual->producto.Nombre);   
        strcpy(id_familia, actual->producto.IdFamilia);    

        float costo = actual->producto.Costo;
        float precio = actual->producto.Precio;
        int cantidad = actual->producto.Cantidad;

        agregarProductos(conexion,id_producto,nombre,id_familia,costo,precio,cantidad);

        // Aqui iria el resto de la logica para guardar en la base de datos.
        free(id_producto);
        free(nombre);
        free(id_familia);

        actual = actual->siguiente;

    }
    return;
}


/**
 * Nombre: validar_exitencia_producto
 * 
 * Descripcion: Funcion para validar la existencia del registro de un producto de la base de datos, esto se hace midiendo la cantidad de elementos que retona la consulta, si es 0 
 * significa que el prodcuto no exite, en caso de ser diferente de cero, significa que existe.
 * 
 * Entradas: 
 *  MYSQL *conexion: Puntero a la instancia de MYSQL
 *  const char *idProducto: Id del prodcuto.
 * 
 * Salidas: int count: la cantidad de elmentos registrados.
 * 
 */
int validar_exitencia_producto(MYSQL *conexion, const char *idProducto) {

    MYSQL_RES *resultado;
    MYSQL_ROW fila;
    char consulta[256];

    // Paso 1: Verificar si el producto existe usando el procedimiento almacenado
    snprintf(consulta, sizeof(consulta), "CALL obtenerProductoPorID('%s')", idProducto);

    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return 1;
    }

    resultado = mysql_store_result(conexion);
    if (resultado == NULL) {
        printf("Error al obtener los resultados: %s\n", mysql_error(conexion));
        return 1;
    }

    int count = 0;

    fila = mysql_fetch_row(resultado); 

    if (fila == NULL) {
        //printf("Error: El producto con ID '%s' no existe en la base de datos.\n", idProducto);
        count++;
    }
    
    do {
        resultado = mysql_store_result(conexion);
        if (resultado) {
            
                // Recorrer los resultados e imprimir cada fila
                while ((fila = mysql_fetch_row(resultado)) != NULL) {
                    if (fila == NULL) {
                        count++;
                    }
                }
            mysql_free_result(resultado); // Liberar resultados del conjunto actual
        }
    } while (mysql_next_result(conexion) == 0); // Procesar los siguientes resultados, si existen

    if (count != 0) {
        printf("Error: El producto con ID '%s' no existe en la base de datos.\n", idProducto);
    }

    return count;

}


/**
 * Nombre: eliminarProducto
 * 
 * Descripcion: Funcion para la eliminacion de un producto mediante el id, se contruye una consulta mediante un DELETE para eliminar el prodcuto..
 * 
 * Entradas: MYSQL *conexion: Puntero a la instancia de MYSQL
 *  const char *idProducto: ID del prodcuto a eliminar.
 * 
 * Salidas: No posee.
 * 
 */
void eliminarProducto(MYSQL *conexion, const char *idProducto) {

    // Validamos que el producto exista.
    int existe_producto = validar_exitencia_producto(conexion, idProducto);
    if (existe_producto != 0) {
        printf("\nPor favo intentelo nuevamente...\n");
        return;
    }

    char consulta[128];
    int resultado;

    // Crear la consulta SQL para eliminar el producto
    snprintf(consulta, sizeof(consulta), "DELETE FROM Productos WHERE IdProducto = '%s'", idProducto);

    // Ejecutar la consulta
    resultado = mysql_query(conexion, consulta);

    if (resultado) {
        // Si ocurre un error, mostrar el mensaje proporcionado por el trigger o MySQL
        printf("Error al eliminar el producto con ID '%s', no e pueden eliminar producto que tengan una cotizacion Pendiente.\n", idProducto);
    } else {
        printf("Producto con ID '%s' eliminado correctamente.\n", idProducto);
    }

    return;

}





