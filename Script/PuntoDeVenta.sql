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
    constraint fk_FIDCotizacion foreign key (IdCotizacion) references Cotizacion(IdCotizacion)
);

-- En esta parte creamos una vista la cual nos mostrara el catalogo con sus precios correspondientes y sus datos necesarios
-- Del producto esto de una forma ordenada y utilizando un join para obtener la descripcion

use puntoVenta;
create view verCatalogo as
select
	p.IdProducto,
    p.Nombre,
	p.precio,
    p.Cantidad,
    fp.Descripcion AS DescripcionFamilia
from Productos as p
JOIN FamiliaProductos AS fp ON p.IdFamilia = fp.IdFamilia;	

-- Esta es para la primera estadistica en la cual revisamos la cantidad de cotizaciones que esten pendiente esto por medio
-- De una vista la cual lo que hace es contar estos estados en donde se indique como Pendiente
use puntoVenta;
create view CantidadCotizacionesPendientes as
select
	count(c.EstadoCotizacion) as cantidadEstados
from Cotizacion as c
	where EstadoCotizacion = 'Pendiente';
  
 -- De la misma forma esta realiza el mismo procedimiento revisando el estado de la cotizacion y si esta en Facturado
 -- Lo cuenta y pues este se devuelve
use puntoVenta;
create view CantidadCotizacionesFacturadas as
select
	count(c.EstadoCotizacion) as cantidadEstados
from Cotizacion as c
	where EstadoCotizacion = 'Facturado';
    
    
-- Estadistica, esta la hago por medio de una vista con la cual utilizo todos los totales de la factura y los resto por los que tiene
-- El id de su factura de esta forma logro sacar el promedio
use puntoVenta;
create view PromedioTotal as
select
	sum(f.Total / f.IdFactura) as Totalpromedio
from Factura as f;


-- Reviso los productos que ya esten facturados y dependiendo de las cantidades de estos pues si coinciden en los mas altos
-- Se van agregando esto con su nombre y con su cantidad, de forma que le sea más facil al usuario ver
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


-- Muestro el producto top por familia si es que hay  por cada fila de estos lo que hago es que el mismo los ponga en un top
-- Entonces al ordenarlos ya no tengo que recorrerlos si no que el lo hace automaticamente y yo solo utilizo este ranking para
-- llamarlo
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

  
  
  
-- Creo una vista para ver el monto top por familia de igual forma por cada fila genero un ranking asi es más fácil que la familia
-- clasifique al que más genero y así solo lo llamo al final en su ordenamiento por el top de ranking
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



    
-- Se crea un trigger para que cuando el usuario en el inventario ingrese una cantidad que haga que la cantidad del producto
-- Se vuelva negativa envie un mensaje de error, esto por cada actualización de producto.
-- drop trigger actualizadorInventario
-- delimiter $$
-- use puntoVenta$$
-- create trigger actualizadorInventario
-- before update on Productos
-- for each row 
-- begin
--	declare cantidadTotal int;
--    set cantidadTotal = new.Cantidad + old.Cantidad;
    
--    if cantidadTotal < 0 then
--		signal sqlstate '45000' 
 --       set MESSAGE_TEXT = 'Error: La cantidad no puede ser negativa';
--	else
--        set new.Cantidad = cantidadTotal;
--	end if;
-- end$$
-- delimiter ;



-- drop trigger actualizadorInventario
 -- De esta forma puede funcionar porque no actualiza directamente la cantidad de productos
 -- esto ya tocaria hacerlo manual
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
	end if;
 end$$
 delimiter ;


DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE actualizaStockC(
    IN idProd VARCHAR(40),
    IN cantidad INT
)
BEGIN
    UPDATE Productos
    SET Cantidad = Cantidad + cantidad
    WHERE IdProducto = idProd;
END$$

DELIMITER ;




-- drop view verCatalogo esto es para quitarla
-- Falta agregar una descripcion en la base, en si se le asigna un alias a la tabla
-- De forma que no se tenga que estar escribiendo constantemente su nombre y permitiendo acceder a sus tablas

-- select * from verCatalogo Este es para ver la consulta



-- drop procedure verCatalogoFiltro;
-- Este procedimiento almacenado lo que hace es revisar una descripcion(Nombre del producto),
-- con el que hacemos una union por medio del id de la familia y del id del producto por la pk y fk
-- que luego comparamos las descripciones en el where y si estan son correctas nos devuelve el select
-- de aquellos productos que cumplan la condición




-- En esta parte creamos una vista la cual nos mostrara el catalogo con sus precios correspondientes y sus datos necesarios
-- Del producto esto de una forma ordenada y utilizando un join para obtener la descripcion, solo que utilizamos un where
-- para cumplir con el filtrado.
DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE verCatalogoFiltro(
    IN descripcion VARCHAR(40)
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
        fp.Descripcion = descripcion;
END $$
DELIMITER ;




    
 -- Recibo un id en el cual selecciono los datos de un producto especifico para así mostrarlos siempre y cuando su id coincida totalmente   
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




-- Recibo un id de cotizacion en el cual muestro todos los detalles adjuntos con esta misma de forma que al usuario le salen
-- de forma más ordenada, de igual forma utilice varios join para conectar con otras tablas y que se genere bien la relación
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


-- Si me dan el nombre de un producto lo que hago es buscarlo en la cotizacion y por medio del id realizo una confirmacion con su nombre
-- y si es asi pues lo borramos
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


-- Aqui se recibe el id del producto y de la cotizacion, a partir de estos valores buscamos en la tabla cotizacion detalle
-- Si logramos ver que estos coinciden los borramos pero para esto se deben cumplir ambas condiciones
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

-- Este procedimiento lo utilizo para buscar todos los datos o el cuerpo de lo que viene a ser la factura para así mostrarle aquello
-- que deseo al usuario además de que esto se recorrera por medio de un while para que sea mejor y se actualice más constante.
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




-- Esto lo que hace es verificar si se tiene primero la cantidad necesaria de productos una vez confirmado esto procede a restarlos
-- sin embargo esto lo hacemos por medio de la cotizacion detalle y los productos como tal el stock siempre se resta a la tabla
-- Productos
DELIMITER $$
CREATE PROCEDURE VerificarYRestarStock(IN idCotizacion INT)
BEGIN

    SELECT cd.IdProducto, cd.Cantidad, p.Cantidad AS StockDisponible
    FROM CotizacionDetalle cd
    LEFT JOIN Productos p ON cd.IdProducto = p.IdProducto
    WHERE cd.IdCotizacion = idCotizacion
    AND p.Cantidad < cd.Cantidad;

    -- si hay suficiente stock actualizamos el stock
    UPDATE Productos p
    JOIN CotizacionDetalle cd ON p.IdProducto = cd.IdProducto
    SET p.Cantidad = p.Cantidad - cd.Cantidad
    WHERE cd.IdCotizacion = idCotizacion
    AND p.Cantidad >= cd.Cantidad;  -- Aseguramos que hay suficiente stock

END$$

DELIMITER ;


-- Esta es la ultima parte de la factura aqui se van a mostrar todos los gastos finales de manera que el usuario pueda observar
-- Lo que tendra que pagar o lo que pago por la factura que se le realizo, de igual forma se confirma mediante el cliente
-- y el id de la cotizacion

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


-- esta se puede ver como la Validacion para corroborar si hay algún ID de familia agregado previamente a su inserción,
-- si es que existe se envia un mensaje de error para informar al usuario
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




-- Esta se puede ver como la validacion para corroborar si hay algún ID de productos agregado previamente a su inserción
-- si es que los hay le informa al usuario por medio de un mensaje
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


-- REVISAR ESTE
-- Esta es la validación para cuando se quiere eliminar un productos ya facturado o cotizado de forma que revise si el producto
-- ya fue facturado y que por lo tanto no se pueda eliminar
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



-- con esta vista podremos observar datos especificos como el id, fecha y hora, subtotal y total
create view verFacturas as
select
	f.IdFactura,
    f.fechaHora,
    f.SubTotal,
    f.Total
from Factura as f;

-- Con este procedimiento recibimos el id de la factura y por medio de este id confirmamos si existe y si es asi mostramos datos
-- como lo son el id de factura y cotizacion, fecha y hora, subtotal, impuesto y total
DELIMITER $$
USE puntoVenta$$
CREATE PROCEDURE verDatosFacturaEspecifica(
    IN idFactura INT
)
BEGIN
    SELECT
        f.IdFactura,
        f.IdCotizacion,
        f.fechaHora,
        f.SubTotal,
        f.Impuesto,
        f.Total,
        n.NombreLocal AS NombreNegocio,
        n.Telefono AS TelefonoNegocio,
        n.CedulaJuridica AS CedulaNegocio
    FROM Factura AS f
    CROSS JOIN Negocio AS n
    WHERE f.IdFactura = idFactura;
END$$
DELIMITER ;

