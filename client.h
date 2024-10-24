/**
 * @file client.h
 * @author Jonathan David Guejia Burbano (jonathanguejia@unicauca.edu.co)
 * @author Jhoan David Chacón Moran (jhoanchacon@unicauca.edu.co)
 * @brief Declaración de las funciones del cliente
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/ip.h>

/**
 * @brief Establece una conexión con el servidor
 * 
 * @param argv Parámetros de la línea de comandos
 * @return int Socket conectado al servidor
 */
int set_connection(char * argv[]);
/**
 * @brief Lee y verifica un comando de la entrada estándar
 * 
 * @return char* Comando validado
 */
char * read_command();
/**
 * @brief Espera por la respuesta del servidor
 * 
 * @param c Socket conectado al servidor
 * @return int 0 si la conexión se cerró, 1 si se recibió una respuesta
 */
int get_response(int c);
#endif