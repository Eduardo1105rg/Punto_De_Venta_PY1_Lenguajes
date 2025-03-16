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

void guardarFamiliasEnDB(NodoFamilia* head);


// Lo relacionado con la lista dinamica para familia:

NodoFamilia* crearNodoFamilia(const char *id, const char *descripcion);

void imprimirListaNodosFamilia(NodoFamilia* head);

void insertarElementoAlFinalFamilia(NodoFamilia** head, const char *id, const char *descripcion);

void liberarListaFamilia(NodoFamilia* head);

#endif