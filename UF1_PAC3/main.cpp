#include <iostream>
#include <vector>
#include <unistd.h>  // Para fork()
#include <sys/wait.h>  // Para wait()
#include <cstdlib>  // Para rand() y srand()
#include <ctime>  // Para time()

using namespace std;

vector<string> nombres;  // Variable global para nombres

int main() {
    int N;
    cout << "Introduce el número de procesos que quieres crear: ";
    cin >> N;

    vector<pid_t> pids(N);  // Array para almacenar los PIDs de los hijos

    srand(time(0));  // Inicializar la semilla para números aleatorios

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();  // Crear proceso

        if (pid < 0) {  // Error al crear el proceso
            cerr << "Error al crear el proceso." << endl;
            return 1;
        }

        if (pid == 0) {  // Código del proceso hijo
            string nombre;
            cout << "Proceso hijo " << i + 1 << ": Introduce un nombre: ";
            cin >> nombre;

            nombres.push_back(nombre);  // Añadir nombre al array global

            // Seleccionar aleatoriamente un nombre del array
            int index_aleatorio = rand() % nombres.size();
            cout << "Proceso hijo " << i + 1 << ": He seleccionado a " << nombres[index_aleatorio] << " del array." << endl;

            _exit(0);  // Terminar el proceso hijo
        } else {
            // Almacenar el PID del proceso hijo en el array en el proceso padre
            pids[i] = pid;
        }
    }

    // Código del proceso padre
    for (int i = 0; i < N; i++) {
        // Esperar a que cada proceso hijo termine
        waitpid(pids[i], NULL, 0);
        cout << "El proceso hijo con PID " << pids[i] << " ha terminado." << endl;
    }

    cout << "El proceso padre ha terminado." << endl;
    return 0;
}
