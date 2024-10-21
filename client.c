/**
 * @file client.c
 * @author Jonathan David Guejia Burbano (jonathanguejia@unicauca.edu.co)
 * @author Jhoan David Chacón Moran (jhoanchacon@unicauca.edu.co)
 * @brief Implementación de las funciones del cliente
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
/**
 * @brief Valida si el comando ingresado es valido
 * 
 * @param command Comando ingresado
 * @return int 0 si es invalido, 1 si es valido
 */
int isValid(char * command);

int set_connection(char * argv[]) {
    //Obtener el puerto
    int puerto = atoi(argv[2]);
    //Obtener la ip
    char * ip = argv[1];
    // 1. Obtener un conector
    int c; //Socket del cliente
    struct sockaddr_in addr_server; /*Direccion del servidor*/
    if((c=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }
    
    // 2. Conectar el socket a una dirección (ip,puerto) ~ bind
    memset(&addr_server,0,sizeof(struct sockaddr_in));
    addr_server.sin_family=AF_INET;
    //Convertir la dirección ip a binario
    if (inet_aton(ip, &addr_server.sin_addr) == 0) {
        perror("Error initializing address");
        exit(EXIT_FAILURE);
    }
    addr_server.sin_port=htons(puerto);
    if(connect(c,(struct sockaddr *)&addr_server,sizeof(struct sockaddr_in))== -1){
        perror("Error al conectar el socket");
        exit(EXIT_FAILURE);
    } 
    printf("Conectado al servidor %s:%d\n",ip,puerto);    
    return c;
}
char * read_command(){
    char * command = (char *)malloc(100*sizeof(char));
    do{
        printf("Ingrese el comando: ");
        fgets(command,100,stdin);
    }while(!isValid(command));
    return command;
}
int isValid(char * command){
    char * argv[3];
    char * str_copy = strdup(command);
    char * token = strtok(str_copy," "); //Se obtiene el primer token del comando (debería ser add, get o list)
    int argc = 0; //Contador de argumentos
    while(token!=NULL){
        argv[argc] = token;
        token = strtok(NULL," ");
        argc++;
    }
    if((argc==3 && EQUALS(argv[0],"add")) || (argc==3 && (EQUALS(argv[0],"get"))) || (argc==2 && EQUALS(argv[0],"list"))|| (argc==1 && EQUALS(argv[0],"list"))){
        return 1;
    }
    printf("Comando no válido\n");
    return 0;
}