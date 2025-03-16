#ifndef FAMILIA_H
#define FAMILIA_H


struct NodoFamilia;


typedef struct FamiliaProductos
{
    char *IdFamilia;
    char *Descripcion;

} FamiliaProductos;


typedef struct NodoFamilia {

    FamiliaProductos familia;

    struct NodoFamilia *siguiente;

} NodoFamilia;

int cargarFamiliasDesdeArchivo(char * nombreArchivo, NodoFamilia** listaDeFamilias);

void guardarFamiliasEnDB();


// Lo relacionado con la lista dinamica para familia:

NodoFamilia* crearNodo(const char *id, const char *descripcion);

void imprimirListaNodos(NodoFamilia* head);

void insertarElementoAlFinal(NodoFamilia** head, const char *id, const char *descripcion);

void liberarLista(NodoFamilia* head);

#endif