#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <mysql/mysql.h>

struct NodoProducto;

typedef struct Productos
{
    char *IdProducto;
    char *Nombre;
    char *IdFamilia;
    float Costo;
    float Precio;
    int Cantidad;

} Productos;


typedef struct NodoProducto {

    Productos producto;

    struct NodoProducto *siguiente;

} NodoProducto;


int cargarProductosDesdeArchivo(char * nombreArchivo, NodoProducto** listaProductos);

void guardarProductosEnDB(MYSQL *conexion, NodoProducto* lista_productos);


// Lo relacionado con la lista dinamica para familia:

NodoProducto* crearNodoProducto(const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad);

void insertarElementoAlFinalProducto(NodoProducto** head, const char *idProducto, const char *nombre, const char *idFamilia, const float costo, float precio, int cantidad);

void imprimirListaNodosProducto(NodoProducto* head);

void liberarListaProducto( NodoProducto* head);

void eliminarProducto(MYSQL *conexion, const char *idProducto);

int validar_exitencia_producto(MYSQL *conexion, const char *idProducto);

#endif