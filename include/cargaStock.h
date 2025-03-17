#ifndef CARGASTOCK_H
#define CARGASTOCK_H


struct NodoCargaStock;


typedef struct CargaStock
{
    char *IdProducto;
    int Cantidad;

} CargaStock;


typedef struct NodoCargaStock {

    CargaStock stockCargado;

    struct NodoCargaStock *siguiente;

} NodoCargaStock;


int cargarStockDesdeArchivo(char * nombreArchivo, NodoCargaStock** listaCargaStock);

void guardarStockEnDB(NodoCargaStock* head);


// Lo relacionado con la lista dinamica para la carga del stock de un producto desde un archivo.

NodoCargaStock* crearNodoCargaStock(const char *idProducto, const int cantidad);

void insertarElementoAlFinalCargaStock(NodoCargaStock** head, const char *idProducto, const int cantidad);

void imprimirListaNodosCargaStock(NodoCargaStock* head);

void liberarListaCargaStock(NodoCargaStock* head);

#endif