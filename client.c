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
#include "client.h"
/**
 * @brief Valida si el comando ingresado es valido
 * 
 * @param command Comando ingresado
 * @return int 0 si es invalido, 1 si es valido
 */
int isValid(char * command);
/**
 * @brief Imprime la ayuda
 */
void usageRVersions();
/**
 * @brief Crea una version en memoria del archivo
 * Valida si el archivo especificado existe y crea su hash
 * @param filename Nombre del archivo
 * @param hash Hash del contenido del archivo
 * @param comment Comentario
 * @param result Nueva version en memoria
 *
 * @return Resultado de la operacion
 */
return_code create_version(char * filename, char * comment, file_version * result);
/**
 * @brief Obtiene el hash de un archivo
 * 
 * @param filename Nombre del archivo
 * @param hash Hash del archivo
 * @return char* Hash del archivo
 */
char *get_file_hash(char * filename, char * hash);

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
    addr_server.sin_port=htons(puerto);
    //Convertir la dirección ip a binario
    if (inet_pton(AF_INET, ip, &addr_server.sin_addr) <= 0) {
        perror("Error initializing address");
        exit(EXIT_FAILURE);
    }
    if(connect(c,(struct sockaddr *)&addr_server,sizeof(struct sockaddr_in))== -1){
        perror("Error al conectar el socket");
        exit(EXIT_FAILURE);
    } 
    printf("Conectado al servidor %s:%d\n",ip,puerto);    
    //Retornar el socket
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
    int argc=0;
    char ** argv = split_command(strdup(command),&argc);
    if((argc==3 && EQUALS(argv[0],"add")) || (argc==3 && (EQUALS(argv[0],"get")) && atoi(argv[1])>0) || (argc==2 && EQUALS(argv[0],"list"))|| (argc==1 && EQUALS(argv[0],"list")||(argc==1 && EQUALS(argv[0],"exit")))){
        return 1;
    }
    usageRVersions();
    return 0;
}
return_code get_response(int c, char * command){
    char buf[BUF_SIZE];
    int argc=0;
    memset(buf,0,BUF_SIZE);
    char ** argv = split_command(command,&argc);

    if(argc>0 && EQUALS(argv[0],"add")){
        //Msj esperado: Verificando si el archivo existe...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;
        file_version v;
	    // Crea la nueva version en memoria
        if(create_version(argv[1], argv[2], &v)==VERSION_ERROR){
		    send_message(c,"Error al crear la versión"); 
            return VERSION_ERROR;
	    }
        if(!send_message(c,"Version creada...")) return MESAGGE_ERROR;
        // Envia la nueva version al servidor
        if(!send_file_version(&v,c)) return SEND_FILE_ERROR;
        //Msj esperado: Descriptor del archivo recibido, verificando si ya existe una version con el mismo hash...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;        
        //Msj esperado: Recibiendo archivo...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;
        if(EQUALS(buf,"Ya existe una versión con el mismo hash")) return VERSION_ALREADY_EXISTS;        
        //Se envia el archivo
        if(!send_file(c,v.filename)) return SEND_FILE_ERROR;
        //Msj esperado: Archivo recibido...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;
        //Msj esperado: Archivo guardado en la base de datos correctamente...
        if(!recieve_message(c,"Server",buf)){
            return 0;
        }
        if(EQUALS(buf,"Archivo guardado en la base de datos correctamente...")) return FILE_ADDED;
        else return VERSION_ERROR;
    }
    else if(EQUALS(argv[0],"get")){ //argv es [get NUMBER ARCHIVO]
        //Msj esperado: Buscando archivo...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;
        //Msj esperado: Archivo encontrado, recbiendo archivo...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;        
        if(EQUALS(buf,"Archivo no encontrado...")) return VERSION_DOESNT_EXISTS;
        if(!recieve_file(c,argv[2])) return RECIEVE_FILE_ERROR;
        //Msj esperado: Archivo recuperado exitosamente...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;                
        return FILE_COPIED;
    }
    else if(EQUALS(argv[0], "list")){
        // Mensaje esperado: Abriendo repositorio de versiones...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;
        //Recibiendo listado de versiones...
        file_version v;
        memset(&v, 0, sizeof(v));
        while(1){
            if(!recieve_file_version(&v,c)) return RECIEVE_FILE_ERROR;
            if(EQUALS(v.filename,"END_OF_LIST")) break;
            printf("Nombre del archivo: %s\n",v.filename);
            printf("Hash: %s\n",v.hash);
            printf("Comentario: %s\n",v.comment);
        }
        // Mensaje esperado: Listado de versiones terminado...
        if(!recieve_message(c,"Server",buf)) return MESAGGE_ERROR;
        return LIST_RETURN;  
    }
    else printf("Comando no reconocido\n");
}
return_code create_version(char * filename, char * comment, file_version *result) {
	// Llena a estructura result recibida por referencia.
	// Debe validar:
	// 1. Que el archivo exista y sea un archivo regular
    memset(result, 0, sizeof(file_version));//Limpia el file_version
	char *hash = (char*)malloc(HASH_SIZE);
	if(hash == NULL) return VERSION_ERROR; //Se retorna error en caso de que no se haya podido asignar memoria   
	if(get_file_hash(filename,hash)==NULL){//Dentro de este método se verifica que exista y sea un archivo correcto
		free(hash); //Libera la memoria si falla
		return VERSION_ERROR;
	}
	// 2. Obtiene y guarda en la estructura el HASH del archivo
	strncpy(result->hash, hash, HASH_SIZE); //Se hace de esta forma ya que no se puede asignar un puntero a un arreglo directamente
	result->hash[HASH_SIZE - 1] = '\0';  // Necesario si es una cadena
	// Llena todos los atributos de la estructura y retorna VERSION_CREATED
	strncpy(result->filename,filename,strlen(filename));
	strncpy(result->comment,comment,strlen(comment));
	// En caso de fallar alguna validacion, retorna VERSION_ERROR
	return VERSION_CREATED;
}
char *get_file_hash(char * filename, char * hash) {
	struct stat s;
	//Verificar que el archivo existe y que se puede obtener el hash
	if (stat(filename, &s) < 0 || !S_ISREG(s.st_mode)) {
		perror("stat");
		return NULL;
	}

	sha256_hash_file_hex(filename, hash);

	return hash;

}
void usageRVersions(){
    printf("Uso:\n   add ARCHIVO \"Comentario\" : Adiciona una version del archivo al repositorio\n");
    printf("    list ARCHIVO             : Lista las versiones del archivo existentes\n");
    printf("    list                     : Lista todos los archivos almacenados en el repositorio\n");
    printf("    get NUMBER ARCHIVO       : Obtiene una version del archivo del repositorio, NUMBER > 0\n");
}