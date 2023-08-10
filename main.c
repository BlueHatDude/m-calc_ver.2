#include <stdio.h>
#include <string.h>
#include "mcalc2.h"


int main(int argc, char* argv[]) {
    char equation[50];

    for(unsigned i = 1; i < argc; ++i) {
        strcat(equation, argv[i]);
    }

    printf("%lf \n", MC2_evaluate(equation));
}

