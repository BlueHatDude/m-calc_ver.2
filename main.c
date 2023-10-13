#include <stdio.h>
#include <string.h>
#include "mcalc2.h"


#include "mlogging.h"
int main(int argc, char* argv[]) {
  if(argc == 1)
    puts("Please input an expression in the form 'mcalc2 {expression}'");
    

  for(unsigned i = 1; i < (unsigned) argc; ++i) {
      printf("%s: %.4lf \n", argv[i], MC2_evaluate(argv[i]));
  }

}

