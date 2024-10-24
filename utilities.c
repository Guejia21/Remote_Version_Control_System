#include "utilities.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * @brief Divide un comando en tokens
 * 
 * @param command Comando a dividir
 * @param argc Número de tokens
 * @return char** Arreglo de tokens
 */
char **split_command(const char *command, int *argc){
    char *str_copy = strdup(command);  // Copiar el comando para no modificar el original
    char *token = strtok(str_copy, " "); // Obtener el primer token
    *argc = 0; // Inicializamos el contador de tokens

    // Arreglo con un tamaño máximo de 3 posiciones
    char **argv = malloc(3 * sizeof(char *));

    // Tokenizar el comando y almacenar hasta 3 tokens
    while (token != NULL && *argc < 3) {
        argv[*argc] = strdup(token);
        (*argc)++;
        token = strtok(NULL, " ");
    }
    //liberar la memoria de la copia del comando
    free(str_copy);

    return argv;
}
