#include <string.h>

class Instruction {

public:
    static char *G00;
    static char *G01;
    static char *M00;
    static char *M01;
    static char *M02;
    static char *T1;
    static char *T2;

    static bool exists(char *input);
};
