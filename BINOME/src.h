#ifndef _SRC
#define _SRC

#include "cell.h"

int hashFile(char *source, char *dest);
char* sha256file(char* file);
List* listdir(char* root dir);
int file exists(char *file);
void cp(char *to, char *from);
char* hashToPath(char* hash);
void blobFile(char* file);

#endif