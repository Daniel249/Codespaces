#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
void generar_pares(int inf, int sup, int stop) {
    pid_t pid;
    int status;

    printf("Proceso padre (PID: %d): Generando números pares...\n", getpid());
    bool flag = false;
    // Generar números pares hasta el stop
    for (int i = inf; i <= sup; i++) {
        if (i % 2 == 0) {
            

            // El padre crea el hijo solo cuando llega a un número par <= stop
            if (i >= stop && !flag) {
                flag = true;
                pid = fork(); // Crear el proceso hijo

                if (pid < 0) {
                    perror("Error al crear el proceso hijo");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // Código del hijo: generar números impares
                    printf("Hijo (%d): Generando números impares...\n", getpid());
                    for (int j = inf; j <= sup; j++) {
                        if (j % 2 != 0) {
                            printf("Hijo (%d): %d\n", getpid(), j);
                        }
                    }
                    // El hijo devuelve el último valor impreso + 100 en el estado de salida
                    exit(sup + 100);
                } else {
                    // El padre espera que el hijo termine de generar los números impares
                    wait(&status);
                    if (WIFEXITED(status)) {
                        int last_value = WEXITSTATUS(status);
                        printf("Padre (%d): El hijo terminó con el valor %d\n", getpid(), last_value);
                    } else {
                        fprintf(stderr, "Error: El hijo terminó con un error\n");
                    }
                }
            } else {
                printf("Padre (%d): %d\n", getpid(), i);
            }
        }
    }

    // Continuar generando números pares después de que el hijo termine
    for (int i = stop + 2; i <= sup; i++) {
        if (i % 2 == 0) {
            printf("Padre (%d): %d\n", getpid(), i);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <inf> <sup> <stop>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int inf = atoi(argv[1]);
    int sup = atoi(argv[2]);
    int stop = atoi(argv[3]);

    // Verificar si los parámetros cumplen con las condiciones
    if (inf >= stop || stop >= sup) {
        fprintf(stderr, "Error: Debe cumplirse que inf < stop < sup\n");
        exit(EXIT_FAILURE);
    }

    // Llamar a la función para generar pares
    generar_pares(inf, sup, stop);

    return 0;
}
