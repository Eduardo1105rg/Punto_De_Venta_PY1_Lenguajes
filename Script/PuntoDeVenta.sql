create database puntoVenta;
use puntoVenta;
create table Negocio(
	NombreLocal varchar(30) not null ,
    Telefono varchar(8) not null,
    CedulaJuridica varchar(14) not null primary key,
    HorarioAtencion varchar(40) not null,
    NumSecuencial int not null
);

create table Usuario( 
	NombreUsuario varchar(30),
    ContraseñaUsuario varchar(30)
);


create table FamiliaProductos(
	IdFamilia varchar(10) primary key,
    Descripcion varchar(40) not null
);

create table Productos(
	IdProducto varchar(10) primary key,
    Nombre varchar(20) not null,
    IdFamilia varchar(10) not null,
    Costo float not null,
    Precio float not null,
    Cantidad int not null,
    
    constraint fk_IDFamilia foreign key (IdFamilia) references FamiliaProductos(IdFamilia)
);


create table Cotizacion (
	IdCotizacion varchar(15) primary key,
    Cliente varchar(30),
    EstadoCotizacion varchar(30)

);

create table CotizacionDetalle (
	IdCotizacion varchar(15),
    IdProducto varchar(10),
    Cantidad int,
    PrecioXunidad float,
    primary key(IdCotizacion, IdProducto),
    constraint fk_IDCotizacion foreign key (IdCotizacion) references Cotizacion(IdCotizacion),
    constraint fk_IDProducto foreign key (IdProducto) references Productos(IdProducto)
);

create table Factura(
	IdFactura varchar(15) primary key,
    IdCotizacion varchar(15) unique not null,
	fechaHora datetime not null,
    SubTotal float not null,
    Impuesto float not null,
    Total float	not null,
    constraint fk_FIDCotizacion foreign key (IdCotizacion) references Cotizacion(IdCotizacion)
);


-- Se crea un trigger para que cuando el usuario en el inventario ingrese una cantidad que haga que la cantidad del producto
-- Se vuelva negativa envie un mensaje de error, esto por cada actualización de producto.
-- drop trigger actualizadorInventario
delimiter $$
create trigger actualizadorInventario
before update on Productos
for each row 
begin
	declare cantidadTotal int;
    set cantidadTotal = new.Cantidad + old.Cantidad;
    
    if cantidadTotal < 0 then
		signal sqlstate '45000' 
        set MESSAGE_TEXT = 'Error: La cantidad no puede ser negativa';
	else
        set new.Cantidad = cantidadTotal;
	end if;
end$$
delimiter;


-- drop view verCatalogo esto es para quitarla
-- Falta agregar una descripcion en la base, en si se le asigna un alias a la tabla
-- De forma que no se tenga que estar escribiendo constantemente su nombre y permitiendo acceder a sus tablas
use puntoVenta;
create view verCatalogo as
select
	p.IdProducto,
    p.Nombre,
	(p.precio -(p.precio *0.13)) as PrecioSinIva,
    p.Cantidad
    fp.Descripcion AS DescripcionFamilia
from Productos as p
JOIN FamiliaProductos AS fp ON p.IdFamilia = fp.IdFamilia;	
-- select * from verCatalogo Este es para ver la consulta



-- drop procedure verCatalogoFiltro;
-- Este procedimiento almacenado lo que hace es revisar una descripcion(Nombre del producto),
-- con el que hacemos una union por medio del id de la familia y del id del producto por la pk y fk
-- que luego comparamos las descripciones en el where y si estan son correctas nos devuelve el select
-- de aquellos productos que cumplan la condición
DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE verCatalogoFiltro(
    IN descripcion VARCHAR(40)
)
BEGIN
    SELECT
        p.IdProducto,
        p.Nombre,
        (p.Precio - (p.Precio * 0.13)) AS PrecioSinIva,
        p.Cantidad,
        fp.Descripcion AS DescripcionFamilia
    FROM 
        Productos p
    JOIN
        FamiliaProductos AS fp ON p.IdFamilia = fp.IdFamilia
    WHERE
        fp.Descripcion = descripcion;
END $$
DELIMITER ;


-- call verCatalogoFiltro('Armas'); un ejemplo de prueba


DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE obtenerProductoPorID(
    IN idProdu VARCHAR(40)
)
BEGIN
    SELECT
        p.IdProducto,
        p.Nombre,
        (p.Precio - (p.Precio * 0.13)) AS PrecioSinIva,
        p.Cantidad,
        fp.Descripcion AS DescripcionFamilia
    FROM 
        Productos p
    JOIN
        FamiliaProductos AS fp ON p.IdFamilia = fp.IdFamilia
    WHERE
        p.IdProducto = idProdu;
END $$
DELIMITER ;


-- CALL obtenerProductoPorID('Prod1');


-- Esto se hace porque los archivos que vienen importados desde el txt vienen sucios o sea con un salto de linea o caracter de finalizacion
-- set SQL_SAFE_UPDATES = 0;
-- update FamiliaProductos
-- set Descripcion = trim(REPLACE(REPLACE(Descripcion, CHAR(13), ''), CHAR(10), ''));
-- set SQL_SAFE_UPDATES = 1;

    
    

