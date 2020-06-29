#include <string.h>

class Instruction {

public:

    /* Rapid positioning
        Usage: G0 X2.5
        Description: Moves to the postion defined by x at the maximum possible speed.
    */
    const char *G00 = "G00";

    /* Linear positioning
        Usage: G1 X2.5
        Description: Moves to the postion defined by x at the speed determined by the current state of the stage.
    */
    const char *G01 = "G01";

    /* Start Program
        Usage: M00
    */
    const char *M00 = "M00";

    /* Stop Program
        Usage: M01
    */
    const char *M01 = "M01";

    /*  Pause Program
        Usage: M02
    */
    const char *M02 = "M02";

    /* Turn on the ADC sampling
        Usage: T1
        Description: 
    */
    const char *T1 = "T1";

    /* Turn off ADC sampling
        Usage: T2
        Description:
    */
    const char *T2 = "T2";

    static bool exists(char *input);
};
