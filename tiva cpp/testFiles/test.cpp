#include <string.h>
#include <stdio.h>
#include "../Instruction.h"


int main(){
    char myInstruction[20] = "G00 X1.5";

    instruction parsedInstruction = Instruction::splitInstruction(myInstruction, " ");
    printf("%d\n", parsedInstruction.parameterCount);
    for (int i = 0; i < parsedInstruction.parameterCount; i++){
        printf("%s\n", parsedInstruction.parameters[i]);
    }
}