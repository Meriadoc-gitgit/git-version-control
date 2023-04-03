#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "src.h"
#include "bash.h"

int case_break(char* chain) {
  
  if (hash(chain)==hash("init")) return 1;
  if (hash(chain)==hash("list-refs")) return 2;
  if (hash(chain)==hash("create-ref")) return 3;
  if (hash(chain)==hash("delete-ref")) return 4;
  if (hash(chain)==hash("add")) return 5;
  if (hash(chain)==hash("list-add")) return 6;
  if (hash(chain)==hash("clear-add")) return 7;
  if (hash(chain)==hash("commit")) return 8;
  if (hash(chain)==hash("get-current-branch")) return 9;
  if (hash(chain)==hash("branch")) return 10;
  if (hash(chain)==hash("branch-print")) return 11;
  if (hash(chain)==hash("checkout-branch")) return 12;
  if (hash(chain)==hash("checkout-commit")) return 13;
  else return -1;
}