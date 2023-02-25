#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

int main(void) {
  //system("ls > list.txt");
  
	/*
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
	*/

	char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
  } else {
    perror("getcwd() error");
    return 1;
  }

  int ret;
  FILE *fp;
  char filename[] = "file.txt";

  fp = fopen(filename, "w");

  fprintf(fp, "%s", "This is tutorialspoint.com");
  fclose(fp);
   
  ret = remove(filename);

  if(ret == 0) {
    printf("File deleted successfully");
  } else {
    printf("Error: unable to delete the file");
  }

	return 0;
}