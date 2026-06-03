"""
pr2_gdi_mongodb.py

Programa de ejemplo para acceder a MongoDB desde Python aplicado al proyecto
PR2/GDI.

El programa permite:
- Conectarse a MongoDB
- Consultar sensores, lecturas, alertas y eventos
- Insertar nuevas lecturas
- Actualizar el estado de una alerta
- Eliminar eventos
- Calcular KPI mediante consultas y aggregation framework

Base de datos esperada:
    pr2_gdi
    
Colecciones esperadas:
    sensores
    lecturas
    alertas
    eventos
"""

from pymongo import MongoClient
from datetime import datetime, timezone


URI_MONGODB = "mongodb://localhost:27017/"
NOMBRE_BD = "pr2_gdi"


def conectar():
    """Crea la conexión con MongoDB y devuelve cliente y base de datos."""
    client = MongoClient(URI_MONGODB)
    db = client[NOMBRE_BD]
    print("Conexión realizada correctamente.")
    return client, db


def mostrar_sensores(db):
    """Muestra todos los sensores registrados."""
    print("\n--- SENSORES REGISTRADOS ---")
    for sensor in db.sensores.find():
        print(sensor)


def consultar_lecturas_sensor(db):
    """Pide un sensor por teclado y muestra sus lecturas."""
    sensor_id = input("Introduce el ID del sensor (ejemplo S001): ")

    resultados = db.lecturas.find(
        {"sensor_id": sensor_id},
        {"_id": 1, "sensor_id": 1, "timestamp": 1, "valor": 1, "unidad": 1}
    )

    print(f"\n--- LECTURAS DEL SENSOR {sensor_id} ---")
    for lectura in resultados:
        print(lectura)


def consultar_alertas_activas(db):
    """Muestra las alertas activas del sistema."""
    print("\n--- ALERTAS ACTIVAS ---")
    resultados = db.alertas.find({"estado": "Activa"})

    for alerta in resultados:
        print(alerta)


def insertar_lectura(db):
    """Inserta una nueva lectura introducida por teclado."""
    sensor_id = input("ID del sensor: ")
    valor = float(input("Valor de la lectura: "))
    unidad = input("Unidad de medida: ")

    total_lecturas = db.lecturas.count_documents({})
    nuevo_id = f"L{total_lecturas + 1:03d}"

    nueva_lectura = {
        "_id": nuevo_id,
        "sensor_id": sensor_id,
        "timestamp": datetime.now(timezone.utc),
        "valor": valor,
        "unidad": unidad
    }

    db.lecturas.insert_one(nueva_lectura)
    print("Lectura insertada correctamente:")
    print(nueva_lectura)


def resolver_alerta(db):
    """Actualiza una alerta activa a estado Resuelta."""
    alerta_id = input("Introduce el ID de la alerta a resolver (ejemplo A001): ")

    resultado = db.alertas.update_one(
        {"_id": alerta_id},
        {"$set": {"estado": "Resuelta"}}
    )

    if resultado.modified_count > 0:
        print("Alerta actualizada correctamente.")
    else:
        print("No se modificó ninguna alerta. Comprueba el ID introducido.")


def eliminar_evento(db):
    """Elimina un evento concreto por su identificador."""
    evento_id = input("Introduce el ID del evento a eliminar (ejemplo E001): ")

    resultado = db.eventos.delete_one({"_id": evento_id})

    if resultado.deleted_count > 0:
        print("Evento eliminado correctamente.")
    else:
        print("No se encontró ningún evento con ese ID.")


def kpi_alertas_activas(db):
    """KPI 1: número de alertas activas."""
    total = db.alertas.count_documents({"estado": "Activa"})
    print("\nKPI 1 - Número de alertas activas")
    print(f"Resultado: {total} alertas activas")


def kpi_alertas_por_sensor(db):
    """KPI 2: número de alertas por sensor."""
    pipeline = [
        {
            "$group": {
                "_id": "$sensor_id",
                "totalAlertas": {"$sum": 1}
            }
        },
        {
            "$sort": {
                "_id": 1
            }
        }
    ]

    print("\nKPI 2 - Número de alertas por sensor")
    for doc in db.alertas.aggregate(pipeline):
        print(doc)


def kpi_valor_medio_por_sensor(db):
    """KPI 3: valor medio registrado por cada sensor."""
    pipeline = [
        {
            "$group": {
                "_id": "$sensor_id",
                "valorMedio": {"$avg": "$valor"}
            }
        },
        {
            "$sort": {
                "_id": 1
            }
        }
    ]

    print("\nKPI 3 - Valor medio por sensor")
    for doc in db.lecturas.aggregate(pipeline):
        print(doc)


def kpi_eventos_por_tipo(db):
    """KPI 4: distribución de eventos por tipo."""
    pipeline = [
        {
            "$group": {
                "_id": "$tipo",
                "cantidad": {"$sum": 1}
            }
        },
        {
            "$sort": {
                "cantidad": -1
            }
        }
    ]

    print("\nKPI 4 - Eventos por tipo")
    for doc in db.eventos.aggregate(pipeline):
        print(doc)


def kpi_total_lecturas(db):
    """KPI 5: número total de lecturas registradas."""
    total = db.lecturas.count_documents({})
    print("\nKPI 5 - Número total de lecturas registradas")
    print(f"Resultado: {total} lecturas registradas")


def mostrar_kpi(db):
    """Ejecuta todos los KPI del proyecto."""
    kpi_alertas_activas(db)
    kpi_alertas_por_sensor(db)
    kpi_valor_medio_por_sensor(db)
    kpi_eventos_por_tipo(db)
    kpi_total_lecturas(db)


def menu():
    """Muestra el menú principal."""
    print("\n========== MENÚ STICKPRINT - MONGODB ==========")
    print("1. Mostrar sensores")
    print("2. Consultar lecturas de un sensor")
    print("3. Consultar alertas activas")
    print("4. Insertar nueva lectura")
    print("5. Resolver una alerta")
    print("6. Eliminar un evento")
    print("7. Mostrar KPI")
    print("0. Salir")


def main():
    client = None

    try:
        client, db = conectar()

        while True:
            menu()
            opcion = input("Selecciona una opción: ")

            if opcion == "1":
                mostrar_sensores(db)
            elif opcion == "2":
                consultar_lecturas_sensor(db)
            elif opcion == "3":
                consultar_alertas_activas(db)
            elif opcion == "4":
                insertar_lectura(db)
            elif opcion == "5":
                resolver_alerta(db)
            elif opcion == "6":
                eliminar_evento(db)
            elif opcion == "7":
                mostrar_kpi(db)
            elif opcion == "0":
                print("Saliendo del programa.")
                break
            else:
                print("Opción no válida.")

    except Exception as error:
        print("Se ha producido un error:")
        print(error)

    finally:
        if client is not None:
            client.close()
            print("Conexión cerrada correctamente.")


if __name__ == "__main__":
    main()
