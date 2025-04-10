#ifndef COTIZACION_H
#define COTIZACION_H

#include <mysql/mysql.h>


struct NodoCotizacionDetalle;

typedef struct CotizacionDetalle
{
    char *IdProducto;
    char *NombreProducto;
    char *Descripcion;  // Esto es de la familia, la descripcion de esta.
    int NumeroFila;
    float precio;
    int cantidad;

} CotizacionDestalle;


typedef struct NodoCotizacionDetalle {

    CotizacionDestalle detallesCotizacion;

    struct NodoCotizacionDetalle * siguiente;

} NodoCotizacionDetalle;


NodoCotizacionDetalle* crearNodoCotizacionDetalle(const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad);

void imprimirListaNodosCotizacionDetalle(NodoCotizacionDetalle* head);

void insertarElementoAlFinalCotizacionDetalle(NodoCotizacionDetalle** head, const char *id, const char *nombreProducto ,const char *descripcion, int precio, int cantidad);

void liberarListaCotizacionDetalle(NodoCotizacionDetalle* head);

void eliminarPorIdCotizacionDetalle(NodoCotizacionDetalle** head, char * id);

void eliminarCotizacionPorNumFila(NodoCotizacionDetalle** head, const int numFila);

void eliminarCotizacionPorNumFilaV2(NodoCotizacionDetalle** head, const int numFila, NodoCotizacionDetalle** lista_elementos_eliminados);


void modificarValoresNodoCotizacionDetalle(NodoCotizacionDetalle** head, const char * id, const int cantidad);



// Apartados especificos..
void mostrar_cotizacion(NodoCotizacionDetalle* head);

void mostrar_cotizacionID(MYSQL *conexion, int id);

void agregar_nuevo_producto(MYSQL *conexion, NodoCotizacionDetalle** head, const char * idProducto, const int cantidad);

int optener_datos_cotizacion_por_id(MYSQL *conexion, NodoCotizacionDetalle** head, const int idCotizacion);

void eliminarFilaBD(MYSQL *conexion, char *IdProd, int idCot);

void imprimirListaCotizacionDetalle(NodoCotizacionDetalle *lista);

void crearCotizacion(MYSQL *conexion, const char *nombreCliente, int *idCotizacion);

void enviarCotizacionDB(MYSQL *conexion, NodoCotizacionDetalle *lista, int idCotizacion);

void mostrar_detalle_cotizacion_facturada(MYSQL *conexion, const int idCotizacion);

void pocesar_cambios_cotizacion(MYSQL *conexion, NodoCotizacionDetalle *lista, NodoCotizacionDetalle *lista_eliminados , int idCotizacion);

#endif