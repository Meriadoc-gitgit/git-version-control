#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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


  const char haystack[20] = "bruh.t";
  const char needle[10] = ".t";
  char *ret;

  ret = strstr(haystack, needle);

  printf("The substring is: %s\n", strstr(haystack, needle));
  return 0;
}