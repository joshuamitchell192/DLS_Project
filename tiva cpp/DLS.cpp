
#include "DLS.h"


DLS::DLS(){

}

void DLS::ReadSerial(){
    char inputString[10] = "";
    char c = Helpers::ReadChar();
    Helpers::WriteChar(c);
    
    if (c == '\n'){
        strcpy(inputString, "");
        //inputString = ;
        return;
    }
    strcat(inputString, &c);
}