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
#include "utilities.h"
/**
 * @brief Funcion para manejar adecuadaente las señales SIGINT y SIGTERM
 * 
 * @param signal Señal recibida
 */
void handle_signal(int signal);
/**
* @brief Imprime la ayuda
*/
void usageClient();
int terminated = 0;/*Bandera que determina la ejecución del proceso*/

int main(int argc,char * argv[]){
    //0. Instalar los manejadores para SIGINT, SIGTERM
    signal(SIGINT,handle_signal);
    signal(SIGTERM,handle_signal);
    //Se comprueba que se ingresaron los argumentos correctos
    if(argc != 3){
        usageClient();
        exit(EXIT_FAILURE);
    }
    int c = set_connection(argv);
    while(!terminated){
        char* command = read_command();
        if(!send_message(c,command) || EQUALS(command,"exit\n")){
            break;
        }
        if(!get_response(c)) continue;
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
void usageClient(){
    printf("Uso: ./rversions <ip> <puerto>\n");
    printf("ip: Dirección IP del servidor\n");
    printf("puerto: Puerto en el que el servidor estará escuchando\n");
    printf("Ejemplo: ./rversions 192.0.0.0 1234\n");
}