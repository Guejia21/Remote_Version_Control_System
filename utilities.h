#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include <stdlib.h>


#ifdef __linux__
	#include <linux/limits.h>
#elif _WIN32
	#include <limits.h>
#endif

#define EQUALS(s1, s2) (strcmp(s1, s2) == 0) /**< Verdadero si dos cadenas son iguales.*/
#define COMMENT_SIZE 80 /** < Longitud del comentario */
#define HASH_SIZE 256 /**< Longitud del hash incluyendo NULL*/

/**
 * @brief Codigo de retorno de operacion
 */
typedef enum {
	VERSION_ERROR, /*!< Error no especificado */
	VERSION_CREATED, /*!< Version creada */
	VERSION_ADDED, /*!< Version agregada */
	VERSION_ALREADY_EXISTS, /*!< Version ya existe */
	FILE_ADDED, /*<! Archivo adicionado  */
	VERSION_DOESNT_EXISTS, /*<! Versión no existe*/
	FILE_NOT_COPIED, /*<! Archivo no copiado*/
	FILE_COPIED, /*<! Archivo copiado*/
	VERSION_ADDED_DB, /*<! Archivo añadida en la BD*/
	/* .. */
}return_code;
/**
 * @brief Version de un archivo.
 * Para cada version de un archivo se almacena el nombre original,
 * el comentario del usuario y el hash de su contenido.
 * El hash es a la vez el nombre del archivo dentro del
 * repositorio.
 */
typedef  struct __attribute__((aligned(512))) {
	char filename[PATH_MAX]; /**< Nombre del archivo original. */
	char hash[HASH_SIZE];           /**< Hash del contenido del archivo. */
	char comment[COMMENT_SIZE];	   /**< Comentario del usuario. */
}file_version;
/**
 * @brief Divide un comando en tokens
 * 
 * @param command Comando a dividir
 * @param argc Número de tokens
 * @return char** Arreglo de tokens
 */
char **split_command(const char *command, int *argc);

#endif