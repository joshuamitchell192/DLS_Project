#include <string.h>

struct instruction{
  int parameterCount;
  char *instruction;
  char **parameters;
};

class Instruction {

public:

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
        Description: Runs a delay loop waiting for a start instruction to be sent.
    */
    static const char *M02;
    
    /* End Program
        Usage: M03  
    */
    static const char *M03;

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
    
    /* Set Sample Duration
        Usage S1 100
        Description:
    */
    static const char *S1;
    
    /* Set Step Mode
        Usage: S2 1.5
        Description: 
    */
   static const char *S2;

    /* Set StepsBetweenSamples
        Usage: S3 1.5
        Description: 
    */
   static const char *S3;

   /* Set Average Interval
        Usage: S4 1.5
        Description: 
   */
  static const char *S4;
  
  /* Calibrate Stage
        Usage: M04
        Description:
  */
  static const char *M04;

  static bool exists(char *input);
  static instruction splitInstruction(char *instruction, char *delimiters);

};
