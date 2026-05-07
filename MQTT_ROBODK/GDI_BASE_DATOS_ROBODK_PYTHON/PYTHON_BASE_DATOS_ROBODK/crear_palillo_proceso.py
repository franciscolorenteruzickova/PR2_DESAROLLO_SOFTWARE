import psycopg
import random


def conectar():
    conn = psycopg.connect(
        dbname="palillo",
        user="postgres",
        password="postgres",
        host="localhost",
        port="5432"
    )
    conn.execute("SET search_path TO produccion_palillos;")
    return conn


def crear_proceso_y_palillo():
    conn = conectar()
    cur = conn.cursor()

    # ==========================
    # 1. CREAR PROCESO
    # ==========================
    codigo_proceso = f"PR{random.randint(1000, 9999)}"
    estado = random.choice(["en_curso", "finalizado", "error"])

    sql_proceso = """
    INSERT INTO produccion (cod_proceso, fecha_inicio, fecha_fin, estado)
    VALUES (%s, %s, %s, %s)
    """

    cur.execute(sql_proceso, (
        codigo_proceso,
        "2024-01-01",
        "2024-01-02",
        estado
    ))

    # ==========================
    # 2. CREAR PALILLO ASOCIADO
    # ==========================
    codigo_palillo = f"PL{random.randint(1000, 9999)}"

    sql_palillo = """
    INSERT INTO palillo (codigo, forma, material_madera, grabado, cod_proceso)
    VALUES (%s, %s, %s, %s, %s)
    """

    cur.execute(sql_palillo, (
        codigo_palillo,
        random.choice(["alargado", "ovalado", "pequeño"]),
        random.choice(["roble", "pino", "cerezo", "reciclado"]),
        "estandar",
        codigo_proceso   # 👈 MISMO PROCESO
    ))

    conn.commit()

    cur.close()
    conn.close()

    print(f"✔ Proceso creado: {codigo_proceso}")
    print(f"✔ Palillo creado: {codigo_palillo}")