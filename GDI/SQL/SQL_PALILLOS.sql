-- ============================================
-- NUEVO ESQUEMA
-- ============================================

CREATE SCHEMA produccion_palillos;

SET search_path TO produccion_palillos;

-- ============================================
-- COMPRA
-- ============================================
CREATE TABLE compra (
    codigo          VARCHAR(10) PRIMARY KEY,
    fecha_compra    DATE NOT NULL,
    precio          CHAR(10) NOT NULL
);

-- ============================================
-- STOCK
-- ============================================
CREATE TABLE stock (
    codigo               VARCHAR(10) PRIMARY KEY,
    cantidad             INTEGER NOT NULL,
    fecha_actualizacion  DATE NOT NULL
);

-- ============================================
-- CLIENTES
-- ============================================
CREATE TABLE clientes (
    codigo      VARCHAR(10) PRIMARY KEY,
    dni         VARCHAR(9) NOT NULL UNIQUE,
    calle       CHAR(30) NOT NULL,
    numero      INTEGER NOT NULL,
    poblacion   CHAR(50) NOT NULL,
    cod_postal  CHAR(50) NOT NULL,
    email       VARCHAR(50) NOT NULL
);

-- ============================================
-- ROBOT
-- ============================================
CREATE TABLE robot (
    cod_robot             VARCHAR(10) PRIMARY KEY,
    tipo                  VARCHAR(20) NOT NULL,
    modelo                VARCHAR(20) NOT NULL,
    estado                VARCHAR(20) NOT NULL,
    fecha_mantenimiento   DATE NOT NULL,

    CONSTRAINT ck_estado_robot
        CHECK (estado IN ('activo', 'mantenimiento', 'error')),
    
    CONSTRAINT ck_tipo_robot
        CHECK (tipo IN ('ur3e', 'delta'))
    

);

-- ============================================
-- PRODUCCION
-- ============================================
CREATE TABLE produccion (
    cod_proceso   VARCHAR(10) PRIMARY KEY,
    fecha_inicio   DATE NOT NULL,
    fecha_fin      DATE NOT NULL,
    estado         VARCHAR(30) NOT NULL,

    CONSTRAINT ck_estado_produccion
        CHECK (estado IN ('en_curso', 'finalizado', 'error'))
);

-- ============================================
-- PROVEEDORES
-- ============================================
CREATE TABLE proveedores (
    codigo       VARCHAR(10) PRIMARY KEY,
    cif          VARCHAR(9) UNIQUE NOT NULL,
    calle        CHAR(30) NOT NULL,
    numero       INTEGER NOT NULL,
    poblacion    CHAR(50) NOT NULL,
    cod_postal   CHAR(50) NOT NULL,
    email        VARCHAR(50) NOT NULL,
    cod_compra   VARCHAR(10) NOT NULL,

    CONSTRAINT fk_prov_compra
        FOREIGN KEY (cod_compra)
        REFERENCES compra(codigo)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);

-- ============================================
-- TELEFONOS PROVEEDORES
-- ============================================
CREATE TABLE telefonos_prov (
    codigo     VARCHAR(10),
    telefonos  VARCHAR(9),

    PRIMARY KEY (codigo, telefonos),

    CONSTRAINT fk_tel_prov
        FOREIGN KEY (codigo)
        REFERENCES proveedores(codigo)
        ON DELETE CASCADE
);

-- ============================================
-- TELEFONOS CLIENTES
-- ============================================
CREATE TABLE telefonos_clien (
    codigo     VARCHAR(10),
    telefonos  VARCHAR(9),

    PRIMARY KEY (codigo, telefonos),

    CONSTRAINT fk_tel_clien
        FOREIGN KEY (codigo)
        REFERENCES clientes(codigo)
        ON DELETE CASCADE
);

-- ============================================
-- CANTIDAD COMPRA
-- ============================================
CREATE TABLE cantidad_compra (
    codigo     VARCHAR(10),
    cantidad   INTEGER NOT NULL,

    PRIMARY KEY (codigo, cantidad),

    CONSTRAINT fk_cantidad_compra
        FOREIGN KEY (codigo)
        REFERENCES compra(codigo)
        ON DELETE CASCADE
);

-- ============================================
-- SERIGRAFIADO
-- ============================================
CREATE TABLE serigrafiado (
    codigo        VARCHAR(10) PRIMARY KEY,
    letra         VARCHAR(10) NOT NULL,
    tamaño        VARCHAR(20) NOT NULL,
    titulo        CHAR(10) NOT NULL,
    simbolo       CHAR(20) NOT NULL,
    cod_cliente   VARCHAR(10),
    fecha_venta   DATE NOT NULL,
    cantidad      INTEGER NOT NULL,
    precio        CHAR(10) NOT NULL,
    valoracion    VARCHAR(30) NOT NULL,
    cod_robot     VARCHAR(10) NOT NULL,

    CONSTRAINT ck_letra
        CHECK (letra IN ('arial', 'times', 'robotic', 'nova')),

    CONSTRAINT ck_tamaño
        CHECK (tamaño IN ('grande', 'mediano', 'pequeño')),

    CONSTRAINT ck_valoracion
        CHECK (valoracion IN ('excelente', 'satisfecho', 'buena', 'regular', 'mala')),

    CONSTRAINT fk_ser_cliente
        FOREIGN KEY (cod_cliente)
        REFERENCES clientes(codigo)
        ON UPDATE CASCADE
        ON DELETE SET NULL,

    CONSTRAINT fk_ser_robot
        FOREIGN KEY (cod_robot)
        REFERENCES robot(cod_robot)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);

-- ============================================
-- PALILLO
-- ============================================
CREATE TABLE palillo (
    codigo              VARCHAR(10) PRIMARY KEY,
    forma               VARCHAR(30) NOT NULL,
    material_madera     VARCHAR(30) NOT NULL,
    grabado             VARCHAR(30) NOT NULL,
    cod_compra          VARCHAR(10),
    cod_stock           VARCHAR(10),
    cod_serigrafiado    VARCHAR(10),
    cod_proceso         VARCHAR(10) NOT NULL,

    CONSTRAINT ck_forma
        CHECK (forma IN ('alargado', 'ovalado', 'pequeño')),

    CONSTRAINT ck_madera
        CHECK (material_madera IN ('reciclado', 'roble', 'pino', 'cerezo')),

    CONSTRAINT ck_grabado
        CHECK (grabado IN ('estandar', 'decorativo')),

    CONSTRAINT fk_pal_compra
        FOREIGN KEY (cod_compra)
        REFERENCES compra(codigo)
        ON UPDATE CASCADE
        ON DELETE SET NULL,

    CONSTRAINT fk_pal_stock
        FOREIGN KEY (cod_stock)
        REFERENCES stock(codigo)
        ON UPDATE CASCADE
        ON DELETE SET NULL,

    CONSTRAINT fk_pal_serig
        FOREIGN KEY (cod_serigrafiado)
        REFERENCES serigrafiado(codigo)
        ON UPDATE CASCADE
        ON DELETE SET NULL,

    CONSTRAINT fk_pal_produccion
        FOREIGN KEY (cod_proceso)
        REFERENCES produccion(cod_proceso)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);

-- ============================================
-- INTERVENCION ROBOT
-- ============================================
CREATE TABLE intervencion_robot (
    cod_proceso     VARCHAR(10),
    tiempo_inicio   DATE NOT NULL,
    tiempo_fin      DATE NOT NULL,
    tarea           VARCHAR(30) NOT NULL,
    cod_robot       VARCHAR(10) NOT NULL,
    cod_produccion    VARCHAR(10) NOT NULL,

    PRIMARY KEY (cod_proceso),

    CONSTRAINT ck_tarea
        CHECK (tarea IN ('clasificar', 'transportar', 'serigrafiar', 'desechar')),

    CONSTRAINT fk_int_proceso
        FOREIGN KEY (cod_produccion)
        REFERENCES produccion(cod_proceso)
        ON DELETE CASCADE,

    CONSTRAINT fk_int_robot
        FOREIGN KEY (cod_robot)
        REFERENCES robot(cod_robot)
        ON DELETE RESTRICT
);