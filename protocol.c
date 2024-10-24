/**
 * @file protocol.c
 * @author Jonathan David Guejia Burbano (jonathanguejia@unicauca.edu.co)
 * @author Jhoan David Chacón Moran (jhoanchacon@unicauca.edu.co)
 * @brief Implementación del protocolo de comunicación
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "protocol.h"

int send_message(int s,char * msg){
    if(write(s,msg,strlen(msg))==-1){
        perror("Error al enviar el saludo");
        return 0;
    }
    return 1;
}
int recieve_message(int s, char* origen,char* buf){
    int numBytes;
    if((numBytes=read(s,buf,BUF_SIZE-1))==-1){
        perror("Error al recibir el saludo");
        return -1;
    }
    if(numBytes == 0) {
        // El cliente ha cerrado la conexión
        printf("El cliente ha cerrado la conexión\n");
        return 0;
    }
    buf[numBytes]='\0';
    if(strcmp(origen,"Cliente")==0 && strncmp(buf,"exit",8)==0){
        printf("El cliente ha decidido cerrar la conexión\n");
        return 0;
    }
    printf("%s: %s",origen,buf);
    return 1;
}
