#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"
#include "src.h"

int main(void) {
  Cell** L = initList();
  Cell* C = buildCell("test");

  insertFirst(L,C);
  insertFirst(L,buildCell("test2"));


  Cell* C2 = searchList(L,"test");
  Cell* C3 = searchList(L,"test2");
  char* test = ctos(C2);

  printf("test2:%s\n",ctos(C3));

  char tmp2[256];
  strcpy(tmp2,ctos(C3));
  strcat(test,tmp2);
  printf("test_cat:%s\n",test);
  printf("tmp2:%s\n",tmp2);
  printf("test2:%s\n",ctos(C3));


  strcpy(tmp2,ctos(C3));
  strcat(test,tmp2);
  printf("\ntest_cat:%s\n",test);
  printf("tmp2:%s\n",tmp2);
  printf("test2:%s\n",ctos(C3));
  //strcat(test,strdup(test));

  printf("\n%s\n",test);
  printf("test2:%s\n\n",ctos(C3));

  char* tmp = ltos(L);
  printf("%s\n",tmp);
  

  int hash = hashFile("src.txt","dst.txt");
  printf("%d\n",hash);
  return 0;
}