create database puntoVenta;
use puntoVenta;
create table Negocio(
	NombreLocal varchar(30) not null ,
    Telefono varchar(8) not null,
    CedulaJuridica varchar(14) not null primary key,
    HorarioAtencion varchar(40) not null,
    NumSecuencial int not null
);
insert into Negocio(NombreLocal,Telefono,CedulaJuridica,HorarioAtencion,NumSecuencial)
values
("Los tilines company","85425308","3002398412","Lunes a domingo de 7am a 10pm",1);
create table Usuario( 
	NombreUsuario varchar(30),
    ContraseñaUsuario varchar(100)
);
insert into Usuario(NombreUsuario,ContraseñaUsuario) values ('Foka', SHA2('ADMIN', 256));

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
	IdCotizacion int auto_increment primary key ,
    Cliente varchar(30),
    EstadoCotizacion varchar(30)

);

create table CotizacionDetalle (
	IdCotizacion int,
    IdProducto varchar(10),
    Cantidad int,
    PrecioXunidad float,
    primary key(IdCotizacion, IdProducto),
    constraint fk_IDCotizacion foreign key (IdCotizacion) references Cotizacion(IdCotizacion),
    constraint fk_IDProducto foreign key (IdProducto) references Productos(IdProducto)
);

create table Factura(
	IdFactura int auto_increment primary key,
    IdCotizacion int unique not null,
	fechaHora datetime not null,
    SubTotal float not null,
    Impuesto float not null,
    Total float	not null,
    NombreCliente varchar(30) not null,
    constraint fk_FIDCotizacion foreign key (IdCotizacion) references Cotizacion(IdCotizacion)
);
use puntoVenta;
create view verCatalogo as
select
	p.IdProducto,
    p.Nombre,
	(p.precio -(p.precio *0.13)) as PrecioSinIva,
    p.Cantidad,
    fp.Descripcion AS DescripcionFamilia
from Productos as p
JOIN FamiliaProductos AS fp ON p.IdFamilia = fp.IdFamilia;	


use puntoVenta;
create view CantidadCotizacionesPendientes as
select
	count(c.EstadoCotizacion) as cantidadEstados
from Cotizacion as c
	where EstadoCotizacion = 'Pendiente';
    
use puntoVenta;
create view CantidadCotizacionesFacturadas as
select
	count(c.EstadoCotizacion) as cantidadEstados
from Cotizacion as c
	where EstadoCotizacion = 'Facturado';
    
    
-- Para sacar el promedio lo que hago es sumar todos los totales y dividirlos por la cantidad de facturas que se han creado    
use puntoVenta;
create view PromedioTotal as
select
	sum(f.Total / f.IdFactura) as Totalpromedio
from Factura as f;

 -- select * from PromedioTotal 


use puntoVenta;
create view Top5ProductosVendidos as
select 
	p.Nombre,
	sum(cd.cantidad) as cantidadVendida
from 
	CotizacionDetalle as cd
join 
	Cotizacion as c on cd.IdCotizacion = c.IdCotizacion
join
	Productos as p on cd.IdProducto = p.IdProducto
where 
	c.EstadoCotizacion = 'Facturado'
group by
	p.Nombre
order by 
	cantidadVendida desc
limit 5;

 -- select * from Top5ProductosVendidos

use puntoVenta;
create view TopProductoFamilia as
select
	Descripcion,
    Nombre,
    cantidadVendida
from (
select 
	fp.Descripcion,
    p.Nombre,
	sum(cd.cantidad) as cantidadVendida,
	row_number() over (partition by fp.Descripcion order by sum(cd.cantidad) desc) as ranking
from 
	CotizacionDetalle as cd
join 
	Cotizacion as c on cd.IdCotizacion = c.IdCotizacion
join
	Productos as p on cd.IdProducto = p.IdProducto
join
	FamiliaProductos as fp on p.IdFamilia = fp.IdFamilia
where 
	c.EstadoCotizacion = 'Facturado'
group by
	fp.Descripcion,
    p.Nombre
) as subquery where ranking = 1;

 --   select * from TopProductoFamilia
  
  
  
  
  use puntoVenta;
create view MontoTopFamilia as
select
	Descripcion,
    Monto
from (
select 
	fp.Descripcion,
	sum(f.Total) as Monto,
	row_number() over (partition by fp.Descripcion order by sum(f.Total) desc) as ranking
from 
	Factura as f
join 
	Cotizacion as c on f.IdCotizacion = c.IdCotizacion
join
	CotizacionDetalle as cd on c.IdCotizacion = cd.IdCotizacion
join
	Productos as p on cd.IdProducto = p.IdProducto
join
	FamiliaProductos as fp on p.IdFamilia = fp.IdFamilia
where 
	c.EstadoCotizacion = 'Facturado'
group by
	fp.Descripcion,
    f.Total
) as subquery where ranking = 1;

--  select * from MontoTopFamilia
   -- select * from CantidadCotizacionesFacturadas

    
-- Se crea un trigger para que cuando el usuario en el inventario ingrese una cantidad que haga que la cantidad del producto
-- Se vuelva negativa envie un mensaje de error, esto por cada actualización de producto.
-- drop trigger actualizadorInventario
delimiter $$
use puntoVenta$$
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
delimiter ;






-- drop view verCatalogo esto es para quitarla
-- Falta agregar una descripcion en la base, en si se le asigna un alias a la tabla
-- De forma que no se tenga que estar escribiendo constantemente su nombre y permitiendo acceder a sus tablas

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

-- Esto se hace porque los archivos que vienen importados desde el txt vienen sucios o sea con un salto de linea o caracter de finalizacion
-- set SQL_SAFE_UPDATES = 0;
-- update FamiliaProductos
-- set Descripcion = trim(REPLACE(REPLACE(Descripcion, CHAR(13), ''), CHAR(10), ''));
-- set SQL_SAFE_UPDATES = 1;

    
    
DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE obtenerProductoPorID(
    IN idProdu varchar(20)
)
BEGIN
    SELECT
        p.IdProducto,
        p.Nombre,
        p.Precio AS PrecioSinIva,
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



DELIMITER $$
use puntoVenta$$
create procedure mostrarDetalleCotizacion(
    in IdCotizacion int
)
begin
    select
        cd.IdProducto,
        p.Nombre,
        fp.Descripcion,
        cd.PrecioXunidad,
        cd.Cantidad
    from 
        CotizacionDetalle cd
    join
        Productos p on cd.IdProducto = p.IdProducto
    join
        FamiliaProductos fp on p.IdFamilia = fp.IdFamilia
    where 
        cd.IdCotizacion = IdCotizacion;
end $$
DELIMITER ;



-- drop procedure eliminarLineaDetalle
DELIMITER $$
use puntoVenta$$
CREATE PROCEDURE eliminarLineaDetalle(
	in nombreProdu varchar(20)
)
begin
	delete from CotizacionDetalle cd where IdProducto in (
		select IdProducto from Productos
        where Nombre = nombreProdu
    ); 
end $$
DELIMITER ;



-- Verifica si hay suficiente cantidad en stock antes de realizar la cotizacion
-- delimiter $$
-- create trigger verificaInventario
-- before insert on CotizacionDetalle
-- for each row 
-- begin
--	declare cantidadDisponible int;
--    select cantidad into cantidadDisponible from Productos
--    where IdProducto = new.IdProducto;
    
--    if new.Cantidad > cantidadDisponible then
--		signal sqlstate '45000' 
--        set MESSAGE_TEXT = 'Error: No hay suficientes productos en stock';
--	end if;
-- end$$
-- delimiter;


DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE eliminarDetalleCotizacion(
    IN idProdu VARCHAR(40),
    in idCot int
)
BEGIN
    delete from CotizacionDetalle  
    where IdProducto = idProdu and IdCotizacion = idCot;
END $$
DELIMITER ;
-- call eliminarDetalleCotizacion("Prod1",1)



DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE facturaFin(
    in idCot int,
    IN Cliente VARCHAR(40)
)
BEGIN
	select 
    p.Nombre, 
    cd.Cantidad, 
    cd.PrecioXunidad,
    (cd.Cantidad * cd.PrecioXunidad) as subtotal,
    (cd.Cantidad * cd.PrecioXunidad * 0.13) as impuesto,
    ((cd.Cantidad * cd.PrecioXunidad) + (cd.Cantidad * cd.PrecioXunidad * 0.13)) as total
    from 
		CotizacionDetalle cd
    join
		Cotizacion as c on cd.IdCotizacion = c.IdCotizacion
	join
        Productos as p on cd.IdProducto = p.IdProducto
    where c.Cliente = Cliente and cd.IdCotizacion = idCot;
END $$
DELIMITER ;

-- call facturaFin(1,'Edutec');
-- Este seria en caso de querer solo mostrar los totales
DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE facturaFinDinero(
    in idCot int,
    IN Cliente VARCHAR(40)
)
BEGIN
	select 
        SUM(cd.Cantidad * cd.PrecioXunidad) AS SubtotalFinal,  
        SUM(cd.Cantidad * cd.PrecioXunidad * 0.13) AS ImpuestoFinal,  
        SUM((cd.Cantidad * cd.PrecioXunidad) + (cd.Cantidad * cd.PrecioXunidad * 0.13)) AS TotalFinal  
    from 
		CotizacionDetalle cd
    join
		Cotizacion as c on cd.IdCotizacion = c.IdCotizacion
	join
        Productos as p on cd.IdProducto = p.IdProducto
    where c.Cliente = Cliente and cd.IdCotizacion = idCot;
END $$
DELIMITER ;

-- Validacion para corroborar si hay algún ID de familia agregado previamente a su inserción
delimiter $$
use puntoVenta$$
create trigger DuplicadosFamilia
before insert on FamiliaProductos
for each row 
begin
	declare nuevoID varchar(10);
    set nuevoID = new.IdFamilia;
	if exists (select 1 from FamiliaProductos where IdFamilia = nuevoID) then 
		signal sqlstate '45000'
        set MESSAGE_TEXT = 'Error: No se pueden repetir identificadores de familias';
	end if;
end$$
delimiter ;

-- insert into FamiliaProductos(IdFamilia,Descripcion) Values ('Fam1','Enlatados'); prueba



-- Validacion para corroborar si hay algún ID de productos agregado previamente a su inserción
delimiter $$
use puntoVenta$$
create trigger DuplicadosProductos
before insert on Productos
for each row 
begin
	declare nuevoID varchar(10);
    set nuevoID = new.IdProducto;
	if exists (select 1 from Productos where IdProducto = nuevoID) then 
		signal sqlstate '45000'
        set MESSAGE_TEXT = 'Error: No se pueden repetir identificadores de productos';
	end if;
end$$
delimiter ;

-- Esta es la validación para cuando se quiere eliminar un productos ya facturado o cotizado 
delimiter $$
use puntoVenta$$
create trigger revisaEliminarProductos
before delete on Productos
for each row 
begin

	declare nuevoID varchar(10);
    set nuevoID = old.IdProducto;

	if exists (

        SELECT 1
        FROM CotizacionDetalle AS cd
        JOIN Cotizacion AS c ON cd.IdCotizacion = c.IdCotizacion
        WHERE cd.IdProducto = nuevoID AND c.EstadoCotizacion = 'Pendiente'
    
    ) then 
		signal sqlstate '45000'
        set MESSAGE_TEXT = 'Error: No se puede eliminar un producto ya cotizado o facturado';
	end if;
end$$
delimiter ;
-- DELETE FROM Productos WHERE IdProducto = 'Prod3';
-- -- DROP TRIGGER revisaEliminarProductos;






delimiter $$
USE puntoVenta$$
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
delimiter ;

-- >> Vista para ver todas las facturas realizadas.
-- Uso: selec * from verFacturas;
create view verFacturas as
select
	f.IdFactura,
    f.fechaHora,
    f.SubTotal,
    f.Total
from Factura as f;

-- >> Procedure para optener los datos completos de una facturas (Se excluyen los datos de los productos).
DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE verDatosFacturaEspecifica(
    IN idFactura INT
)
BEGIN
    select
        f.IdFactura,
        f.IdCotizacion,
        f.fechaHora,
        f.SubTotal,
        f.Impuesto,
        f.Total
    from Factura as f
    WHERE
        f.idFactura = idFactura;
END $$
DELIMITER ;
