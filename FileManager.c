#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "FileManager.h"
#include "Util.h"
#include <stdlib.h>
#include <limits.h> /* PATH_MAX */

static const char * jumpLine = "\n";
static const char * modeReadOnly = "r";
static const char * modeUpdate = "r+";
char * pathFile;
FILE * file;
ssize_t sizeReadLastLine;
char * lastMode;

/**
 * Abre el archivo para lectura en modo read por defecto
* @param path es la ruta del archivo a analizar 
 **/
void FileManager(char * path) {
	pathFile = path;
	lastMode = (char*) modeUpdate;
	file = fopen(pathFile, lastMode);
}

/**
 * Trae la siguiente linea de texto a leer en el archivo
 **/
char * readLine() {
	if(strcmp(lastMode, modeReadOnly) != 0) {
		file = fopen(pathFile, lastMode = (char*) modeReadOnly);
	}
	char * line = NULL;
	size_t size = 0;
	sizeReadLastLine = getline(&line, &size, file);
	if(strchr(line, '.')!=NULL)
		return NULL;
	if (strcmp(line, "\n") == 0 || strcmp(line, "") == 0) {
		return NULL;
	}else{
		line = removeOfString(line, getString("\n"));
		return line;
	}
}

/**
 * Escribe una linea de texto en el archivo y reliza un salto de linea
* @param line es el elemento a analizar 
* @param mode es el modo de escritura
 **/
void writeLine(char * line, char * mode) {
	file = fopen(pathFile, lastMode = mode);
	fprintf(file, line);
	fprintf(file, jumpLine);
	fclose(file);
}

/**
 * Determina si un archivo es accesible
* @param path es el elemento a analizar 
 **/
bool existFile(char * path) {
	return (access(path, F_OK) != -1) ? true : false;
}

/**
 * Canonaliza el path del archivo por ejemplo /.././hola/./../queHace/ ===> /hola/queHace
* @param path es el elemento a analizar 
 **/
void pathCanonicalize (char *path)
{
	size_t i;
	size_t j;
	size_t k;
	
	//Move to the beginning of the string
	i = 0;
	k = 0;
	
	//Replace backslashes with forward slashes
	while (path[i] != '\0') {
		//Forward slash or backslash separator found?
		if (path[i] == '/' || path[i] == '\\') {
			path[k++] = '/';
			while (path[i] == '/' || path[i] == '\\')
				i++;
		} else {
			path[k++] = path[i++];
		}
	}
	
	//Properly terminate the string with a NULL character
	path[k] = '\0';
	
	//Move back to the beginning of the string
	i = 0;
	j = 0;
	k = 0;
	
	//Parse the entire string
	do {
		//Forward slash separator found?
		if (path[i] == '/' || path[i] == '\0') {
			//"." element found?
			if ((i - j) == 1 && !strncmp (path + j, ".", 1)) {
				//Check whether the pathname is empty?
				if (k == 0) {
					if (path[i] == '\0') {
						path[k++] = '.';
					} else if (path[i] == '/' && path[i + 1] == '\0') {
						path[k++] = '.';
						path[k++] = '/';
					}
				} else if (k > 1) {
					//Remove the final slash if necessary
					if (path[i] == '\0')
						k--;
				}
			}
			//".." element found?
			else if ((i - j) == 2 && !strncmp (path + j, "..", 2)) {
				//Check whether the pathname is empty?
				if (k == 0) {
					path[k++] = '.';
					path[k++] = '.';
					
					//Append a slash if necessary
					if (path[i] == '/')
						path[k++] = '/';
				} else if (k > 1) {
					//Search the path for the previous slash
					for (j = 1; j < k; j++) {
						if (path[k - j - 1] == '/')
							break;
					}
					
					//Slash separator found?
					if (j < k) {
						if (!strncmp (path + k - j, "..", 2)) {
							path[k++] = '.';
							path[k++] = '.';
						} else {
							k = k - j - 1;
						}
						
						//Append a slash if necessary
						if (k == 0 && path[0] == '/')
							path[k++] = '/';
						else if (path[i] == '/')
							path[k++] = '/';
					}
					//No slash separator found?
					else {
						if (k == 3 && !strncmp (path, "..", 2)) {
							path[k++] = '.';
							path[k++] = '.';
							
							//Append a slash if necessary
							if (path[i] == '/')
								path[k++] = '/';
						} else if (path[i] == '\0') {
							k = 0;
							path[k++] = '.';
						} else if (path[i] == '/' && path[i + 1] == '\0') {
							k = 0;
							path[k++] = '.';
							path[k++] = '/';
						} else {
							k = 0;
						}
					}
				}
			} else {
				//Copy directory name
				memmove (path + k, path + j, i - j);
				//Advance write pointer
				k += i - j;
				
				//Append a slash if necessary
				if (path[i] == '/')
					path[k++] = '/';
			}
			
			//Move to the next token
			while (path[i] == '/')
				i++;
			j = i;
		}
		else if (k == 0) {
			while (path[i] == '.' || path[i] == '/') {
				j++,i++;
			}
		}
	} while (path[i++] != '\0');
	
	//Properly terminate the string with a NULL character
	path[k] = '\0';
}
/**
* Metodo ue llama a pathCanonicalize(pathFile) para obtener la ruta Canonicalizada
**/
char * getAbsolutePath() {
	pathCanonicalize(pathFile);
	return pathFile ;
}
