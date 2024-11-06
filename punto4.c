#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

bool es_primo(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

int generar_primos(int iteracion) {
    int cuenta = 0;
    int suma = 0;
    int num = 2;  // Comenzamos desde el primer número primo
    int inicio_primo = iteracion + 1;      // Índice del primer primo para esta iteración
    int fin_primo = 2 * (iteracion + 1);   // Índice del último primo para esta iteración

    printf("Proceso hijo PID %d (iteración %d): ", getpid(), iteracion);

    for (int i = 1; i <= fin_primo; num++) {  // i cuenta el índice del primo encontrado
        if (es_primo(num)) {
            if (i >= inicio_primo) {
                printf("%d ", num);
                suma += num;
                cuenta++;
            }
            i++;
        }
    }

    printf("\n");
    return suma;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <número de hijos>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    pid_t pid;
    int status;

    for (int i = 0; i < n; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Código del proceso hijo
            int suma = generar_primos(i);
            exit(suma);  // Devolver la suma como estado de salida
        }
    }

    // Código del proceso padre
    for (int i = 0; i < n; i++) {
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            int suma_hijo = WEXITSTATUS(status);
            printf("Hijo PID %d terminó con suma: %d\n", child_pid, suma_hijo);
        }
    }

    return 0;
}
