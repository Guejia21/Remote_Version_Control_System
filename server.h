/**
 * @file server.h
 * @author Jonathan David Guejia Burbano (jonathanguejia@unicauca.edu.co)
 * @author Jhoan David Chacón Moran (jhoanchacon@unicauca.edu.co)
 * @brief Declaración de las funciones del servidor
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include "protocol.h"
#define VERSIONS_DB "versions.db" /**< Nombre de la base de datos de versiones. */
#define VERSIONS_DIR ".versions" /**< Directorio del repositorio. */
#define VERSIONS_DB_PATH VERSIONS_DIR "/" VERSIONS_DB /**< Ruta completa de la base de datos.*/
/**
 * @brief Configura el servidor
 * 
 */
void configureServer();
/**
 * @brief Obtiene una conexión con el cliente
 * 
 * @param argv Parámetros de la línea de comandos
 * @return int Socket del cliente
 */
int getConnection(char * argv[]);
/**
 * @brief Ejecuta un comando
 * 
 * @param command Comando a ejecutar
 * @param c Socket del cliente
 * @return int 0 si la conexión se cerró, 1 si se ejecutó el comando
 */
int executeCommand(char * command, int c);

#endif