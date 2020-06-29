#include <string.h>

class Instruction {

public:
    Instruction::Instruction(char * _instruction);

    /* Rapid positioning
        Usage: G0 X2.5
        Description: Moves to the postion defined by x at the maximum possible speed.
    */
    static const char *G00;

    /* Linear positioning
        Usage: G1 X2.5
        Description: Moves to the postion defined by x at the speed determined by the current state of the stage.
    */
    static const char *G01;

    /* Start Program
        Usage: M00
    */
    static const char *M00;

    /* Stop Program
        Usage: M01
    */
    static const char *M01;

    /*  Pause Program
        Usage: M02
    */
    static const char *M02;

    /* Turn on the ADC sampling
        Usage: T1
        Description: 
    */
    static const char *T1;

    /* Turn off ADC sampling
        Usage: T2
        Description:
    */
    static const char *T2;

    static bool exists(char *input);
};
