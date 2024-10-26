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
#include <sys/stat.h>
/**
 * @brief Verifica si una version de un archivo ya existe
 * 
 * @param filename Nombre del archivo
 * @param hash hash del archivo (con este se realiza la verificación)
 * @return return_code Resultado de la operacion
 */
return_code version_exists(char * filename, char * hash);

void configureServer(){
    struct stat s;

	//Crear el directorio ".versions/" si no existe
    #ifdef __linux__
	    mkdir(VERSIONS_DIR, 0755);
    #elif _WIN32
	    mkdir(VERSIONS_DIR);
    #endif

	// Crea el archivo .versions/versions.db si no existe
	if (stat(VERSIONS_DB_PATH, &s) != 0) {
		creat(VERSIONS_DB_PATH, 0755);
	}
}

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
int executeCommand(char * command, int c){
    int argc=0;
    char ** argv = split_command(command,&argc);
    if(EQUALS(argv[0],"add")){
        if(!send_message(c,"Verificando si el archivo existe...")){
            return 0;
        };
        file_version v;
        //Se recibe la versión del archivo
        if(!recieve_file_version(&v,c)){
            return 0;
        }
        if(!send_message(c,"Descriptor del archivo recibido, verificando si ya existe una version con el mismo hash...")){
            return 0;
        }
        //Se verifica si ya existe una versión con el mismo hash
        int resultado = version_exists(v.filename, v.hash);
	    if(resultado==VERSION_ERROR)return VERSION_ERROR;
	    if(resultado == VERSION_ALREADY_EXISTS) {
            if(!send_message(c,"Ya existe una versión con el mismo hash")){
                return 0;
            }
            return VERSION_ALREADY_EXISTS;
        }
        if(!send_message(c,"Recibiendo archivo...")){
            return 0;
        }
        char dst_filename[PATH_MAX];
	    snprintf(dst_filename, PATH_MAX, "%s/%s", VERSIONS_DIR, v.hash);
        if(!recieve_file(c,dst_filename)) return 0;
        if(!send_message(c,"Arhivo recibido...")) return 0;
        

    }
}
return_code version_exists(char * filename, char * hash) {
	FILE *file;
	file = fopen(VERSIONS_DB_PATH,"r"); //Abre la BD
	if(file == NULL){
        perror("Error al abrir la base de datos");
        return VERSION_ERROR; 
    } 
	file_version r; //Estructura que recibe cada archivo guardado en la BD
	while(!feof(file)){ //Se lee todo el archivo
		if(fread(&r,sizeof(file_version),1,file)!=1) break; //Se lee un solo elemento de tamaño (file_version)
		if(EQUALS(r.filename,filename) && EQUALS(r.hash,hash)) return VERSION_ALREADY_EXISTS;//Si existe un archivo guardado con el mismo nombre y hash, significa que ya existe
	}
	return VERSION_DOESNT_EXISTS;
}
