#ifndef _SRC
#define _SRC

#include "cell.h"

int hashFile(char *source, char *dest);
char* sha256file(char* file);
List* listdir(char* root_dir); // Manque de "_" dans root_dir, deja corrige
int file_exists(char *file); // Manque de "_" dans file_exists, deja corrige
void cp(char *to, char *from);
char* hashToPath(char* hash);
void blobFile(char* file);

#endif