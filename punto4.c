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

    // Saltamos a los primos de acuerdo a la iteración actual
    int inicio_primo = 1;
    for (int i = 0; i < iteracion + 1; i++) {
        while (!es_primo(num)) num++;
        if (i < iteracion) {
            num++;
        }
    }
    inicio_primo = num;

    printf("Proceso hijo PID %d (iteración %d): ", getpid(), iteracion);

    while (cuenta < 2 * (iteracion + 1)) {
        if (es_primo(num)) {
            printf("%d ", num);
            suma += num;
            cuenta++;
        }
        num++;
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
