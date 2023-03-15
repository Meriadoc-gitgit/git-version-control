#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char* ch = (char*)malloc(sizeof(char));
  ch[0] = '1';
  ch[1] = 'r';
  printf("%s\n",ch);
  return 0;
}