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
int send_file_version(file_version *v, int s){
    if(write(s,v,sizeof(file_version))==-1){
        perror("Error al enviar la versión");
        return 0;
    }
    return 1;
}
int recieve_file_version(file_version *v, int s){
    if(read(s,v,sizeof(file_version))==-1){
        perror("Error al recibir la versión");
        return 0;
    }
    return 1;
}
int recieve_message(int s, char* origen,char* buf){
    int numBytes;
    if((numBytes=read(s,buf,BUF_SIZE-1))==-1){
        perror("Error al recibir el saludo");
        return 0;
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
    printf("%s: %s\n",origen,buf);
    return 1;
}
int send_file(int s, char * source){
    //Abrir el archivo
    FILE *file = fopen(source,"rb");
    if(file==NULL){
        perror("Error al abrir el archivo");
        return 0;
    }
    char buf[BUF_SIZE];
    int numBytes;
    //Leer el archivo y enviarlo
    while(!feof(file)){
        numBytes=fread(buf,1,BUF_SIZE,file); //Leer el archivo
        if(write(s,buf,numBytes)!=numBytes){ //Enviar el archivo por el socket
            perror("Error al enviar el archivo");
            return 0;
        }
    }
    send_message(s,"Archivo enviado");
    if(numBytes==-1){
        perror("Error al leer el archivo");
        return 0;
    }
    fclose(file);
    return 1;
}
int recieve_file(int s, char * destination){
    FILE *file = fopen(destination,"wb");
    char buf[BUF_SIZE];
    size_t  numBytes;
    //TODO: Cambiar el ciclo para que no sea infinito y se quede leyendo
    while(1){
        if((numBytes=read(s,buf,BUF_SIZE))==-1){ //Recibir el archivo por el socket
            perror("Error al recibir el archivo");
            return 0;
        }
        if(numBytes==0){
            break;
        } 
        if(fwrite(buf,1,numBytes,file)!=numBytes){ //Escribir el archivo
            perror("Error al escribir el archivo");
            return 0;
        }
    }
    if(numBytes==-1){
        perror("Error al recibir el archivo");
        return 0;
    }
    fclose(file);
    return 1;
}