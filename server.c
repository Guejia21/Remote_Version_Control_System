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
/**
 * @brief Adiciona una nueva version de un archivo.
 *
 * @param filename Nombre del archivo.
 * @param comment Comentario de la version.
 * @param hash Hash del contenido.
 *
 * @return return_code Resultado de la operacions.
 */
return_code add_new_version(file_version * v);
/**
 * @brief Elimina el arhivo copiado en .versions
 * 
 * @param hash Nombre del archivo ubicado en .versions
 */
void removeFile(char * hash);
/**
* @brief Almacena un archivo en el repositorio
*
* @param hash Hash del archivo: nombre del archivo en el repositorio
* @param filename Nombre del archivo
* @param c Socket del cliente
*
* @return return_code Resultado de la operación
*/
return_code retrieve_file(char * hash, char * filename,int c);

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
return_code executeCommand(char * command, int c){
    //Gracias a la validación previa, se sabe que el comando es correcto
    int argc=0;
    char ** argv = split_command(command,&argc);
    if(EQUALS(argv[0],"add")){
        return add(c);
    }
    else if(EQUALS(argv[0],"get")){
        return get(c,argv);
    }
    //TODO: Implementar list
    return 0;
}
return_code add(int c){
    char buf[BUF_SIZE];
    if(!send_message(c,"Verificando si el archivo existe...")) return MESAGGE_ERROR;
    //Msj esperado Version creada...
    if(!recieve_message(c,"Client",buf)) return MESAGGE_ERROR;
    if(EQUALS(buf,"Error al crear la versión")) return VERSION_ERROR;
    file_version v;
    //Se recibe la versión del archivo
    if(!recieve_file_version(&v,c)) return RECIEVE_FILE_ERROR;
    if(!send_message(c,"Descriptor del archivo recibido, verificando si ya existe una version con el mismo hash...")) return MESAGGE_ERROR;
    //Se verifica si ya existe una versión con el mismo hash
    int resultado = version_exists(v.filename, v.hash);
	if(resultado == VERSION_ERROR) return VERSION_ERROR;
	if(resultado == VERSION_ALREADY_EXISTS) {
        if(!send_message(c,"Ya existe una versión con el mismo hash")) return MESAGGE_ERROR;
        return VERSION_ALREADY_EXISTS;
    }
    if(!send_message(c,"Recibiendo archivo...")) return MESAGGE_ERROR;
    //Se recibe el archivo
    char dst_filename[PATH_MAX];
	snprintf(dst_filename, PATH_MAX, "%s/%s", VERSIONS_DIR, v.hash);
    if(!recieve_file(c,dst_filename)) return RECIEVE_FILE_ERROR;
    if(!send_message(c,"Arhivo recibido...")) return MESAGGE_ERROR;
    if(add_new_version(&v)==VERSION_ERROR) return VERSION_ERROR;
    if(!send_message(c,"Archivo guardado en la base de datos correctamente...")) return MESAGGE_ERROR;
    return FILE_ADDED;
}
return_code get(int c, char ** argv){ //argv es [get NUMBER ARCHIVO]
    int version = atoi(argv[1]);
    //1. Abre la BD y busca el registro r que coincide con filename y version
    file_version r; 
	FILE *file;
	file = fopen(VERSIONS_DB_PATH,"r"); //Abre la BD
	if(file == NULL) return VERSION_ERROR; 
	int contador = 1;
    if(!send_message(c,"Buscando archivo...")) return MESAGGE_ERROR;
    trim_newline(argv[2]); //Se elimina el \n del final
	while(!feof(file)){ //Se lee todo el archivo
		if(fread(&r,sizeof(file_version),1,file)!=1) break; //Se lee un solo elemento de tamaño (file_version)
		if(EQUALS(r.filename,argv[2])){ //El argv[2] es el nombre del archivo, además de que en el final contiene un \n
			if(contador == version){
                if(!send_message(c,"Archivo encontrado, iniciando envio...")) return MESAGGE_ERROR;
                fclose(file);
				return retrieve_file(r.hash,argv[2],c);
			} 
			else contador++;
		}
	}
    if(!send_message(c,"Archivo no encontrado...")) return MESAGGE_ERROR;
	fclose(file);
	return VERSION_DOESNT_EXISTS;
}
return_code add_new_version(file_version * v) {
	FILE * fp;
    printf("Guardando en base de datos:\n");
    printf("Nombre: %s\n",v->filename);
    printf("Hash: %s\n",v->hash);
    printf("Comentario: %s\n",v->comment);
	fp = fopen(VERSIONS_DB_PATH,"a"); //Se abre la base de datos en modo append, es decir que se escribirá al final del archivo
    if(fp == NULL){
        perror("Error al abrir la base de datos");
        return VERSION_ERROR;
    }
	if(fwrite(v,sizeof(file_version),1,fp)!=1){
		removeFile(v->hash);
		return VERSION_ERROR;
	}
    fclose(fp); 
	return VERSION_ADDED_DB;
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
return_code retrieve_file(char * hash, char * filename,int c) {
	char src_filename[PATH_MAX];
	snprintf(src_filename, PATH_MAX, "%s/%s", VERSIONS_DIR, hash);
    if(!send_file(c,src_filename)) return SEND_FILE_ERROR;
    if(!send_message(c,"Archivo recuperado exitosamente...")) return MESAGGE_ERROR;
	return FILE_COPIED;
}
void removeFile(char * hash){
	char dst_filename[PATH_MAX];
	snprintf(dst_filename, PATH_MAX, "%s/%s", VERSIONS_DIR,hash);
	remove(dst_filename);
}