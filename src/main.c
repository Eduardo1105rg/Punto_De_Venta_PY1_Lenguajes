#include <stdio.h>
#include "../include/familia.h"



// Comando para ejecutar desde c, se deben de compilar todos los archivos .c
// gcc -o punto_de_venta src/main.c src/familia.c src/manipularArchivos.c


int main() {

    NodoFamilia *listaDeFamilias = NULL;


    cargarFamiliasDesdeArchivo("data/familias.txt", &listaDeFamilias);

    imprimirListaNodos(listaDeFamilias);

    liberarLista(listaDeFamilias);

    return 0;
}