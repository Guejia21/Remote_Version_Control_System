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

#define BUF_SIZE 1024

/**
 * @brief Envia un mensaje 
 * 
 * @param s Socket al que se envia el mensaje
 * @param msg Mensaje a enviar
 * @return int 0 si se envio el saludo, -1 si hubo un error
 */
int send_message(int s,char * msg);
/**
 * @brief Recibe un mensaje
 * 
 * @param s Socket del que se recibe el mensaje
 * @return int 0 si se recibio el saludo, -1 si hubo un error
 */
int recieve_message(int s, char* origen);

#endif