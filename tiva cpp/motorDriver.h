
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
        double stepsPerMM;
        int currentPosition;
        double sampleDuration;
        // The distance to move between sampling during scanbetween.
        double stepSize;
        // Based on the stepMode to calculate the number of steps that are required for each mode.
        int stepAmount;

        void calibrate(bool &stop);
        static void setDriverTimer(double seconds);
        static void stepMotor(void);
        // void sampleHere(int duration,int avgInterval);
        void goTo(bool &stop, int dest);
        // static void scanBetween(int &currentPosition, int &stop, float stepsPerMM, int dest, double stepSize, double sampleDuration, int stepAmount, int &sampleTotal, int &numSamples);
        void scanBetween(bool &stop, int dest);
        static void setStepAmount(int stepAmount);
        // void sampleTwoPoints(double sampleDuration, int point1, int point2);
        void move(bool &stop, int dest, bool setMaxSpeed);
        
        static void sendInt(int input);
        
        static bool IsAdcOn();
        static void TurnAdcOn();
        static void TurnAdcOff();
};