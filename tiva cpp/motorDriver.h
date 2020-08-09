
enum StepModes {
    Full = 2,
    Half = 1,
    Quarter = 0
};

class MotorDriver{
    public:
        MotorDriver();

        // The stepper motor driver step size
        StepModes stepMode;
        // This is a double to retain precision
        double stepsPerMM;
        int currentPosition;
        volatile double sampleDuration;
        // The distance to move between sampling during scanbetween.
        double stepSize;
        // Based on the stepMode to calculate the number of steps that are required for each mode.
        int stepAmount;
        int sampleTotal;
        int numSamples;
        int stepsBetweenSamples;

    
        
        void calibrate(bool &stop);
        void stepSizeMove(bool &stop, int dest);
        static void setDriverTimer(double seconds);
        static void stepMotor(void);
        void goTo(bool &stop, int dest);
        void ScanBetween(bool &stop, int dest);
        void setStepMode(int stepMode);
        void setStepsBetweenSamples(double stepLength);
        void move(bool &stop, int dest, bool setMaxSpeed);
        int SetDirection(int dest);
        void stepSizeMove(bool &stop, int dest, double sampleDuration);
        
        static void sendInt(int input);
        
        static bool IsAdcOn();
        static void TurnAdcOn();
        static void TurnAdcOff();
};