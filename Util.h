#ifndef UTIL_H_
#define UTIL_H_

static const char SYMBOL_LAMBDA = '@';
char * removeOfString(char * string, char * stringToRemove);
char * getString(const char * str);
char * readLineKonsole();
int selectInputTwoOptions(const char * message, int numberArgs, ...);
char * getStringOfKonsole();
void split(char * string, const char * delimitation, char *** arrayOutput, int * quantityTokens);
char * getCopy(char * string);
int getPositionInArray(char ** array, int length, char * string);
bool containtArray(char ** array, int length, char * string);
char * getStringOfChar(char character);

#endif /* UTIL_H_ */
