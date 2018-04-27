#include <stdbool.h>
#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

extern char * pathFile;
extern FILE * file;
extern ssize_t sizeReadLastLine;
extern char * lastMode;

void FileManager(char * path);
char * readLine();
void writeLine(char * line, char * mode);
bool existFile(char * path);
char * getAbsolutePath();

#endif
