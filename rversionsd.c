#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/ip.h>
#include <signal.h>
#include "protocol.h"
#include "server.h"
/**
 * @brief Funcion para manejar adecuadaente las señales SIGINT y SIGTERM
 * 
 * @param signal Señal recibida
 */
void handle_signal(int signal);
/**
* @brief Imprime la ayuda
*/
void usageServer();

int terminated = 0;/*Bandera que determina la ejecución del proceso*/

int main(int argc,char * argv[]){
    //0. Instalar los manejadores para SIGINT, SIGTERM
    signal(SIGINT,handle_signal);
    signal(SIGTERM,handle_signal);
    //Se comprueba que se ingresaron los argumentos correctos
    if(argc != 2){
        usageServer();
        exit(EXIT_FAILURE);
    }
    int client_socket = getConnection(argv);
    char buf[BUF_SIZE];
    while(!terminated){
        //TODO: Comunicacion
        if(!recieve_message(client_socket,"Cliente",buf)){
            break;
        }
        executeCommand(buf,client_socket);
    }
    // 7. Cerrar el socket c
    printf("Cerrando sockets...\n");
    close(client_socket);
    exit(EXIT_SUCCESS);
}
void handle_signal(int signal){
    printf("Se ha recibido la señal %d\n",signal);
    terminated = 1; 
}
void usageServer(){
    printf("Uso: ./rversionsd <puerto>\n");
    printf("puerto: Puerto en el que el servidor estará escuchando\n");
    printf("Ejemplo: ./rversionsd 1234\n");

}