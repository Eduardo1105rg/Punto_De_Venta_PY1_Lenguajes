#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/producto.h"
#include "../include/manipularArchivos.h"


// ================ Funciones para lo que seria la creacion de la lista de familias.


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2
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

void insertarelementoAlInicioProducto(NodoProducto** head, const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad) {
    NodoProducto* nuevoNodo = crearNodoProducto(idProducto, nombre, idFamilia, costo, precio, cantidad);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}

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
int cargarProductosDesdeArchivo(char * nombreArchivo, NodoProducto** listaDeFamilias) {
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return -1;
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



void eliminarProducto(MYSQL *conexion, const char *idProducto) {


    return;
}