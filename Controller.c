#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Controller.h"
#include "FileManager.h"
#include "Util.h"

/**
 * Inicia el programa
 **/
void init() {
	printf("\n ____________________\n");
	printf("\n 	Automata \n");
	printf("\n ____________________\n");
	if (!processOptionAutomata()) {
		printf("\n[Error. Qintupla incorrecta, por favor revísela]\n");
		init();
		return;
	}
	printf("\n Automata procesado... \n");
	if(!processOptionWords()) {
		printf("\n[Error. Qintupla incorrecta, por favor revísela]\n");
		init();
		return;
	}
	init();
}

/**
 * Corresponde a la primera opcion para el usuario, permite procesar un automata y el procedimiento correspondiente
 **/
bool processOptionAutomata() {
	int valueAutomata = selectInputTwoOptions("Digite 1 si para ingresar un archivo de Automata\n Digite 2 para ingresar el Automata por consola \n Digite 3 para salir del programa", 3, "1", "2", "3");
	switch (valueAutomata) {
	case 1:
		if (!readAutomataForFile()){return false;} break;
	case 2:
		if (!readAutomataForKonsole()){return false;} break;
	case 3:
		exit(0);
	}
	return true;
}

/**
 * Corresponde a la segunda opcion para el usuario, realiza el procedimiento de procesado de palabras
 **/
bool processOptionWords() {
	int valueWord = selectInputTwoOptions("Digite 1 para ingresar un archivo de pruebas \n Digite 2 para ingresar Palabras por consola \n Digite 3 para solucionar otro Automata \n Digite 4 para salir del programa", 4, "1", "2", "3", "4");
	switch (valueWord) {
		case 1:
			if(readWordForFile()){processOptionWords();return true;}break;
		case 2:
			if(readWordForKonsole()){processOptionWords();return true;}break;
		case 3:
			return true;
		case 4:
			exit(0);
	}
	return false;
}

/**
 * Obtiene y administra cada una de las entradas de la quintupla por consola de parte del usuario
 **/
bool readAutomataForKonsole() {
	char * alphabet = getStringFillOfKonsole(getString("Digite el alfabeto \nPor ejemplo: a, b, c, d\n"));
	char * status = getStringFillOfKonsole(getString("Digite la lista de estados \nPor ejemplo: Q0, Q1 ,Q2 ,Q3\n"));
	char * transitionFunction = verifyTransitionFunctionForKonsole();
	char * statusInit = validateStatusInitOfKonsole(getString("Digite el estado inicial \nPor ejemplo: Q0\n"));
	char * finalStatus = getStringFillOfKonsole(getString("Digite el(los) estado(s) final(es) \nPor ejemplo: Q1,Q3\n"));
	int valueWord = selectInputTwoOptions("Digite 1 si desea guardar los datos en un archivo plano \n Digite 2 por si no desea guardarlo", 2, "1", "2");
	if (valueWord == 1) {
		char * file = getStringFillOfKonsole(getString("Digite el nombre del archivo con su extencion \n"));
		while(existFile(file)) {
			printf("\n[Error. El archivo ya existe o no se puede guardar en este diretorio]\n");
			file = getStringFillOfKonsole(getString("Digite el nombre del archivo"));
		}
		FileManager(file);
		writeLine(alphabet, getString("a"));
		writeLine(status, getString("a"));
		writeLine(transitionFunction, getString("a"));
		writeLine(statusInit, getString("a"));
		writeLine(finalStatus, getString("a"));
		printf("\n  ____________________________________");
		printf("\n | Archivo Guardado en: %s\n", getAbsolutePath());
		printf("\n  ____________________________________");
	}

	if (!processAlphabet(alphabet)) return false;
	if (!processStatus(status)) return false;
	if (!processTransitionFunction(transitionFunction)) {
		puts("\n[Error. La funcion de transicion es incorrecta, presenta una syntaxis equivocada]");
		return false;
	}
	if (!processStatusInit(statusInit)) return false;
	if (!processFinalStatus(finalStatus)) return false;

	return true;
}

/**
 * Valida si la estructura de la funcion de transicion ingresada por consola es correcta
 **/
char * verifyTransitionFunctionForKonsole() {
	char * line = getStringFillOfKonsole(getString("Digite la funcion de transicion \n Por ejemplo: Q0:a:Q1;Q0:b:Q0,Q2;Q0:c:Q0;Q1:a:Q2;Q2:a:Q2;Q2:b:Q2;\n"));
	while(strstr(line, ";") == NULL || strstr(line, ":") == NULL) {
		printf("\n[Error, la estructura de la funciona de transicion es erronea]\n");
		line = getStringFillOfKonsole(getString("Digite la funcion de transicion \n Por ejemplo: Q0:a:Q1;Q0:b:Q0,Q2;Q0:c:Q0;Q1:a:Q2;Q2:a:Q2;Q2:b:Q2;\n"));
	}
	return line;
}

/**
 * Valida si la estructura de los estados iniciales ingresados por consola es correcta
 * @param message es el elemento a analizar 
 **/
char * validateStatusInitOfKonsole(char * message) {
	char * statusInit = getStringFillOfKonsole(message);
	while(strstr(statusInit, ",") != NULL) {
		puts("\n[Error. Solo puede ingresar 1 estado inicilal]");
		statusInit = getStringFillOfKonsole(message);
	}
	return statusInit;
}

/**
 * Permite verificar que el usuario ingrese por consola al menos un dato
 **/
char * getStringFillOfKonsole(char * message) {
	printf("\n%s\n", message);
	char * line = getStringOfKonsole();
	while (strcmp(line, "") == 0) {
		puts("[Error, línea vacia]");
		printf("\n%s\n", message);
		line = getStringOfKonsole();
	}
	return line;
}

/**
 * Procesa en el automata cada una de las palabras que el usuario ingrese por medio de la consola
 **/
bool readWordForKonsole() {
	char * word = getStringFillOfKonsole(getString("Ingrese la palabra que desea probar: "));
	printf("\n");
	if (strstr(word, getStringOfChar(SYMBOL_LAMBDA)) == NULL) {
		printf("La palabra: %s %s se ha procesado\n", word, (!processWordInAutomata(word)) ? "NO" : "SI");
	}else {
		printf("La palabra: %s NO debe contener Lambda (@)\n", word);
	}
	return true;
}

/**
 * Valida la entrada del archivo que contiene la quintupla y luego llama al metodo para procesar la quintupla
 **/
bool readAutomataForFile() {
	printf("Ingrese el nombre del Archivo (recuerde que debe estar en la misma direccion del ejecutable)\n");
	char * file = readLineKonsole();
	while (!existFile(file)) {
		printf("\n[Error. al acceder al Archivo]\n");
		printf("\n[Ingrese el nombre del Archivo] (recuerde que debe estar en la misma direccion del ejecutable):\n ");
		file = readLineKonsole();
		
	}
	if (!processAutomataFromFile(file)) return false;
	return true;
}

/**
 * Valida el ingreso del archivo que contiene las palabras y luego llama al metodo de verificado de las palabras
 **/
bool readWordForFile() {
	printf("\n Ingrese el nombre de archivo (recuerde que debe estar en la misma direccion del ejecutable) \n");
	char * file = readLineKonsole();
	while (!existFile(file)) {
		printf("\n[Error, al acceder al archivo]\n");
		printf("\nIngrese el nombre del Archivo (recuerde que debe estar en la misma direccion del ejecutable): ");
		file = readLineKonsole();
	}
	if (!processWordsFromFile(file)) return false;
	return true;
}

/**
 * Procesa en el automata cada una de las palabras que el usuario ingrese por medio de un archivo de texto plano
 * @param file es el elemento a analizar 
 **/
bool processWordsFromFile(char * file) {
	FileManager(file);
	char * line;
	printf("\n");
	while((line = (char*)readLine()) != NULL) {
		if (strstr(line, getStringOfChar(SYMBOL_LAMBDA)) == NULL) {
			printf("La palabra: %s %s  ha sido admitida por el automata\n", line, (!processWordInAutomata(line)) ? "NO" : "SI");
		}else {
			printf("La palabra: %s NO debe contener Lambda (@)\n", line);
		}
	}
	return true;
}

/**
 * Hace las validaciones necesarias antes de procesar una palabra en el automata y luego llama al metodo que realiza la recursividad en busca de la palabra
* @param line es el elemento a analizar 
**/
bool processWordInAutomata(char * line) {
	int positionStatusInit = getPositionInArray(fiveFold.status.array, fiveFold.status.length, fiveFold.statusInit);
	if (containtArray(fiveFold.alphabet.array, fiveFold.alphabet.length, getStringOfChar(SYMBOL_LAMBDA))) {
		int newLength = (strlen(line) * 2);
		char * newLine = (char *) calloc(newLength, sizeof(char));
		int count = 0;
		newLine[0] = SYMBOL_LAMBDA;
		for (int i = 0; i < newLength; ++i) {
			if (i%2 == 0) {
				newLine[i] = SYMBOL_LAMBDA;
			}else {
				newLine[i] = line[count];
				count = count + 1;
			}
		}
		line = newLine;
	}
	return verifyCharacter(fiveFold.transitionFunction[positionStatusInit], fiveFold.alphabet.length, line, 0);
}

/**
 * Metodo principal para determinar si una palabra pertenece al automata
* @param array es el elemento a analizar 
* @param length es la longitud del array
* @param line es el elemento a analizar con el array
* @param positionCurrentOfLine es la posicion de la linea
 **/
bool verifyCharacter(char ** array, int length, char * line, int positionCurrentOfLine) {
	char * characterString = getStringOfChar(line[positionCurrentOfLine]);
	int positionCharacterInArray = getPositionInArray(fiveFold.alphabet.array, fiveFold.alphabet.length, characterString);
	if (positionCharacterInArray == -1) {
		return false;
	}
	char ** arrayCurrentOfStatus = NULL;
	int countStatus = 0;
	split(array[positionCharacterInArray], ",", &arrayCurrentOfStatus, &countStatus);
	if(arrayCurrentOfStatus != 0) {
		for (int i = 0; i < countStatus; ++i) {
			int positionStatus = getPositionInArray(fiveFold.status.array, fiveFold.status.length, arrayCurrentOfStatus[i]);
			bool continueToNextStatus = false;
			if (positionCurrentOfLine == (strlen(line) - 1)) {
				if (containtArray(fiveFold.finalStatus.array, fiveFold.finalStatus.length, fiveFold.status.array[positionStatus])) {
					return true;
				}else {
					if (i == (countStatus - 1)) {
						return false;
											}
					continueToNextStatus = true;
				}
			}
			if (!continueToNextStatus) {
				bool result = verifyCharacter(fiveFold.transitionFunction[positionStatus], fiveFold.alphabet.length, line, positionCurrentOfLine+1);
				if (result == true) {
					return result;
				}
			}
		}
	}else {
		if(strcmp(characterString, getStringOfChar(SYMBOL_LAMBDA)) == 0) {
			positionCurrentOfLine = positionCurrentOfLine+1;
			return verifyCharacter(array, fiveFold.alphabet.length, line, positionCurrentOfLine);
		}
	}
	return false;
}

/**
 * Obtiene y administra cada una de las entradas de la quintupla por el archivo plano
* @param file es el elemento a analizar 
**/
bool processAutomataFromFile(char * file) {
	FileManager(file);
	char * alphabet = readLine();
	char * status = readLine();
	char * transitionFunction = readLine();
	char * statusInit = readLine();
	char * finalStatus = readLine();
	if (alphabet == NULL || status == NULL || transitionFunction == NULL || statusInit == NULL || finalStatus == NULL) {
		puts("\n[Error. El archivo no contiene toda la quintupla]");
		return false;
	}

	if (!processAlphabet(alphabet)) return false;
	if (!processStatus(status)) return false;
	if (!processTransitionFunction(transitionFunction)) {
		puts("\n[Error. la funcion de transicion no se puede procesar, esta mal escrita o el archivo de texto contiene un formato erroneo]");
		return false;
	}
	if (!processStatusInit(statusInit)) return false;
	if (!processFinalStatus(finalStatus)) return false;

	return true;
}

/**
 * Carga a memoria el alfabeto obtenido del archivo
* @param alphabet es el elemento a analizar 
 **/
bool processAlphabet(char * alphabet) {
	char ** array = NULL;
	int count = 0;
	split(alphabet, ",", &array, &count);
	fiveFold.alphabet.array = array;
	fiveFold.alphabet.length = count;
	return true;
}

/**
 * Carga a memoria la lista de estados obtenidos del archivo
* @param status es el elemento a analizar 
 **/
bool processStatus(char * status) {
	char ** array = NULL;
	int count = 0;
	split(status, ",", &array, &count);
	fiveFold.status.array = array;
	fiveFold.status.length = count;
	return true;
}

/**
 * Carga a memoria la funcion de transicion obtenida del archivo
* @param transitionFunction es el elemento a analizar 
 **/
bool processTransitionFunction(char * transitionFunction) {
	if (strstr(transitionFunction, ";") == NULL || strstr(transitionFunction, ":") == NULL) {
		return false;
	}
	verifyExistLambda(transitionFunction);
	createTransitionFunction();
	char ** arrayFirst = NULL;
	int countFirst = 0;
	split(transitionFunction, ";", &arrayFirst, &countFirst);
	for (int i = 0; i < countFirst; ++i) {
		char ** arraySecond = NULL;
		int countSecond = 0;
		split(arrayFirst[i], ":", &arraySecond, &countSecond);
		int positionStatus = getPositionInArray(fiveFold.status.array, fiveFold.status.length, arraySecond[0]);
		int positionAlphabet = getPositionInArray(fiveFold.alphabet.array, fiveFold.alphabet.length, arraySecond[1]);
		if (positionStatus == -1 || positionAlphabet == -1) {
			puts("\n[Error. La funcion de transicion contiene uno o más estados iniciales o letras del alfabeto erroneas]");
			return false;
		}
		if (!isStatus(arraySecond[2])) {
			puts("\[Error. la funcion de transicion contiene uno o mas estados de llegada que no pertenecen a la lista de estados]");
			return false;
		}
		char * line = fiveFold.transitionFunction[positionStatus][positionAlphabet];
		fiveFold.transitionFunction[positionStatus][positionAlphabet] = strcat(line, ((strcmp(line, "") == 0) ? arraySecond[2] : strcat(getString(","), arraySecond[2])));
	}
	return true;
}
/**
* Determina si el estado ingresado pertenece a la quintupla
* @param stringStatus es el elemento a analizar 
**/
bool isStatus(char * stringStatus) {
	char ** array = NULL;
	int count = 0;
	split(stringStatus, ",", &array, &count);
	for (int i = 0; i < count; ++i) {
		int positionStatus = getPositionInArray(fiveFold.status.array, fiveFold.status.length, array[i]);
		if (positionStatus == -1) {
			return false;
		}
	}
	return true;
}

/**
 * Determina si existe la funcion lambda en la funcion de transicion para agregarla al alfabeto
* @param transitionFunction es el elemento a analizar 
 **/
void verifyExistLambda(char * transitionFunction) {
	if (strstr(transitionFunction, getStringOfChar(SYMBOL_LAMBDA)) != NULL) {
		char ** newAlphabet = (char **) calloc(fiveFold.alphabet.length + 1, sizeof(char*));
		for (int i = 0; i < fiveFold.alphabet.length + 1; ++i) {
			newAlphabet[i] = fiveFold.alphabet.array[i];
		}
		newAlphabet[fiveFold.alphabet.length] = getStringOfChar(SYMBOL_LAMBDA);
		fiveFold.alphabet.array = newAlphabet;
		fiveFold.alphabet.length = fiveFold.alphabet.length + 1;
	}
}

/**
 * Asigana en memoria el espacio necesario para almacenar la matriz de transicion
 **/
void createTransitionFunction() {
	int rows = fiveFold.status.length;
	int columns = fiveFold.alphabet.length;
	fiveFold.transitionFunction = (char ***) calloc(rows, sizeof(char**));
	for (int i = 0; i < rows; ++i) {
		fiveFold.transitionFunction[i] = (char **) calloc(columns, sizeof(char*));
		for (int j = 0; j < columns; ++j) {
			fiveFold.transitionFunction[i][j] = (char*) calloc(3, sizeof(char));
		}
	}
}

/**
 * Carga a memoria en estado inicial obtenido del archivo
* @param statusInit es el elemento a analizar 
 **/
bool processStatusInit(char * statusInit) {
	for (int i = 0; i < fiveFold.status.length; ++i) {
		if (strcmp(fiveFold.status.array[i], statusInit) == 0) {
			fiveFold.statusInit = statusInit;
			return true;
		}
	}
	puts("\n[Error, el estado inicial no esta contenido en la lista de estados]");
	return false;
}

/**
 * Carga a memoria la lista de estados finales obtenidos del archivo
* @param finalStatus es el elemento a analizar 
 **/
bool processFinalStatus(char * finalStatus) {
	char ** array = NULL;
	int count = 0;
	split(finalStatus, ",", &array, &count);
	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < fiveFold.status.length; ++j) {
			if (strcmp(fiveFold.status.array[j], array[i]) == 0) {
				break;
			}
			if (j == (fiveFold.status.length - 1)){
				puts("\n[Error, el o los estados finales no esta contenidos en la lista de estados]");
				return false;
			}
		}
	}
	fiveFold.finalStatus.array = array;
	fiveFold.finalStatus.length = count;
	return true;
}
