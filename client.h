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

#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "client.h"
#include "protocol.h"
#include "utilities.h"
#include "sha256.h"
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
 * @param command Comando enviado
 * @return return_code Resultado de la operacion
 */
return_code get_response(int c, char * command);

#endif