#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  //system("ls > list.txt");
  char* tmp = "strtok needs to be called several times to split a string";
	char str[(int)strlen(tmp)];
	for (int i=0;i<(int)strlen(tmp);i++) {
		str[i] = tmp[i];
	}
  printf("%s\n",str);
	int init_size = (int)strlen(str);
	char* delim = " ";

	char *ptr = strtok(str, delim);

	while (ptr != NULL) {
		printf("'%s'\n", ptr);
		ptr = strtok(NULL, delim);
	}

	return 0;
}