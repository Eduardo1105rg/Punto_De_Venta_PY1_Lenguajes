#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>


// Include de archivos del programa.
#include "../include/generales.h"
#include "../include/consultaCatalogo.h"
#include "../include/manipularArchivos.h"
#include "../include/db.h"
#include "../include/cotizacion.h"
#include "../include/facturacion.h"


int cantidadCotizaciones = 1;



/**
 * Nombre: ConsultaOpcional
 * 
 * Descripcion: En este apartado se descuenta el stock de los productos en factura y se hace la consulta en caso de ser necesario
 un ajuste
 * 
 * Funcionamiento: Primero verifica los datos que se van a enviar a la cotizacion para la parte del stock, si no es necesario
 no se ajusta sino se le indica al usuario por medio de un mensaje si lo quiere modificar
 * 
 * Entradas: Un puntero a un tipo de dato MYSQL, un entero para el id de cotizacion
 * 
 * Salidas: No tiene
 * 
 */
void ConsultaOpcional(MYSQL *conexion, int id_cotizacion) {
    char *consultaBF = NULL;

    int largoConsultaBF = asprintf(&consultaBF, "CALL VerificarYRestarStock(%d)", id_cotizacion);

    if (mysql_query(conexion, consultaBF)) {
        printf("Me caí aquí\n");
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaBF);
        return;
    }
    printf("Verifiqué el stock\n");

    
    int *ajustes = NULL;
    char **productos = NULL;
    int *cantidadesSolicitadas = NULL;
    int *cantidadesDisponibles = NULL;
    int index = 0;
    int capacidad = 10;  

    ajustes = (int *)malloc(capacidad * sizeof(int));
    productos = (char **)malloc(capacidad * sizeof(char *));
    cantidadesSolicitadas = (int *)malloc(capacidad * sizeof(int));
    cantidadesDisponibles = (int *)malloc(capacidad * sizeof(int));

    if (!ajustes || !productos || !cantidadesSolicitadas || !cantidadesDisponibles) {
        printf("Error al asignar memoria\n");
        free(consultaBF);
        free(ajustes);
        free(productos);
        free(cantidadesSolicitadas);
        free(cantidadesDisponibles);
        return;
    }
    do {
        MYSQL_RES *resultadoBF = mysql_store_result(conexion);
        if (resultadoBF) {
            MYSQL_ROW filaBF;

            while ((filaBF = mysql_fetch_row(resultadoBF)) != NULL) {
                int cantidadSolicitada = atoi(filaBF[1]);
                int cantidadDisponible = atoi(filaBF[2]);
                
                // Si la cantidad solicitada es menor o igual a la disponible no es necesario que lo ajustemos
                if (cantidadSolicitada <= cantidadDisponible) {
                    printf("Producto %s tiene suficiente stock (%d disponibles, %d solicitados), restando automáticamente.\n",
                        filaBF[0], cantidadDisponible, cantidadSolicitada);
                    
                    char *consultaBF5 = NULL;
                    asprintf(&consultaBF5,
                        "UPDATE Productos SET Cantidad = Cantidad - %d WHERE IdProducto = '%s';",
                        cantidadSolicitada, filaBF[0]);

                    if (mysql_query(conexion, consultaBF5)) {
                        printf("Error al realizar la consulta (Productos - descuento automático): %s\n", mysql_error(conexion));
                    }
                    free(consultaBF5);
                    continue; 
                }
                if (cantidadDisponible == 0) {
                    printf("Lo sentimos, actualmente no podemos generar un re-stock pues tenemos '%d' productos.\n", cantidadDisponible);
                
                    mysql_free_result(resultadoBF);
                
                    while (mysql_next_result(conexion) == 0) {
                        MYSQL_RES *temp = mysql_store_result(conexion);
                        if (temp) mysql_free_result(temp);
                    }
                
                    char *consultaBF7 = NULL;
                    asprintf(&consultaBF7,
                        "DELETE FROM CotizacionDetalle WHERE IdCotizacion = %d AND IdProducto = '%s';",
                        id_cotizacion, filaBF[0]);  
                
                    if (mysql_query(conexion, consultaBF7)) {
                        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
                    } else {
                        printf("Línea de cotización eliminada correctamente.\n");
                    }
                
                    free(consultaBF7);
                    return;
                }
                
                
                
                

                printf("Estimado usuario, este producto %s nos pide una cantidad de %d\n"
                       "Actualmente contamos con %d disponibles, desea ajustarlo? "
                       "Si es así, escriba un 1; sino, un 0: ",
                       filaBF[0], cantidadSolicitada, cantidadDisponible);
                
                int recibep = 0;
                if (scanf("%d", &recibep) != 1) {
                    printf("Error en la entrada. Por favor ingrese un número válido.\n");
                    mysql_free_result(resultadoBF);
                    free(consultaBF);
                    free(ajustes);
                    free(productos);
                    free(cantidadesSolicitadas);
                    free(cantidadesDisponibles);
                    return;
                }
                if (index >= capacidad) {
                    capacidad *= 2; 
                    ajustes = (int *)realloc(ajustes, capacidad * sizeof(int));
                    productos = (char **)realloc(productos, capacidad * sizeof(char *));
                    cantidadesSolicitadas = (int *)realloc(cantidadesSolicitadas, capacidad * sizeof(int));
                    cantidadesDisponibles = (int *)realloc(cantidadesDisponibles, capacidad * sizeof(int));

                    if (!ajustes || !productos || !cantidadesSolicitadas || !cantidadesDisponibles) {
                        printf("Error al reasignar memoria\n");
                        free(consultaBF);
                        free(ajustes);
                        free(productos);
                        free(cantidadesSolicitadas);
                        free(cantidadesDisponibles);
                        return;
                    }
                }

                ajustes[index] = recibep;
                productos[index] = strdup(filaBF[0]); 
                cantidadesSolicitadas[index] = cantidadSolicitada;
                cantidadesDisponibles[index] = cantidadDisponible;
                index++;
            }

            mysql_free_result(resultadoBF);
        }
    } while (mysql_next_result(conexion) == 0);  

    for (int i = 0; i < index; i++) {
        if (ajustes[i] == 1) {
            
            // Actualizar CotizacionDetalle
            char *consultaBF2 = NULL;
            asprintf(&consultaBF2,
                "UPDATE CotizacionDetalle SET Cantidad = %d WHERE IdCotizacion = %d AND IdProducto = '%s';",
                cantidadesDisponibles[i], id_cotizacion, productos[i]);

            if (mysql_query(conexion, consultaBF2)) {
                printf("Error al realizar la consulta (CotizacionDetalle): %s\n", mysql_error(conexion));
            }
            free(consultaBF2);

            // Actualizar Productos
            char *consultaBF3 = NULL;
            asprintf(&consultaBF3,
                "UPDATE Productos SET Cantidad = Cantidad - %d WHERE IdProducto = '%s';",
                cantidadesDisponibles[i], productos[i]);

            if (mysql_query(conexion, consultaBF3)) {
                printf("Error al realizar la consulta (Productos): %s\n", mysql_error(conexion));
            }
            free(consultaBF3);

            printf("Stock ajustado.\n");
        } else {
            printf("No se ajusta el stock de %s.\n", productos[i]);
        }
        free(productos[i]); 
    }

    free(consultaBF);
    free(ajustes);
    free(productos);
    free(cantidadesSolicitadas);
    free(cantidadesDisponibles);
}


/**
 * Nombre: menu_consulta_catalogo
 * 
 * Descripcion: El menu pincipal para la consulta de catalogo, se consulta de dos modos, ver todos las productos en una tabla y el otros modo
 * es ver todos lod prodcutos de una familia especifica.
 * 
 * Funcionamiento: Los productos agregados a la base de datos, se mostraran en forma de tabla, esta funcion hacer peticiones a una vista en 
 * de la base de datos, para optener todos los prodcutos registrado. En la opcion de ver catalogo por familia, el usuario ingresa el nombre de una familia
 * el sistema llama al procedimiento alamacenado en la base de datos para optener los datos.
 * 
 * Entradas: No tiene
 * 
 * Salidas: No tiene
 * 
 */
void menu_consulta_catalogo() {

    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    char opcion;
    do {
        printf("\nConsultal del catalogo... \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Consulta normal \n");
        printf(">> B) Consulta por familia.\n");
        printf(">> S) Volver al menu anterior.\n");


        printf("Ingrese la letra de la consulta a realizar: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
            
                consultarCatalogo(conexion, 1);
                break;
            case 'A':
                consultarCatalogo(conexion, 1);
                break;

            // ========== Consulta de catalogo por la descripcion de la familia.
            case 'b':
                 
                // Solicitar 
                char *descripcion_familia1;
                printf("\nIngresa la descripcion de la familia a buscar: ");
                leerCaracteresDeFormadinamica(&descripcion_familia1);
                printf("\n");
                consultarCatalogoPorFamilia(conexion, descripcion_familia1, 1);
                free(descripcion_familia1);
                break;

            case 'B':

                char *descripcion_familia2;
                printf("\nIngresa la descripcion de la familia a buscar: ");
                leerCaracteresDeFormadinamica(&descripcion_familia2);
                printf("\n");
                consultarCatalogoPorFamilia(conexion, descripcion_familia2, 1);
                free(descripcion_familia2);

                break;

            // ========== Salir del menu.
            case 's':
                printf("Volviendo a la seccion anterior...\n");
                break;
            case 'S':
                printf("Volviendo a la seccion anterior...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's');

    cerrarConexion(conexion);
    
    return;
}


/**
 * Nombre:menu_cotizacion
 * 
 * Descripcion: Es el menu principal para lo que viene despues de crear la cotizacion o sea el relleno de su contenido
 * 
 * Funcionamiento:Una vez creada nuestra cotizacion podemos agregar productos, ver catalogo, eliminarlos, registrar la cotizacion
 *  o salir y no guardarla, conforme hagamos cambios el usuario podra observar los productos que esta cotizando, sus valores o le 
 *  muestra si borra alguno, pero para hacer los cambios efectivos debe guardalo
 * 
 * Entradas:No tiene
 * 
 * Salidas:No tiene
 * 
 */
void menu_cotizacion() {

    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    NodoCotizacionDetalle *lista_productos_en_cotizacion = NULL;

    char opcion;
    do {
        printf("\nCotizacion... \n");

        mostrar_cotizacion(lista_productos_en_cotizacion);


        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Ver catalogo de productos. \n");
        printf(">> B) Agregar productos.\n");
        printf(">> C) Eliminar producto.\n");
        printf(">> D) Registrar cotizacion.\n");
        printf(">> S) Volver al menu anterior.\n");


        printf("Ingrese la letra de la accion a realizar: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
            
                menu_consulta_catalogo();
                break;
            case 'A':
                menu_consulta_catalogo();
                break;

            // ========== Agregar un nuevo producto a la cotizacion.
            case 'b':
                 
                // Solicitar 
                char *id_producto1;
                printf("\nIngresa el id del producto a agregar: ");
                leerCaracteresDeFormadinamica(&id_producto1);
                printf("\n");


                printf("\nIngresa la cantidad del producto que desea agregar: ");
                int cantidad_producto1 = leeNumeroDinamicoV2();
                printf("\n");

                if (cantidad_producto1 <= 0) {
                    printf("Error: No pueden haber numeros menores o igualesa cero");
                    break;
                }
                //printf("Pass0");
                agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto1, cantidad_producto1);

                free(id_producto1);
                break;

            case 'B':

                // Solicitar el id del producto.
                char *id_producto2;
                printf("\nIngresa el id del producto a agregar: ");
                leerCaracteresDeFormadinamica(&id_producto2);
                printf("\n");

                //Solicitar la cantidad del producto.
                printf("\nIngresa la cantidad del producto que desea agregar: ");
                int cantidad_producto2 = leeNumeroDinamicoV2();
                printf("\n");
                if (cantidad_producto2 <= 0) {
                    printf("Error: No pueden haber numeros menores o igualesa cero");
                    break;
                }

                agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto2, cantidad_producto2);

                free(id_producto2);

                break;

            // ========== Eliminar un producto de la cotizacion.
            case 'c':

                //char *id_producto_a_eliminar1;
                printf("\nIngresa el numero de la fila del producto que desea eliminar: ");
                // leerCaracteresDeFormadinamica(&id_producto_a_eliminar1);
                int num_fila_eliminar1 = leeNumeroDinamicoV2();
                printf("\n");
                if (num_fila_eliminar1 == -21) {
                    break;
                }
                if (num_fila_eliminar1 <= 0) {
                    printf("Error: No pueden haber numeros menores o igualesa cero");
                    break;
                }
                eliminarCotizacionPorNumFila(&lista_productos_en_cotizacion, num_fila_eliminar1);
                //eliminarPorIdCotizacionDetalle(&lista_productos_en_cotizacion, id_producto_a_eliminar1);

                //free(id_producto_a_eliminar1);

                break;

            case 'C':

                printf("\nIngresa el numero de la fila del producto que desea eliminar: ");
                int num_fila_eliminar2 = leeNumeroDinamicoV2();
                printf("\n");
                if (num_fila_eliminar2 == -21) {
                    break;
                }
                if (num_fila_eliminar2 <= 0) {
                    printf("Error: No pueden haber numeros menores o igualesa cero");
                    break;
                }
                eliminarCotizacionPorNumFila(&lista_productos_en_cotizacion, num_fila_eliminar2);
                break;

            // ========== Guardar la cotizacion realizada.
            case 'd':
                mostrar_cotizacion(lista_productos_en_cotizacion);

                // Validar si deverdad se quiere guardar.
                char aceptar;

                printf("Seguro que desea guardar? si es asi presione cualquier letra\n ");
                printf("Distinta de N y si quiere borrar presione N(para no guardar)\n ");

                // scanf(" %c", &aceptar); 
                // getchar(); 
                leerPrimerCaracter(&aceptar);

                if (aceptar != 'N' && aceptar != 'n') {
                    char *nombre_cliente1;
                    printf("\nIngresa el nombre del cliente para la cotizacion: ");
                    leerCaracteresDeFormadinamica(&nombre_cliente1);
                    printf("\n");
                    int idCotizacion;
                    crearCotizacion(conexion, nombre_cliente1, &idCotizacion);
                    enviarCotizacionDB(conexion,lista_productos_en_cotizacion,idCotizacion );
                    opcion = 's';
                }


                // char *nombre_cliente1;
                // printf("\nIngresa el nombre del cliente para la cotizacion: ");
                // leerCaracteresDeFormadinamica(&nombre_cliente1);
                // printf("\n");
                // int idCotizacion;
                // crearCotizacion(conexion, nombre_cliente1, &idCotizacion);
                // enviarCotizacionDB(conexion,lista_productos_en_cotizacion,idCotizacion );
                //printf("Cotización creada, tu ID de cotizacion es: %d", cantidadCotizaciones);  
                break;

            case 'D':
                mostrar_cotizacion(lista_productos_en_cotizacion);

                // Validar si de verdad se quiere guardar.
                char aceptar2;

                printf("Seguro que desea guardar? si es asi presione cualquier letra\n ");
                printf("Distinta de N y si quiere borrar presione N(para no guardar)\n ");

                // scanf(" %c", &aceptar2); 
                // getchar(); 
                leerPrimerCaracter(&aceptar2);


                if (aceptar2 != 'N' && aceptar2 != 'n') {
                    char *nombre_cliente2;
                    printf("\nIngresa el nombre del cliente para la cotizacion: ");
                    leerCaracteresDeFormadinamica(&nombre_cliente2);
                    printf("\n");

                    int idCotizacion2;
                    crearCotizacion(conexion, nombre_cliente2,&idCotizacion2);
                    enviarCotizacionDB(conexion,lista_productos_en_cotizacion,idCotizacion2);
                    opcion = 's';
                    //printf("Cotización creada, tu ID de cotizacion es: %d", cantidadCotizaciones);  
                }
                break;

            // ========== Salir del menu.
            case 's': 
                char op1;
                printf("Advertencia: Si sales en este momento no se guardara la cotización.\n");
                printf("Si desea salir vuelva a escribir S: ");
                // scanf("%c",&op1);
                // getchar();
                leerPrimerCaracter(&op1);

                printf("\n");
                if(op1 == 'S' || op1 == 's'){
                    printf("Volviendo a la seccion anterior...\n");
                    opcion = 's';
                }else {
                    opcion = 'n';
                }
                break;

            case 'S':
                char op2;
                printf("Advertencia: Si sales en este momento no se guardara la cotización.\n");
                printf("Si desea salir vuelva a escribir S: ");
                // scanf("%c",&op2);
                // getchar();
                leerPrimerCaracter(&op2);

                printf("\n");

                if(op2 == 'S' || op2 == 's'){
                    printf("Volviendo a la seccion anterior...\n");
                    opcion = 's';
                } else {
                    opcion = 'n';
                }
            break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's' && opcion != 'S');

    cerrarConexion(conexion);
    
    liberarListaCotizacionDetalle(lista_productos_en_cotizacion);

    return;
}


int ingresar_indentificador_cotizacion() {
    while (1)
    {
        printf("Por favor escriba el identificador de la cotizacion:");
        int identificadorCotizacion = leeNumeroDinamicoV2();
        // scanf("%d", &identificadorCotizacion);
        printf("\n");
    
        if (identificadorCotizacion != -21) {
            return identificadorCotizacion;
        }
        printf("El valor ingresado no es aceptado. Ingresolo nuevamente.\n\n");
    }


    return -21;
}
/**
 * Nombre:menu_modificar_cotizacion
 * 
 * Descripcion: Es un menu que contiene opciones acerca de una cotización
 * 
 * Funcionamiento:Es un menu que nos deja por medio de un ID de cotizacion podemos agregar productos, eliminar productos,
 ver catalogo, guardarlo o salir, en general podremos salir y no se guardan los cambios pero si hacemos eliminaciones o 
 agregamos productos estos cambios no se realizaran de forma permanente hasta que se guarde
 * 
 * Entradas:No tiene
 * 
 * Salidas:No tiene
 * 
 */
void menu_modificar_cotizacion() {

    MYSQL *conexion = NULL;
    if (conectar(&conexion) != 0) {
        
        return; 
    }

    NodoCotizacionDetalle *lista_productos_en_cotizacion = NULL;


    //printf("Por favor escriba el identificador de la cotizacion a modificar:");
    int identificadorCotizacion = ingresar_indentificador_cotizacion();
    // scanf("%d", &identificadorCotizacion);
    printf("\n");

    if (identificadorCotizacion == -21) {
        return;
    }

    int estado_datos = optener_datos_cotizacion_por_id(conexion, &lista_productos_en_cotizacion, identificadorCotizacion);

    if (estado_datos == 0) {
        printf("El id de cotizacion ingresado no ha sido encontrado.");
        return;
    }

    char opcion;
    do {
        printf("\nModificar cotizacion... \n");

        mostrar_cotizacion(lista_productos_en_cotizacion);


        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Ver catalogo de productos. \n");
        printf(">> B) Agregar productos.\n");
        printf(">> C) Eliminar producto.\n");
        printf(">> D) Guardar cambios.\n");
        printf(">> S) Volver al menu anterior (No se guardan los cambios).\n");


        printf("Ingrese la letra de la accion a realizar: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);


        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
            
                menu_consulta_catalogo();
                break;
            case 'A':
                menu_consulta_catalogo();
                break;

            // ========== Agregar un nuevo producto a la cotizacion.
            case 'b':

                 
                char *id_producto1;
                printf("\nIngresa el id del producto a agregar: ");
                leerCaracteresDeFormadinamica(&id_producto1);
                printf("\n");

                printf("\nIngresa la cantidad del producto que desea agregar: ");
                int cantidad_producto1 = leeNumeroDinamicoV2();
                if (cantidad_producto1 == -21) {
                    break;
                }
                if (cantidad_producto1 <= 0) {
                    printf("Error: No pueden haber numeros menores o igualesa cero");
                    break;
                }
                printf("\n");
                agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto1, cantidad_producto1);
                int guarda = 1;

                free(id_producto1);
                break;

            case 'B':

                // Solicitar el id del producto.
                char *id_producto2;
                printf("\nIngresa el id del producto a agregar: ");
                leerCaracteresDeFormadinamica(&id_producto2);
                printf("\n");

                //Solicitar la cantidad del producto.
                printf("\nIngresa la cantidad del producto que desea agregar: ");
                int cantidad_producto2 = leeNumeroDinamicoV2();
                if (cantidad_producto2 == -21) {
                    break;
                }
                if (cantidad_producto2 <= 0) {
                    printf("Error: No pueden haber numeros menores o igualesa cero");
                    break;
                }
                printf("\n");

                agregar_nuevo_producto(conexion, &lista_productos_en_cotizacion, id_producto2, cantidad_producto2);

                free(id_producto2);

                break;

            // ========== Eliminar un producto de la cotizacion.
            case 'c':

                //char *id_producto_a_eliminar1;
                printf("\nIngresa el numero de la fila del producto que desea eliminar: ");
                // leerCaracteresDeFormadinamica(&id_producto_a_eliminar1);
                int num_fila_eliminar1 = leeNumeroDinamicoV2();

                printf("\n");

                eliminarCotizacionPorNumFila(&lista_productos_en_cotizacion, num_fila_eliminar1);
                //eliminarPorIdCotizacionDetalle(&lista_productos_en_cotizacion, id_producto_a_eliminar1);

                //free(id_producto_a_eliminar1);

                break;
            case 'C':

                printf("\nIngresa el numero de la fila del producto que desea eliminar: ");
                int num_fila_eliminar2 = leeNumeroDinamicoV2();
                printf("\n");

                eliminarCotizacionPorNumFila(&lista_productos_en_cotizacion, num_fila_eliminar2);

                break;

            // ========== Guardar la cotizacion creada.
            case 'd':

                char aceptar1;
                printf("Seguro que desea guardar? si es asi presione cualquier letra\n ");
                printf("Distinta de N y si quiere borrar presione N(para no guardar)\n ");

                // scanf(" %c", &aceptar2); 
                // getchar(); 
                leerPrimerCaracter(&aceptar1);


                if (aceptar1 == 'N' && aceptar1 == 'n') {
                    break;
                }
                opcion = 's';


                //Aqui lo que hacemos es utilizar la lista de los productos más actualizada para así pasar los datos de poco a poco
                //aunque lo que realmente se termina haciendo es una consulta para insertar estos datos en la base una vez el usuario
                //Haya decidido guardarlos
                if(guarda == 1) {
                    char * consulta3 = NULL;
                    NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                    while (actual != NULL) { 
                        char *IdProd = actual->detallesCotizacion.IdProducto;
                        int cantidadProdu = actual->detallesCotizacion.cantidad;
                        float precio = actual->detallesCotizacion.precio;

                        printf("%d",identificadorCotizacion);                    
                        printf("ID Producto: %s, Nombre Producto: %d\n", IdProd, identificadorCotizacion);
                        
                        int largoConsulta = asprintf(&consulta3, 
                            "INSERT INTO CotizacionDetalle (IdCotizacion, IdProducto,Cantidad,PrecioXunidad) VALUES (%d,'%s',%d,%f);", 
                            identificadorCotizacion, IdProd, cantidadProdu, precio);
                    
                        if (mysql_query(conexion, consulta3)) {
                            printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
                            free(consulta3);
                            return;
                        }
                        free(consulta3);                     
                        actual = actual->siguiente; 
                    }
                

                }else { 
                    NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                    while (actual != NULL) { 
                        char *IdProd = actual->detallesCotizacion.IdProducto;  
                        printf("%d",identificadorCotizacion);                    
                        printf("ID Producto: %s, Nombre Producto: %d\n", IdProd, identificadorCotizacion);
                        
                        eliminarFilaBD(conexion, IdProd, identificadorCotizacion);
                        
                        actual = actual->siguiente; 
                    }
                }
                break;

            case 'D':

                char aceptar2;
                printf("Seguro que desea guardar? si es asi presione cualquier letra\n ");
                printf("Distinta de N y si quiere borrar presione N(para no guardar)\n ");

                // scanf(" %c", &aceptar2); 
                // getchar(); 
                leerPrimerCaracter(&aceptar2);


                if (aceptar2 == 'N' && aceptar2 == 'n') {
                    break;
                }
                opcion = 's';

                if(guarda == 1) {
                    char * consulta3 = NULL;
                    NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                    while (actual != NULL) { 
                        char *IdProd = actual->detallesCotizacion.IdProducto;
                        int cantidadProdu = actual->detallesCotizacion.cantidad;
                        float precio = actual->detallesCotizacion.precio;                   
                        int largoConsulta = asprintf(&consulta3, 
                            "INSERT INTO CotizacionDetalle (IdCotizacion, IdProducto,Cantidad,PrecioXunidad) VALUES (%d,'%s',%d,%f);", 
                            identificadorCotizacion, IdProd, cantidadProdu, precio);
                    
                        if (mysql_query(conexion, consulta3)) {
                            printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
                            free(consulta3);
                            return;
                        }
                        free(consulta3);                    
                        actual = actual->siguiente; 
                    }
                

                }else { 
                    NodoCotizacionDetalle *actual = lista_productos_en_cotizacion;
                    while (actual != NULL) { 
                        char *IdProd = actual->detallesCotizacion.IdProducto;  
                        printf("%d",identificadorCotizacion);                    
                        printf("ID Producto: %s, Nombre Producto: %d\n", IdProd, identificadorCotizacion);
                        
                        eliminarFilaBD(conexion, IdProd, identificadorCotizacion);
                        
                        actual = actual->siguiente; 
                    }
                }
                break;

            // ========== Salir del menu.
            case 's': 
                char op1;

                printf("Advertencia: Si sales en este momento no se guardara la cotización.\n");
                printf("Si desea salir vuelva a escribir S: ");
                // scanf("%c",&op2);
                // getchar();
                leerPrimerCaracter(&op1);

                printf("\n");

                if(op1 == 'S' || op1 == 's'){
                    printf("Volviendo a la seccion anterior...\n");
                    opcion = 's';
                }else {
                    opcion = 'n';
                }
                break;

            case 'S':
                char op2;
                printf("Advertencia: Si sales en este momento no se guardara la cotización.\n");
                printf("Si desea salir vuelva a escribir S: ");
                // scanf("%c",&op2);
                // getchar();s
                leerPrimerCaracter(&op2);

                printf("\n");

                if(op2 == 'S' || op2 == 's'){
                    printf("Volviendo a la seccion anterior...\n");
                    opcion = 's';
                }else {
                    opcion = 'n';
                }
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's' && opcion != 'S');

    cerrarConexion(conexion);
    
    liberarListaCotizacionDetalle(lista_productos_en_cotizacion);

    return;

}


/**
 * Nombre:menu_crear_factura
 * 
 * Descripcion: Esta es la parte que hace toda la facturación de las cotizaciones
 * 
 * Funcionamiento: En si primero le pedimos al usuario el nombre de la persona que cotizo y el ID de esta cotización
 de forma que si coincide le mostramos la facturacion en base a lo que nos había dado en la cotización, cambiando
 su estado y mostrando todo esto por medio de prints, así el usuario puede observar todo
 * 
 * Entradas:No tiene
 *  
 * Salidas:No tiene
 * 
 */
void menu_crear_factura() {
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);

    //Funcion que habiamos hecho en el portafolio para obtener la fecha y hora actual
    char fechaHora[70];
    char *formato = "%Y-%m-%d %H:%M:%S";
    int escritos = strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);
    if (escritos != 0) {
        printf("Fecha y hora: %s\n", fechaHora);
    
    } else {
        printf("No se formateo bien la fecha.\n");
    }

    MYSQL *conexion = NULL;

    if (conectar(&conexion) != 0) {
        return;
    }


    // printf("Antes de darle su factura por favor escribanos su numero de cotizacion\n");
    // printf("y si es muy amable el nombre de la persona que cotizo\n");
    // scanf("%d", &numCotizacionF);
    // getchar(); 
    // scanf("%s", nombreClienteF);

    printf("\nIngresa el id de la cotizacion a factruar: ");
    int id_cotizacion = ingresar_indentificador_cotizacion();
    //printf("\n Valor ingresado: %i\n", id_cotizacion);

    printf("\n");

    char *consultaff = NULL;
    asprintf(&consultaff,
        "select EstadoCotizacion from Cotizacion where IdCotizacion = %d;",
        id_cotizacion);

    if (mysql_query(conexion, consultaff)) {
        printf("Error al realizar la consulta (Productos): %s\n", mysql_error(conexion));
    }
    MYSQL_RES *resultadoff = mysql_store_result(conexion);
    MYSQL_ROW filas = mysql_fetch_row(resultadoff);
    if (filas != NULL) {
        char *estadoCotizacion = filas[0]; 
    
        if (estadoCotizacion != NULL && strcmp(estadoCotizacion, "Facturado") == 0) {
            printf("La cotización ya está facturada.\n");
            mysql_free_result(resultadoff);
            free(consultaff);
            return;
        }
    }
    
    mysql_free_result(resultadoff);
    free(consultaff);


    char *nombre;
    printf("\nIngresa el nombre del cliente que se usao para la cotizacion: ");
    leerCaracteresDeFormadinamica(&nombre);


    //Validacion nombre cliente
    char *consultaff2 = NULL;
    asprintf(&consultaff2,
        "SELECT Cliente FROM Cotizacion WHERE IdCotizacion = %d;",
        id_cotizacion);
    
    if (mysql_query(conexion, consultaff2)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaff2);
        return;
    }
    
    MYSQL_RES *resultadoff2 = mysql_store_result(conexion);
    if (resultadoff2) {
        MYSQL_ROW filas2 = mysql_fetch_row(resultadoff2);
        
        if (filas2 != NULL) {
            if (strcmp(filas2[0], nombre) != 0) {
                printf("Error: El nombre del cliente debe ser el mismo de la cotización.\n");
                mysql_free_result(resultadoff2);
                free(consultaff2);
                return;
            }
        } else {
            printf("Error: No se encontró la cotización con el ID ingresado.\n");
        }
    
        mysql_free_result(resultadoff2);
    }
    
    free(consultaff2);
        //Validacion nombre cliente


    printf("\n");

    //Aqui realizamos lo de quitar stock
    ConsultaOpcional(conexion,id_cotizacion);

    //Cambiamos el estado de la cotizacion a Facturado
    char *consultaFC = NULL;
    int largoConsultaFC = asprintf(&consultaFC, "update Cotizacion set EstadoCotizacion = '%s' where IdCotizacion = '%d'", "Facturado", id_cotizacion);
    if (mysql_query(conexion, consultaFC)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaFC);
        return;
    }
    free(consultaFC);





    //Aqui recien creamos la factura antes de mostrarla
    int resID = crearFactura(conexion, id_cotizacion, nombre, fechaHora);
    printf("DEBUG: Valor de resID = %d\n", resID);


    //Esta se podría ver como la parte inicial de la factura pues de aquí sacamos cierto tipo de datos
    //Que van en el encabezado de esta misma
    char *consultaF = NULL;
    int largoConsultaF = asprintf(&consultaF, "select NumSecuencial, NombreLocal, CedulaJuridica, Telefono from Negocio");

    if (mysql_query(conexion, consultaF)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaF);
        return;
    }
    free(consultaF);

    MYSQL_RES *resultado2 = mysql_store_result(conexion);
    if (resultado2 == NULL) {
        printf("No se obtuvieron resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila;
    //Aqui lo que hacemos es ir pasando de fila en fila en los resultados para así mostrarlo por medio de prints 
    //Pero como este no tiene muchos datos solo es de aplicarle un break
    while ((fila = mysql_fetch_row(resultado2)) != NULL) {
        char *empresaNombre = fila[1];   
        char *cedulaJuridica = fila[2];  
        char *telefonoEmpresa = fila[3]; 

        printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
        printf("|                                       %-57s |\n", empresaNombre); 
        printf("| Fecha de emisión: %-77s |\n", fechaHora);
        printf("| Identificador: %-80d |\n", resID);
        printf("| Cédula jurídica: %-78s |\n", cedulaJuridica);
        printf("| Teléfono: %-85s |\n", telefonoEmpresa);
        printf("| Cliente: %-86s |\n", nombre);
        printf("|                                       %-57s |\n", "Productos"); 

        break;
    }

    //Aqui sacamos la parte de la cotizacion para así mostrarla en la factura
    char *consultaF2 = NULL;
    int largoConsultaF2 = asprintf(&consultaF2, "call facturaFin('%d', '%s')", id_cotizacion, nombre);

    if (mysql_query(conexion, consultaF2)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaF2);
        return;
    }
    free(consultaF2);

    MYSQL_RES *resultado3 = mysql_store_result(conexion);
    if (resultado3 == NULL) {
        printf("No se obtuvieron resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila2;
    //Aqui si es necesario el bucle pues hasta que ya no hayan más datos este seguira imprimiendo
    //Los productos que fueron comprados por el usuario
    while ((fila2 = mysql_fetch_row(resultado3)) != NULL) {
        char *Producto = fila2[0];   
        char *Cantidad = fila2[1];    
        char *PrecioXUnidad = fila2[2];
        char *subtotal = fila2[3];  
        char *Impuesto = fila2[4]; 
        char *Total = fila2[5];
        printf("| Producto: %-85s |\n", Producto);
        printf("| Cantidad: %-85s |\n", Cantidad);
        printf("| Precio X Unidad: %-78s |\n", PrecioXUnidad);
        printf("| subtotal: %-85s |\n", subtotal);
        printf("| Impuesto: %-85s |\n", Impuesto);
        printf("| Total: %-88s |\n", Total);
        printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
    }


    while (mysql_next_result(conexion) == 0) {
        MYSQL_RES *res = mysql_store_result(conexion);
        mysql_free_result(res);
    }

    char *consultaF3 = NULL;
    int largoConsultaF3 = asprintf(&consultaF3, "call facturaFinDinero('%d', '%s')", id_cotizacion, nombre);

    if (mysql_query(conexion, consultaF3)) {
        printf("Error al realizar la consulta: %s\n", mysql_error(conexion));
        free(consultaF3);
        return;
    }
    free(consultaF3);

    MYSQL_RES *resultado4 = mysql_store_result(conexion);
    if (resultado4 == NULL) {
        printf("No se obtuvieron resultados: %s\n", mysql_error(conexion));
        return;
    }

    MYSQL_ROW fila3;
    //Por ultimo hacemos el calculo final de lo que viene a ser la factura para así mostrar con mayor claridad todos
    //los costos.
    while ((fila3 = mysql_fetch_row(resultado4)) != NULL) {
        char *subototalF = fila3[0];   
        char *impuestoF = fila3[1];    
        char *TotalF = fila3[2];
        printf("|                                       %-57s |\n", "Final factura"); 
        printf("| Subtotal final: %-79s |\n", subototalF);
        printf("| Impuesto final: %-79s |\n", impuestoF);
        printf("| Total final: %-82s |\n", TotalF);
        printf("+------------+--------------+----------------------+-------------------+-------------+------------+\n");
        break;
    }
    free(nombre);
    return;
}


/* ============================ Menu principal de la seccion ==========================*/
/**
 * Nombre:menu_principal_generales
 * 
 * Descripcion:En si esta es el menu inicial que se tiene al ingresar en el apartado de opciones generales
 * 
 * Funcionamiento: Podremos navegar constantemente en las diferentes opciones que nos ofrece como lo son los catalogos
 la cotizacion, modificar cotizacion, facturar o volver al menu principal, aunque suene raro dentro de este menu hay otros submenus
 con muchas más opciones entonces la idea es que el usuario pueda explorar constantemente entre la distintas opciones que posee.
 * 
 * Entradas:No tiene
 * 
 * Salidas:No tiene
 * 
 */
void menu_principal_generales() {

    char opcion;
    do {
        printf("\nBienvenido al apartado de Opciones generales. \n");
        printf("Selecciona una de las siguientes opciones para continuar...\n");

        printf(">> A) Consulta de catalogo. \n");
        printf(">> B) Cotizar.\n");
        printf(">> C) Modificar cotización.\n");
        printf(">> D) Facturar.\n");
        printf(">> S) Volver al menu principal.\n");

        printf("Ingrese la letra de las seccion a la que desea ingresar: ");
        // scanf(" %c", &opcion); 
        // getchar(); 
        leerPrimerCaracter(&opcion);

        switch (opcion) {
            // ========== Consulta de catalogo.
            case 'a' :
                menu_consulta_catalogo();
                break;
            case 'A':
                menu_consulta_catalogo();
                break;

            // ========== Cotizar..
            case 'b':
                menu_cotizacion();
                break;

            case 'B':
                menu_cotizacion();

                break;

            // ========== Modificar cotización.
            case 'c' :

                menu_modificar_cotizacion();
                break;

            case 'C':
                menu_modificar_cotizacion();
                break;

            // ========== Facturar.
            case 'd':
                menu_crear_factura();
                
                break;

            case 'D':
                menu_crear_factura();
                break;      

            // ========== Salir del menu.
            case 's':
                printf("Saliendo de la seccion de opciones generales...\n");
                break;

            case 'S':
                printf("Saliendo de la seccion de opciones generales...\n");
                break;

            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    } while (opcion != 's' && opcion != 'S');

    return;
}

