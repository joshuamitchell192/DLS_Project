#include "Instruction.hpp"

/* Rapid positioning
    Usage: G0 x
    Description: Moves to the postion defined by x at the maximum possible speed.
*/
static String G0 = "G0";

/* Linear positioning
    Usage: G1 x
    Description: Moves to the postion defined by x at the speed determined by the current state of the stage.
*/
static String G1 = "G1";

// Stop Program
static String M0 = "M0";

bool Instruction::exists(String input) {
    return true;
}
