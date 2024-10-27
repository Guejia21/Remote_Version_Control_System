#include "utilities.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
void manage_response(return_code response){
    printf("Respuesta: ");
    switch (response)
    {
    case MESAGGE_ERROR:
        printf("Error en el mensaje\n");
        break;
    case RECIEVE_FILE_ERROR:
        printf("Error en la recepcion de archivo\n");
        break;
    case SEND_FILE_ERROR:
        printf("Error en el envio de archivo\n");
        break;
    case VERSION_ERROR:
        printf("Error no especificado\n");
        break;
    case VERSION_CREATED:
        printf("Version creada\n");
        break;
    case VERSION_ADDED:
        printf("Version agregada\n");
        break;
    case VERSION_ALREADY_EXISTS:
        printf("Version ya existe\n");
        break;
    case FILE_ADDED:
        printf("Archivo adicionado\n");
        break;
    case VERSION_DOESNT_EXISTS:
        printf("Versión no existe\n");
        break;
    case FILE_NOT_COPIED:
        printf("Archivo no copiado\n");
        break;
    case FILE_COPIED:
        printf("Archivo copiado\n");
        break;
    case VERSION_ADDED_DB:
        printf("Archivo añadida en la BD\n");
        break;
    default:
        break;
    }
}
