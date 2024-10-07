#include <iostream>
#include "../include/sqlite3.h"

using namespace std;

// Función para ejecutar un comando SQL y mostrar el resultado (en caso de ser una consulta)
static int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

int main() {
    sqlite3* DB;
    char* errorMessage;
    int exit = 0;

    // Abrir o crear la base de datos
    exit = sqlite3_open("myDataBase.db", &DB);

    if (exit) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(DB) << endl;
        return -1;
    } else {
        cout << "Conexión a la base de datos exitosa." << endl;
    }

    // Crear una tabla si no existe
    string sqlCreateTable = "CREATE TABLE IF NOT EXISTS PERSONA("
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "NOMBRE TEXT NOT NULL, "
                            "EDAD INT NOT NULL);";

    exit = sqlite3_exec(DB, sqlCreateTable.c_str(), NULL, 0, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "Error al crear la tabla: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << "Tabla creada o ya existe." << endl;
    }

    // Insertar un valor en la tabla
    string sqlInsert = "INSERT INTO PERSONA (NOMBRE, EDAD) VALUES('Oscar', 30);";
    exit = sqlite3_exec(DB, sqlInsert.c_str(), NULL, 0, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "Error al insertar datos: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << "Datos insertados correctamente." << endl;
    }

    // Consultar los datos
    string sqlSelect = "SELECT * FROM PERSONA;";
    cout << "Mostrando datos de la tabla PERSONA:" << endl;
    exit = sqlite3_exec(DB, sqlSelect.c_str(), callback, 0, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "Error al consultar datos: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    // Modificar un valor en la tabla
    string sqlUpdate = "UPDATE PERSONA SET EDAD = 31 WHERE NOMBRE = 'Oscar';";
    exit = sqlite3_exec(DB, sqlUpdate.c_str(), NULL, 0, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "Error al modificar datos: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << "Datos actualizados correctamente." << endl;
    }

    // Volver a consultar para ver los cambios
    cout << "Mostrando datos actualizados de la tabla PERSONA:" << endl;
    exit = sqlite3_exec(DB, sqlSelect.c_str(), callback, 0, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "Error al consultar datos actualizados: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    // Cerrar la base de datos
    sqlite3_close(DB);
    return 0;
}