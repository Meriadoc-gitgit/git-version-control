#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
  char* ch = (char*)malloc(sizeof(char));
  char str[15];
  sprintf(str, "%d", 42);
  printf("%s\n",str);

  ch[0] = 'r';
  printf("%s\n",ch);

  sprintf(str,"%s %s",str,str);
  printf("%s\n",str);
  printf("%d\n",MAX_INPUT);
  return 0;
}