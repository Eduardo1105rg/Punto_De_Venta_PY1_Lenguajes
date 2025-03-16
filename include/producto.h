#ifndef PRODUCTO_H
#define PRODUCTO_H

typedef struct Productos
{
    char *IdFamilia;
    char *Descripcon;

} Productos;


typedef struct NodoProducto {

    Productos familia;

    NodoProducto *siguiente;

} NodoProducto;


int cargarProductosDesdeArchivo(char * nombreArchivo);

void guardarProductosEnDB();

#endif