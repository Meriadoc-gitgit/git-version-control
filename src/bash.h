#ifndef _BASH
#define _BASH

#include "src.h"

/* Part 1 */
int hashFile(char* src, char *dst);
char* sha256file(char* file);

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char* to, char* from);
char* hashToPath(char* hash);
void blobFile(char* file);

/* Part 2 */
int getChmod(const char* path);
void setMode(int mode,char* path);
#endif