-- ============================================
-- COMPRA (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.compra (codigo, fecha_compra, precio)
VALUES
('C004', '2025-02-01', '1200'),
('C005', '2025-02-10', '1750'),
('C006', '2025-02-18', '2100'),
('C007', '2025-03-01', '980'),
('C008', '2025-03-05', '1340'),
('C009', '2025-03-10', '1560'),
('C010', '2025-03-15', '1890');

-- ============================================
-- STOCK (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.stock (codigo, cantidad, fecha_actualizacion)
VALUES
('S004', 650, '2025-04-05'),
('S005', 980, '2025-04-06'),
('S006', 430, '2025-04-07'),
('S007', 1200, '2025-04-10'),
('S008', 300, '2025-04-11'),
('S009', 870, '2025-04-12'),
('S010', 540, '2025-04-13');

-- ============================================
-- CLIENTES (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.clientes (codigo, dni, calle, numero, poblacion, cod_postal, email)
VALUES
('CL004', '44444444D', 'Calle Rio', 4, 'Valencia', '46001', 'cl4@mail.com'),
('CL005', '55555555E', 'Calle Mar', 5, 'Sevilla', '41001', 'cl5@mail.com'),
('CL006', '66666666F', 'Calle Bosque', 6, 'Bilbao', '48001', 'cl6@mail.com'),
('CL007', '77777777G', 'Calle Sol', 7, 'Madrid', '28001', 'cl7@mail.com'),
('CL008', '88888888H', 'Calle Luna', 8, 'Barcelona', '08001', 'cl8@mail.com'),
('CL009', '99999999J', 'Calle Estrella', 9, 'Zaragoza', '50001', 'cl9@mail.com'),
('CL010', '10101010K', 'Calle Nube', 10, 'Malaga', '29001', 'cl10@mail.com');

-- ============================================
-- TELEFONOS CLIENTES (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.telefonos_clien (codigo, telefonos)
VALUES
('CL004', '644444444'),
('CL005', '655555555'),
('CL006', '666666666'),
('CL007', '677777777'),
('CL008', '688888888'),
('CL009', '699999999'),
('CL010', '600000001');

-- ============================================
-- PROVEEDORES (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.proveedores (codigo, cif, calle, numero, poblacion, cod_postal, email, cod_compra)
VALUES
('P004', 'B45678901', 'Calle Norte', 11, 'Leon', '24001', 'prov4@mail.com', 'C004'),
('P005', 'B56789012', 'Calle Sur', 12, 'Granada', '18001', 'prov5@mail.com', 'C005'),
('P006', 'B67890123', 'Calle Este', 13, 'Zaragoza', '50001', 'prov6@mail.com', 'C006'),
('P007', 'B78901234', 'Calle Oeste', 14, 'Sevilla', '41001', 'prov7@mail.com', 'C007'),
('P008', 'B89012345', 'Calle Centro', 15, 'Madrid', '28002', 'prov8@mail.com', 'C008'),
('P009', 'B90123456', 'Calle Playa', 16, 'Valencia', '46002', 'prov9@mail.com', 'C009'),
('P010', 'B01234567', 'Calle Montaña', 17, 'Bilbao', '48002', 'prov10@mail.com', 'C010');

-- ============================================
-- TELEFONOS PROVEEDORES (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.telefonos_prov (codigo, telefonos)
VALUES
('P004', '700111111'),
('P005', '700222222'),
('P006', '700333333'),
('P007', '700444444'),
('P008', '700555555'),
('P009', '700666666'),
('P010', '700777777');

-- ============================================
-- CANTIDAD COMPRA (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.cantidad_compra (codigo, cantidad)
VALUES
('C004', 1100),
('C005', 1400),
('C006', 900),
('C007', 800),
('C008', 1200),
('C009', 1500),
('C010', 600);

-- ============================================
-- ROBOTS (AMPLIADO Y CORREGIDO CHECK)
-- ============================================
INSERT INTO produccion_palillos.robot (cod_robot, tipo, modelo, estado, fecha_mantenimiento)
VALUES
('R004', 'ur3e', 'UR3E-PRO', 'activo', '2025-03-20'),
('R005', 'delta', 'RX300', 'activo', '2025-03-22'),
('R006', 'ur3e', 'UR3E-LITE', 'error', '2025-03-25'),
('R007', 'delta', 'DX200', 'activo', '2025-03-26'),
('R008', 'ur3e', 'UR3E-MAX', 'mantenimiento', '2025-03-27'),
('R009', 'delta', 'RX500', 'activo', '2025-03-28'),
('R010', 'ur3e', 'UR3E-NEW', 'activo', '2025-03-29');

-- ============================================
-- PRODUCCION (SIN NULL Y MÁS REALISTA)
-- ============================================
INSERT INTO produccion_palillos.produccion (cod_proceso, fecha_inicio, fecha_fin, estado)
VALUES
('PR004', '2025-03-01', '2025-03-05', 'finalizado'),
('PR005', '2025-03-06', '2025-03-12', 'finalizado'),
('PR006', '2025-03-13', '9999-12-31', 'en_curso'),
('PR007', '2025-03-14', '2025-03-18', 'finalizado'),
('PR008', '2025-03-19', '9999-12-31', 'en_curso'),
('PR009', '2025-03-20', '2025-03-25', 'finalizado'),
('PR010', '2025-03-26', '9999-12-31', 'en_curso');

-- ============================================
-- SERIGRAFIADO (VARIADO Y CONSISTENTE)
-- ============================================
INSERT INTO produccion_palillos.serigrafiado
(codigo, letra, tamaño, titulo, simbolo, cod_cliente, fecha_venta, cantidad, precio, valoracion, cod_robot)
VALUES
('SE004', 'arial', 'grande', 'LogoD', '♣', 'CL004', '2025-03-01', 180, '55', 'excelente', 'R004'),
('SE005', 'times', 'mediano', 'LogoE', '♦', 'CL005', '2025-03-02', 160, '48', 'buena', 'R005'),
('SE006', 'nova', 'pequeño', 'LogoF', '♠', 'CL006', '2025-03-03', 120, '35', 'satisfecho', 'R004'),
('SE007', 'arial', 'mediano', 'LogoG', '★', 'CL007', '2025-03-04', 200, '60', 'excelente', 'R007'),
('SE008', 'robotic', 'grande', 'LogoH', '▲', 'CL008', '2025-03-05', 140, '52', 'buena', 'R008'),
('SE009', 'times', 'pequeño', 'LogoI', '■', 'CL009', '2025-03-06', 110, '40', 'regular', 'R009'),
('SE010', 'nova', 'mediano', 'LogoJ', '●', 'CL010', '2025-03-07', 190, '58', 'excelente', 'R010');

-- ============================================
-- PALILLOS (AMPLIADO)
-- ============================================
INSERT INTO produccion_palillos.palillo
(codigo, forma, material_madera, grabado, cod_compra, cod_stock, cod_serigrafiado, cod_proceso)
VALUES
('PL004', 'alargado', 'roble', 'decorativo', 'C004', 'S004', 'SE004', 'PR004'),
('PL005', 'ovalado', 'pino', 'estandar', 'C005', 'S005', 'SE005', 'PR005'),
('PL006', 'pequeño', 'cerezo', 'decorativo', 'C006', 'S006', 'SE006', 'PR006'),
('PL007', 'alargado', 'roble', 'estandar', 'C007', 'S007', 'SE007', 'PR007'),
('PL008', 'ovalado', 'pino', 'decorativo', 'C008', 'S008', 'SE008', 'PR008'),
('PL009', 'pequeño', 'cerezo', 'estandar', 'C009', 'S009', 'SE009', 'PR009'),
('PL010', 'alargado', 'roble', 'decorativo', 'C010', 'S010', 'SE010', 'PR010');

-- ============================================
-- INTERVENCION ROBOT (AMPLIADO SIN ERRORES)
-- ============================================
INSERT INTO produccion_palillos.intervencion_robot
(cod_proceso, tiempo_inicio, tiempo_fin, tarea, cod_robot, cod_produccion)
VALUES
('INT004', '2025-03-20', '2025-03-20', 'clasificar', 'R004', 'PR004'),
('INT005', '2025-03-21', '2025-03-21', 'serigrafiar', 'R005', 'PR005'),
('INT006', '2025-03-22', '2025-03-22', 'transportar', 'R006', 'PR006'),
('INT007', '2025-03-23', '2025-03-23', 'clasificar', 'R007', 'PR007'),
('INT008', '2025-03-24', '2025-03-24', 'serigrafiar', 'R008', 'PR008'),
('INT009', '2025-03-25', '2025-03-25', 'transportar', 'R009', 'PR009'),
('INT010', '2025-03-26', '2025-03-26', 'desechar', 'R010', 'PR010');