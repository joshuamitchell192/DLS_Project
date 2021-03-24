enum StepModes {
    Full = 2,
    Half = 1,
    Quarter = 0
};

class MotorDriver{
    public:
        MotorDriver();

        // The stepper motor driver step size
        // This is a double to retain precision
        double stepsPerMM;
        int currentPosition;
        double sampleDuration;
        double motorStepDelay;
        // The distance to move between sampling during scanbetween.
        double stepSize;
        // Based on the stepMode to calculate the number of steps that are required for each mode.
        int stepAmount;
        int sampleTotal;
        int numSamples;
        int stepsBetweenSamples;
        float totalTimeElapsed;
        double averageInterval;
        int calibrationStepCount;

        void Calibrate(bool &stop);
        void StepSizeMove(bool &stop, int dest);
        static void SetMotorStepDelayTimer(double seconds);
        void SetSampleDurationTimer(double seconds);
        void RunSampleDurationTimer();
        static void StepMotor(void);
        void StartSamplingHere(bool &stop);
        void GoToPosition(bool &stop, int dest);
        void ScanBetween(bool &stop, int dest);
        void SetAverageInterval(float averageInterval_);
        void SetStepMode(int stepMode);
        void SetStepsBetweenSamples(double stepLength);
        void SetSampleDuration(double sampleDuration);
        void Move(bool &stop, double dest);
        int SetDirection(int dest);
        void WaitForSamples(double duration);
        float CalculateCurrentTime();
        void OffSetStage(bool &stop, int direction);
        
        static void SendInt(int input);
        
        static bool IsSwitchB2On();
        static bool IsSwitchB1On();
        static bool IsAdcOn();
        static void TurnAdcOn();
        static void TurnAdcOff();
};