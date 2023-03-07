#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>

int hashFile(char* source, char* dest){
  FILE*f=fopen(source,"r");
  assert(f!=NULL);
  static char template [] ="/tmp/myfileXXXXXX";
  strcpy (dest, template) ;
  int fd = mkstemp (dest) ;
  if (fd==-1){
    printf("ERREUR");
    return fd;
  } 
  FILE*f1=fopen(dest,fd);
  int hash=sha256sum(f);
  fprintf(f1,"%d",hash);
  fclose(f);
  fclose(f1);
}

char* sha256file(char* file){
  
  FILE*f=fopen(file,"r");
  assert(f!=NULL);
  static char template [] ="/tmp/myfileXXXXXX";
  char fTmp [1000];
  int fd = mkstemp (fTmp) ;//fichier temporaire
  strcpy (fTmp, template);
   if (fd==-1){
    printf("ERREUR");
  }
  sprintf()
 // fclose(f);
  //fclose(fTmp);
  return sha256sum(file)>fTmp;//pas fini
}

int main() {
  printf("hihi\n");
  system("ls");
  // Question 1.2
 //main.c|sha256sum> file.tmp
}