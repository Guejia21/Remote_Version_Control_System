/**
 * @file protocol.h
 * @author Jonathan David Guejia Burbano (jonathanguejia@unicauca.edu.co)
 * @author Jhoan David Chacón Moran (jhoanchacon@unicauca.edu.co)
 * @brief Interfaz del protocolo de comunicación
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "utilities.h"
#define BUF_SIZE 1024

/**
 * @brief Envia un mensaje 
 * 
 * @param s Socket al que se envia el mensaje
 * @param msg Mensaje a enviar
 * @return int 1 si se envio el saludo, 0 si hubo un error
 */
int send_message(int s,char * msg);
/**
 * @brief Recibe un mensaje
 * 
 * @param s Socket del que se recibe el mensaje
 * @param origen Origen del mensaje
 * @param buf Buffer donde se almacena el mensaje
 * @return int 1 si se recibio el saludo, 0 si hubo un error
 */
int recieve_message(int s, char* origen,char* buf);
/**
 * @brief Envia una versión de un archivo
 * 
 * @param v Versión del archivo
 * @param s Socket al que se envia la versión
 * @return int 1 si se envio la versión, 0 si hubo un error
 */
int send_file_version(file_version *v, int s);
/**
 * @brief Recibe una versión de un archivo
 * 
 * @param v Versión del archivo
 * @param s Socket del que se recibe la versión
 * @return int 1 si se recibio la versión, 0 si hubo un error
 */
int recieve_file_version(file_version *v, int s);
/**
 * @brief Envia un archivo
 * 
 * @param s Socket al que se envia el archivo
 * @param filename Nombre del archivo
 * @return int 1 si se envio el archivo, 0 si hubo un error
 */
int send_file(int s, char * filename);  
/**
 * @brief Recibe un archivo
 * 
 * @param s Socket del que se recibe el archivo
 * @param file Archivo donde se almacena el archivo
 * @return int 1 si se recibio el archivo, 0 si hubo un error
 */
int recieve_file(int s, char * destination);
#endif