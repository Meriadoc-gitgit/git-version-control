#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "src.h"

int hashFile(char* src, char *dst) {
  char* res = "cat ";
  strcat(res,src);
  strcat(" | sha256sum > ",dst);
  return system(res);
}

char* sha256file(char* file) {
  char* src = strdup(file);
  int filedes = -1, count = 0;
  
  errno = 0;

  filedes = mkstemp(file);
  unlink(file);
  
  if (filedes<1) {
    printf("Echec de creation de file temporaire [%s]\n",strerror(errno));
    return 1;
  } 
  else printf("File temporaire [%s] cree\n",file);

  errno = 0;

  hashFile(src, file);
  /* Read the data */
  char buffer[256];
  FILE* f = fopen(file,"r");
  if (!f) {
    printf("Erreur lors de l'ouverture\n");
    return;
  }
  fgets(buffer, 256, f);
  fclose(f);
  
  return buffer;
}