#include "DLS.h"
 
DLS::DLS(){
 
}
 
void DLS::ReadSerial(char c){
    char inputString[10] = "";
    //char c = Helpers::ReadChar();
    Helpers::WriteChar(c);
    
    if (c == '\n'){
        strcpy(inputString, "");
        //inputString = ;
        return;
    }
    strcat(inputString, &c);
}
 
class DLS {
private:
	int inputCharPos = 0;
    char inputChar[10] = "";
 
    Queue *queue;
    Instruction instruction;
 
    const int directionPin = 4;
    const int stepPin = 5;
 
    // MAX LENGTH - 6600
    const int StepsPerRev = 6600;
 
public:
 
    DLS::DLS (Queue *_queue) {
        queue = _queue;
    }
 
    void DLS::eventLoop(){
        for (;;) {
            String currentInstruction = queue->peek();
            if (strcmp(currentInstruction, "G0") == 0) {
                spinMotor();
            }else if (){

            }else if (){

            }
        }
    }
 
    
    void spinMotor() {
        for (int x = 0; x < StepsPerRev; x++) {
            digitalWrite(stepPin, HIGH);
            
            delayMicroseconds(800);
            digitalWrite(stepPin, LOW);
            
            delayMicroseconds(800);
 
        }
    }
    
    void DLS::readSerial(char inChar){
    	inputString[inputCharPos] = inChar;
        inputCharPos++;

        if (inChar == '\n') {
          	queue->enqueue(inputString);

          	inputCharPos = 0;
          	break;
        }
    }

};