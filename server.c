/**
 * @file server.c
 * @author Jonathan David Guejia Burbano (jonathanguejia@unicauca.edu.co)
 * @author Jhoan David Chacón Moran (jhoanchacon@unicauca.edu.co)
 * @brief Implementación de las funciones del servidor
 * @version 0.1
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "server.h"
#include "utilities.h"
int getConnection(char * argv[]) {
    //1. Obtener el puerto del argumento
    int puerto = atoi(argv[1]);
    //2. Crear un socket    
    int s; //Socket del servidor
    int c; //Socket del cliente  
    struct sockaddr_in addr_server; /*Direccion del servidor*/
    if((s=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }
    // 3. Asignar una dirección al socket (ip,puerto)
    memset(&addr_server,0,sizeof(struct sockaddr_in));
    addr_server.sin_family=AF_INET;
    addr_server.sin_port=htons(puerto);
    addr_server.sin_addr.s_addr=INADDR_ANY;
    bzero(&(addr_server.sin_zero),8);
    if(bind(s,(struct sockaddr *)&addr_server,sizeof(struct sockaddr_in))== -1){
        perror("Error al asignar la dirección al socket");
        exit(EXIT_FAILURE);
    }
    // 4. Poner el socket en modo de escucha
    if(listen(s,10)==-1){
        perror("Error al poner el socket en modo de escucha");
        exit(EXIT_FAILURE);
    }
    // 5. (Bloqueante)Esperar por una conexion "c" - accept
    struct sockaddr_in addr_client;
    socklen_t addr_client_len=sizeof(struct sockaddr_in);
    printf("SERVIDOR INICIADO\n");
    printf("Esperando por una conexión en el puerto %d\n",puerto);
    if((c = accept(s,(struct  sockaddr *) &addr_client,&addr_client_len))==-1){
        perror("Error al aceptar la conexión");
        exit(EXIT_FAILURE);
    }
    printf("Conexión aceptada\n");
    close(s);
    return c;
}
void executeCommand(char * command, int c){
    int argc=0;
    char ** argv = split_command(command,&argc);
    if(EQUALS(argv[0],"add")){
        send_message(c,"Verificando si el archivo existe...");
    }
}