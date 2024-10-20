#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "protocol.h"
#include "client.h"

/**
 * @brief Funcion para manejar adecuadaente las señales SIGINT y SIGTERM
 * 
 * @param signal Señal recibida
 */
void handle_signal(int signal);
/**
* @brief Imprime la ayuda
*/
void usage();
int terminated = 0;/*Bandera que determina la ejecución del proceso*/

int main(int argc,char * argv[]){
    //0. Instalar los manejadores para SIGINT, SIGTERM
    signal(SIGINT,handle_signal);
    signal(SIGTERM,handle_signal);
    //Se comprueba que se ingresaron los argumentos correctos
    if(argc != 3){
        usage();
        exit(EXIT_FAILURE);
    }
    int c = setConnection(argv);
    while(!terminated){
        //TODO: Comunicacion
        break;
    }
    // 4. Cerrar el socket
    printf("Cerrando el socket\n");
    close(c);
    exit(EXIT_SUCCESS);
}
void handle_signal(int signal){
    printf("Se ha recibido la señal %d\n",signal);
    terminated = 1; 
}
void usage(){
    printf("Uso: ./rversions <ip> <puerto>\n");
    printf("ip: Dirección IP del servidor\n");
    printf("puerto: Puerto en el que el servidor estará escuchando\n");
    printf("Ejemplo: ./rversions 192.0.0.0 1234\n");
}