#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include "../include/cotizacion.h"
#include "../include/producto.h"
// int cantidadCotizaciones = 1;
const char *estadoCotizacion = "Pago pendiente";


// Video el cual fue usado como guia para crear las listas, creditos al autor: https://www.youtube.com/watch?v=3-u5SRuStuc&t=966s&ab_channel=DIF%E2%84%A2
NodoCotizacionDetalle* crearNodoCotizacionDetalle(const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad) {
    NodoCotizacionDetalle* nuevoNodo = (NodoCotizacionDetalle*)malloc(sizeof(NodoCotizacionDetalle));

    if (nuevoNodo == NULL) {
        printf("Error a asignar memoria al nodo.");
        exit(1);
    }

    // Asignar memoria para IdFamilia y Descripcion
    nuevoNodo->detallesCotizacion.IdProducto = (char *)malloc((strlen(id) + 1) * sizeof(char));
    nuevoNodo->detallesCotizacion.NombreProducto = (char *)malloc((strlen(nombreProducto) + 1) * sizeof(char));
    nuevoNodo->detallesCotizacion.Descripcion = (char *)malloc((strlen(descripcion) + 1) * sizeof(char));

    if (nuevoNodo->detallesCotizacion.IdProducto == NULL || nuevoNodo->detallesCotizacion.NombreProducto == NULL || nuevoNodo->detallesCotizacion.Descripcion == NULL) {
        printf("Error al asignar memoria para las cadenas.\n");
        free(nuevoNodo); // Liberar la memoria del nodo si falla
        exit(1);
    }

    // En esta parte de copiarian los datos y se le asignarian a lo de 
    // este guardado en el nodo.
    strcpy(nuevoNodo->detallesCotizacion.IdProducto, id);
    strcpy(nuevoNodo->detallesCotizacion.NombreProducto, nombreProducto);
    strcpy(nuevoNodo->detallesCotizacion.Descripcion, descripcion);
    nuevoNodo->detallesCotizacion.precio = precio;
    nuevoNodo->detallesCotizacion.cantidad = cantidad;


    nuevoNodo->siguiente = NULL; //cambio realizado
    return nuevoNodo;
}

void insertarelementoAlInicioCotizacionDetalle(NodoCotizacionDetalle** head, const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad) {
    NodoCotizacionDetalle* nuevoNodo = crearNodoCotizacionDetalle(id, nombreProducto, descripcion, precio, cantidad);
    nuevoNodo->siguiente = *head;
    *head = nuevoNodo;
    return;
}

void insertarElementoAlFinalCotizacionDetalle(NodoCotizacionDetalle** head, const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad) {

    //Cambio 
    if (head == NULL) {
        printf("Error: El puntero 'head' es NULL.\n");
        return;
    }

    NodoCotizacionDetalle* nuevoNodo = crearNodoCotizacionDetalle(id, nombreProducto, descripcion, precio, cantidad);

    if (nuevoNodo == NULL) {
        printf("Error al crear un nuevo nodo.\n");
        return;
    }    

    if (*head == NULL) {

        nuevoNodo->detallesCotizacion.NumeroFila = 1;
        *head = nuevoNodo;
        return;
    }

    int numeroFila = 2;
    NodoCotizacionDetalle* actual = *head;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
        numeroFila++;
    }
    nuevoNodo->detallesCotizacion.NumeroFila = numeroFila;

    actual->siguiente = nuevoNodo;
    return;
}

void eliminarPorIdCotizacionDetalle(NodoCotizacionDetalle** head, char * id) {

    NodoCotizacionDetalle* actual = *head;
    NodoCotizacionDetalle* anterior = NULL;

    while (actual != NULL && strcmp(actual->detallesCotizacion.IdProducto, id) != 0)
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

void eliminarCotizacionPorNumFila(NodoCotizacionDetalle** head, const int numFila) {

    NodoCotizacionDetalle* actual = *head;
    NodoCotizacionDetalle* anterior = NULL;

    while (actual != NULL && actual->detallesCotizacion.NumeroFila != numFila)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("El producto con el indice %i no ha sido encontrado.", numFila);
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


int buscarPorIdCotizacionDetalle(NodoCotizacionDetalle* head, const char* id) {

    if (head == NULL) {
        return 0;
    }

    NodoCotizacionDetalle* actual = head;

    while (actual != NULL)
    {
        if (strcmp(actual->detallesCotizacion.IdProducto, id) == 0) {
            return actual->detallesCotizacion.cantidad;
        }
        actual = actual->siguiente;
    }

    return 0;
}

void imprimirListaNodosCotizacionDetalle(NodoCotizacionDetalle* head) {
    NodoCotizacionDetalle *actual = head;

    while (actual != NULL)
    {
        printf("Datos del elementos de la lista:\n");
        printf("Id: %s, \nDescripcion: %s \n\n", actual->detallesCotizacion.IdProducto, actual->detallesCotizacion.Descripcion);
        actual = actual->siguiente;
    }
    return;
}

void modificarValoresNodoCotizacionDetalle(NodoCotizacionDetalle** head, const char * id, const int cantidad) {

    if (head == NULL) {
        return;
    }
    NodoCotizacionDetalle* actual = *head;
    while (actual != NULL)
    {

        // Buscamos el id que coincida y modificamos la cantidad.
        if (strcmp(actual->detallesCotizacion.IdProducto, id) == 0 ) {

            actual->detallesCotizacion.cantidad += cantidad;
            return;
        }
        actual = actual->siguiente;
    }

    return;

}


void liberarListaCotizacionDetalle(NodoCotizacionDetalle* head) {
    NodoCotizacionDetalle *actual;
    while (head != NULL)
    {
        actual = head;
        head = head->siguiente;

        free(actual->detallesCotizacion.IdProducto);
        free(actual->detallesCotizacion.NombreProducto);
        free(actual->detallesCotizacion.Descripcion);
        free(actual);
    }
    return;
}




void mostrar_cotizacion(NodoCotizacionDetalle* head) {

    if (head == NULL) {
        printf("Actualmente esta cotizacion, no tiene productos para mostrar.\n");
        return;
    }


    NodoCotizacionDetalle *actual = head;

    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
    printf("| Fila       | ID Producto  | Nombre               | Descripcion       | Precio      | Cantidad   |\n");
    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");

    while (actual != NULL)
    {
        printf("| %-10i | %-12s | %-20s | %-17s | %-11.2f | %-10i |\n", actual->detallesCotizacion.NumeroFila, actual->detallesCotizacion.IdProducto, actual->detallesCotizacion.NombreProducto, actual->detallesCotizacion.Descripcion, actual->detallesCotizacion.precio, actual->detallesCotizacion.cantidad);
        
        actual = actual->siguiente;
    }

    printf("+------------+--------------+----------------------+-------------------+-------------+-------------+\n\n");

    return;
}

void mostrar_cotizacionID(MYSQL *conexion, int id) {
    char *consulta = NULL;
    int largoConsulta = asprintf(&consulta, "call mostrarDetalleCotizacion('%d')", id);
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }
    free(consulta);

    MYSQL_RES *resultado = mysql_store_result(conexion);
    if (resultado == NULL) {
        printf("Error al obtener los resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila;

    char* nombres[100]; 
    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
    printf("| Fila       | ID Producto  | Nombre               | Descripcion       | Precio      | Cantidad   |\n");
    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");

    int filaNum = 1;
    while ((fila = mysql_fetch_row(resultado)) != NULL) {
        printf("| %-10d | %-12s | %-20s | %-17s | %-11.2f | %-10d |\n", filaNum, fila[0], fila[1], fila[2], atof(fila[3]), atoi(fila[4]));
        nombres[filaNum - 1] = strdup(fila[1]); 
        filaNum++;
    }

    printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");


    while (mysql_next_result(conexion) == 0) {
        MYSQL_RES *res = mysql_store_result(conexion);
        mysql_free_result(res);
    }
    

    printf("Si desea agregar productos escriba 1, si quiere eliminar escriba 2: ");
    int modifica2 = 0;
    scanf("%d", &modifica2);

    if (modifica2 == 1) {
        printf("Estas agregando productos, escribe el nombre del producto\n");
    } else if (modifica2 == 2) {
        printf("Estas eliminando productos, escribe el número de fila a eliminar:\n");
        int modifica3 = 0;
        scanf("%d", &modifica3);
        if (modifica3 > 0 && modifica3 <= filaNum) {
            printf("%d\n", modifica3);
            printf("%s\n", nombres[modifica3 - 1]);
            char *consulta2 = NULL;
            int largoConsulta2 = asprintf(&consulta2, "call eliminarLineaDetalle('%s')", nombres[modifica3 - 1]);
            if (mysql_query(conexion, consulta2)) {
                printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
                free(consulta2);
                return;
            }
            printf("Toda la línea fue eliminada correctamente.\n");
            free(nombres[modifica3 - 1]); 
            nombres[modifica3 - 1] = NULL;
            free(consulta2);
        } else {
            printf("Número de fila inválido.\n");
        }
    } else {
        printf("Opción inválida.\n");
    }

    for (int i = 0; i < filaNum - 1; i++) {
        if(nombres[i] != NULL) {
            free(nombres[i]);

        }
    }
}


void eliminarFilaBD(MYSQL *conexion, char *IdProd, int idCot) {
    char *consulta2 = NULL;
    printf("%s",IdProd);
    printf("%d",idCot);
    int largoConsulta2 = asprintf(&consulta2, "call eliminarDetalleCotizacion('%s','%d')", IdProd,idCot);
    if (mysql_query(conexion, consulta2)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta2);
        return;
    }
    free(consulta2);
}


void agregar_nuevo_producto(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idProducto, const int cantidad) {
    
    int existe_producto = validar_exitencia_producto(conexion, idProducto);
    if (existe_producto != 0) {
        printf("\nPor favo intentelo nuevamente...\n");
        return;
    }
    
    printf("%s",idProducto);
    printf("%d",cantidad);

    MYSQL_RES *resultado;
    MYSQL_ROW fila;
    char consulta[256]; // Tamaño que se podria ocupar por la cuestion del id

    // Crear la consulta al procedimiento almacenado
    snprintf(consulta, sizeof(consulta),"CALL obtenerProductoPorID('%s')", idProducto);

    // Ejecutar la consulta
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        return;
    }

    do {
        resultado = mysql_store_result(conexion);
        if (resultado) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {



                const char *nombreProducto = fila[1];
                const char *descripcion = fila[4];
                int precio = atoi(fila[2]);
                
                // Revisar si no hay otro elemento con este ID
                int cantProducto = buscarPorIdCotizacionDetalle(*head, idProducto);
                

                if (cantProducto != 0) {
                    

                    // Este es para combinar las cantodades de producto exitentes...
                    modificarValoresNodoCotizacionDetalle(head, idProducto, cantidad);

                } else {

                    // Agregar el producto a la cotización
                    insertarElementoAlFinalCotizacionDetalle(head, idProducto, nombreProducto, descripcion, precio, cantidad);

                    printf("Producto agregado a la cotización: %s\n", idProducto);
                }

            }
            mysql_free_result(resultado); // Liberar este conjunto de resultados
        }
    } while (mysql_next_result(conexion) == 0); // Mover al siguiente conjunto de resultados


    // Liberar memoria
    //mysql_free_result(resultado);

    return;
}


void optener_datos_cotizacion_por_id(MYSQL *conexion, NodoCotizacionDetalle** head, const int idCotizacion) {
    MYSQL_RES *resultado;
    MYSQL_ROW fila;

    char *consulta = NULL;
    int largoConsulta = asprintf(&consulta, "call mostrarDetalleCotizacion('%d')", idCotizacion);
    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }

    // registrar los datos del detalle de la cotizacion.
    do {
        resultado = mysql_store_result(conexion);
        if (resultado) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {

                // fila[0]: Id Prodcuto.
                // fila[1]: Nombre producto.
                // fila[2]: Descripcion.
                // fila[3]: Precio.
                // fila[4]: Cantidad.
                
                // Crear la lista de nodos con los datos de la cotizacion.
                insertarElementoAlFinalCotizacionDetalle(head, fila[0], fila[1], fila[2], atof(fila[3]), atoi(fila[4]));

            }
            mysql_free_result(resultado); // Liberar este conjunto de resultados
        }
    } while (mysql_next_result(conexion) == 0); // Mover al siguiente conjunto de resultados
    
    free(consulta);

    return;
}

/* Na */
void imprimirListaCotizacionDetalle(NodoCotizacionDetalle *lista) {
    NodoCotizacionDetalle *actual = lista;
    while (actual != NULL) {
        printf("IdProducto: %s\n", actual->detallesCotizacion.IdProducto);
        printf("NombreProducto: %s\n", actual->detallesCotizacion.NombreProducto);
        printf("Descripcion: %s\n", actual->detallesCotizacion.Descripcion);
        printf("NumeroFila: %d\n", actual->detallesCotizacion.NumeroFila);
        printf("Precio: %.2f\n", actual->detallesCotizacion.precio);
        printf("Cantidad: %d\n", actual->detallesCotizacion.cantidad);
        printf("-------------------------\n");
        actual = actual->siguiente;
    }
}

void crearCotizacion(MYSQL *conexion, const char *nombreCliente, int *idCotizacion) {
    printf("Entre aquí\n");
    char *consulta = NULL;

    // Insertar la cotización con estado 'Pendiente'
    int largoConsulta = asprintf(&consulta, 
        "INSERT INTO Cotizacion (EstadoCotizacion, Cliente) VALUES ('Pendiente', '%s');", 
        nombreCliente);

    if (mysql_query(conexion, consulta)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consulta);
        return;
    }

    // Obtener el ID generado automáticamente
    *idCotizacion = mysql_insert_id(conexion);
    printf("El ID de tu cotización es: %d\n", *idCotizacion);

    free(consulta);
}

void enviarCotizacionDB(MYSQL *conexion, NodoCotizacionDetalle *lista, int idCotizacion) {
    printf("Entre aquí\n");

    NodoCotizacionDetalle *actual = lista;
    char *consulta = NULL;

    while (actual != NULL) {
        const char *idProdu = actual->detallesCotizacion.IdProducto;
        int cantidadProductos = actual->detallesCotizacion.cantidad;
        float precioProducto = actual->detallesCotizacion.precio;

        // Insertar los detalles de la cotización
        int largoConsulta = asprintf(&consulta, 
            "INSERT INTO CotizacionDetalle (IdCotizacion, IdProducto, Cantidad, PrecioXunidad) "
            "VALUES ('%d', '%s', '%d', '%f');", 
            idCotizacion, idProdu, cantidadProductos, precioProducto);

        if (mysql_query(conexion, consulta)) {
            printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
            free(consulta);
            return;
        }

        free(consulta);
        actual = actual->siguiente;
    }
}

