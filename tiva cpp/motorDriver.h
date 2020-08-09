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
        volatile double sampleDuration_;
        // The distance to move between sampling during scanbetween.
        double stepSize;
        // Based on the stepMode to calculate the number of steps that are required for each mode.
        int stepAmount;
        int sampleTotal;
        int numSamples;
        int stepsBetweenSamples;

    
        
        void Calibrate(bool &stop);
        void StepSizeMove(bool &stop, int dest);
        static void SetDriverTimer(double seconds);
        static void StepMotor(void);
        void GoTo(bool &stop, int dest);
        void ScanBetween(bool &stop, int dest);
        void SetStepMode(int stepMode);
        void SetStepsBetweenSamples(double stepLength);
        void Move(bool &stop, int dest, bool setMaxSpeed);
        int SetDirection(int dest);
        void MoveToNextSamplePosition(bool &stop, int dest, double sampleDuration);
        
        static void SendInt(int input);
        
        static bool IsAdcOn();
        static void TurnAdcOn();
        static void TurnAdcOff();
};