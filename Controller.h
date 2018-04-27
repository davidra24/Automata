#include <stdbool.h>
#ifndef CONTROLLER_H_
#define CONTROLLER_H_
/**
Posee todos los elementos que podra interperetar el automata
**/
typedef struct Alphabet{
	char ** array;
	int length;
} alphabet;
/**
Posee todos lo estados ue tiene el automata
**/
typedef struct Status{
	char ** array;
	int length;
} status;
/**
Posee los estados en los que se terminara la comprobacion de las palabras
**/
typedef struct FinalStatus{
	char ** array;
	int length;
} finalStatus;
/**
Es el autamata en si con las estructuras en el
**/
extern struct FiveFold{
	struct Alphabet alphabet;
	struct Status status;
	char *** transitionFunction;
	char * statusInit;
	struct FinalStatus finalStatus;
} fiveFold;
struct FiveFold fiveFold;
void init();
bool readAutomataForFile();
bool processOptionAutomata();
bool processOptionWords();
bool readWordForFile();
bool readWordForKonsole();
bool processAutomataFromFile(char * file);
bool processWordsFromFile(char * file);
bool processWordInAutomata(char * line);
bool verifyCharacter(char ** array, int length, char * line, int positionCurrentOfLine);
bool processAlphabet(char * alphabet);
bool processStatus(char * status);
bool processTransitionFunction(char * transitionFunction);
void verifyExistLambda(char * transitionFunction);
void createTransitionFunction();
bool processStatusInit(char * statusInit);
bool processFinalStatus(char * finalStatus);
bool readAutomataForKonsole();
char * verifyTransitionFunctionForKonsole();
char * getStringFillOfKonsole(char * message);
char * validateStatusInitOfKonsole(char * message);
bool isStatus(char * stringStatus);

#endif /* CONTROLLER_H_ */
