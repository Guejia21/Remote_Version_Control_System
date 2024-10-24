#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include <stdlib.h>

#define EQUALS(s1, s2) (strcmp(s1, s2) == 0) /**< Verdadero si dos cadenas son iguales.*/
/**
 * @brief Divide un comando en tokens
 * 
 * @param command Comando a dividir
 * @param argc Número de tokens
 * @return char** Arreglo de tokens
 */
char **split_command(const char *command, int *argc);

#endif