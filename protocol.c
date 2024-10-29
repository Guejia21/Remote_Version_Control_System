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
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "protocol.h"
#include "utilities.h"

int send_message(int s,char * msg){
    size_t msg_size = strlen(msg);
    if(write(s,&msg_size,sizeof(size_t))==-1){
        perror("Error al enviar el tamanio del mensaje");
        return 0;
    }    
    if(write(s,msg,strlen(msg))==-1){
        perror("Error al enviar el mensaje");
        return 0;
    }
    return 1;
}
int send_file_version(file_version *v, int s){
    ssize_t nsend = 0;
    ssize_t n =0;
    size_t struct_size = sizeof(file_version);
    char buffer[struct_size];
    memcpy(buffer,v,struct_size);
    while(nsend < struct_size){
        n = send(s,buffer+nsend,struct_size-nsend,0);
        if(n<0){
            perror("Error sendind to socket");
            return n;
        }
        nsend += n;
    } 
    return 1;
}
int recieve_file_version(file_version *v, int s){
    ssize_t nread = 0;
    ssize_t n = 0;
    size_t struct_size = sizeof(file_version);
    char buffer[struct_size];
    while (nread < struct_size) {
        n = recv(s, buffer + nread, struct_size - nread, 0);
        if (n < 0) {
            perror("Error reading from socket");
            return n;
        }
        nread += n;
    }
    memcpy(v, buffer, struct_size);
    return 1;
}
int recieve_message(int s, char* origen,char* buf){
    size_t msg_size;
    recv(s,&msg_size,sizeof(size_t),0);
    int numBytes;
    if((numBytes=read(s,buf,msg_size))==-1){
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
    printf("Source: %s\n",source);
    FILE *file = fopen(source,"rb");
    if(file==NULL){
        perror("Error al abrir el archivo");
        return 0;
    }
    // Primero obtenemos el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    // Enviamos primero el tamaño del archivo
    if (write(s, &fileSize, sizeof(fileSize)) != sizeof(fileSize)) {
        perror("Error al enviar tamaño del archivo");
        fclose(file);
        return 0;
    }
    char buf[BUF_SIZE];
    size_t numBytes;
    long totalSent = 0;
    //Leer el archivo y enviarlo
    while(totalSent < fileSize && (numBytes = fread(buf, 1, sizeof(buf), file)) > 0){
        if(write(s,buf,numBytes)!=numBytes){ //Enviar el archivo por el socket
            perror("Error al enviar el archivo");
            return 0;
        }
        totalSent += numBytes;
    }
    fclose(file);
    return 1;
}
int recieve_file(int s, char * destination){
    FILE *file = fopen(destination,"wb");
    if(file==NULL){
        perror("Error al abrir el archivo");
        return 0;
    }
    long fileSize;
    if (read(s, &fileSize, sizeof(fileSize)) != sizeof(fileSize)) {
        perror("Error al recibir tamaño del archivo");
        fclose(file);
        return 0;
    }
    char buf[BUF_SIZE];
    size_t  numBytes;
    long totalReceived = 0; //Contador para rastrear el progreso de la recepción del archivo
    while(totalReceived < fileSize){
        if((numBytes=read(s,buf,MIN(sizeof(buf), fileSize - totalReceived)))<=0){ //Recibir el archivo
            perror("Error al recibir el archivo"); //Con MIN se evita leer más allá del tamaño restante del archivo si se da el caso
            return 0;
        }
        if(fwrite(buf,1,numBytes,file)!=numBytes){ //Escribir el archivo
            perror("Error al escribir el archivo");
            return 0;
        }
        totalReceived += numBytes;
    }
    fclose(file);
    return 1;
}
