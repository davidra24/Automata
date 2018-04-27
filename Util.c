#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "Util.h"

/**
 * Trae la informacion que haya ingresado el usuario por consola
 **/
char * readLineKonsole() {
	char *line = NULL;
	size_t len = 0;
	getline(&line, &len, stdin);
	removeOfString(line, getString("\n"));
	return line;
}

/**
 * Convierte una constante de cadena de texto en un variable
* @param str es el elemento a analizar 
**/
char * getString(const char * str) {
	return (char *) str;
}

/**
 * Remueve una cadena de texto de una cadena principal
* @param string es el elemento a analizar 
* @param stringToRemove es la cadena a eliminar de string 
 **/
char * removeOfString(char * string, char * stringToRemove) {
	char *line = strstr(string, stringToRemove);
	if (line)
		strcpy(line, line + strlen(stringToRemove));
	return string;
}

/**
 * Permite arministrar los mensajes de opcion doble en los cuales el usuario debe ingresar uno u otro caracter para continiar un proceso
* @param message es el elemento a analizar 
* @param numberArgs es el numero de argumentos 
**/
int selectInputTwoOptions(const char * message, int numberArgs, ...) {
	va_list elements;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	printf("\n %s \n", message);
	while ((read = getline(&line, &len, stdin)) != -1) {
		removeOfString(line, getString("\n"));
		va_start(elements, numberArgs);
		for (int i = 0; i < numberArgs; ++i) {
			if (strcmp(line, va_arg(elements, const char *)) == 0) {
				free(line);
				va_end(elements);
				return i + 1;
			}
		}
		puts("[Error. Opción Incorrecta]");
		printf("\n %s \n", message);
	}
	va_end(elements);
	return 0;
}

/**
 * Trae la informacion que haya ingresado el usuario por consola
 **/
char * getStringOfKonsole() {
	char *line = NULL;
	size_t len = 0;
	getline(&line, &len, stdin);
	removeOfString(line, getString("\n"));
	return line;
}

/**
 * Perite separar cadenas de texto de acuerdo a lo que se requiera y guarda el resultado en arrayOutput y el tamano de separaciones hechas en la variable quantityTokens
* @param string es el elemento a analizar
* @param delimitation es por cuales caracteres se va a romper la cadena
* @param arrayOutput es el array ue almacena los elementos ue se rompieron de string
* @param quantityTokens es la cantidad de elementos rotos 
**/
void split(char * string, const char * delimitation, char *** arrayOutput, int * quantityTokens) {
	int countTokens = 0;
	char * token;
	char ** array = NULL;
	token = strtok(string, delimitation);
	while (token != NULL) {
		array = (char **) realloc(array, (countTokens + 1) * sizeof(char *));
		array[countTokens] = token;
		countTokens++;
		token = strtok(NULL, delimitation);
	}
	*arrayOutput = array;
	*quantityTokens = countTokens;
}

/**
 * Trae la posicion en la que se encuentra un elemento en un array, si el elemento no se encuentra retorna -1
* @param array es el elemento a analizar 
* @param length es la longitud de array
* @param string es el elemento a analizar en array 
 **/
int getPositionInArray(char ** array, int length, char * string) {
	for (int i = 0; i < length; ++i) {
		if (strcmp(array[i], string) == 0) {
			return i;
		}
	}
	return -1;
}

/**
 * Determina si un dato esta contenido en un array
* @param array es el elemento a analizar 
* @param length es la longitud de array
* @param string es el elemento a analizar en array  
 **/
bool containtArray(char ** array, int length, char * string) {
	return getPositionInArray(array, length, string) != -1;
}

/**
 * Convierte un caracter a cadena de texto
*@param character es el elemento a analizar 
 **/
char * getStringOfChar(char character) {
	char * characterString = (char*) calloc(2, sizeof(char));
	characterString[0] = character;
	characterString[1] = '\0';
	return characterString;
}

