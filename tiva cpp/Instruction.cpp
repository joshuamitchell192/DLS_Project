#include "Instruction.h"

/* Rapid positioning
    Usage: G0 X2.5
    Description: Moves to the postion defined by x at the maximum possible speed.
*/
static char G00[4] = "G00";

/* Linear positioning
    Usage: G1 X2.5
    Description: Moves to the postion defined by x at the speed determined by the current state of the stage.
*/
static char G01[4] = "G01";

/* Start Program
    Usage: M00
*/
static char M00[4] = "M00";

/* Stop Program
    Usage: M01
*/
static char M01[4] = "M01";

/* Turn on the ADC sampling
    Usage: T1
    Description: 
*/
static char T1[3] = "T1";

/* Turn off ADC sampling
    Usage: T2
    Description:
*/
static char T2[3] = "T2";

bool Instruction::exists(char *instruction) {
    switch (instruction[0]){
        case 'G':
            if (instruction[1] == '0') {
                switch (instruction[2]){
                    case '0':
                    case '1':
                        return true;
                }
            }
            return false;

        case 'M':
            if (instruction[1] == '0') {
                switch (instruction[2]){
                    case '0':
                    case '1':
                    case '2':
                        return true;
                }
            }
            return false;

        case 'T':
            switch(instruction[1]){
                case '0':
                case '1':
                    return true;
            }
            return false;
    }
}
