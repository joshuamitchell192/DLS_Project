#include "Instruction.hpp"

/* Rapid positioning
    Usage: G0 x
    Description: Moves to the postion defined by x at the maximum possible speed.
*/
static string G0 = "G0";

/* Linear positioning
    Usage: G1 x
    Description: Moves to the postion defined by x at the speed determined by the current state of the stage.
*/
static string G1 = "G1";

/* Start Program
    Usage: M0
*/
static string M0 = "M0";

/* Stop Program
    Usage: M1
*/
static string M1 = "M1";

/* Turn on the ADC sampling
    Usage: T1
    Description: 
*/
static string T1 = "T1";

/* Turn off ADC sampling
    Usage: T2
    Description:
*/
static string T2 = "T2";

bool Instruction::exists(string input) {
    return true;
}
