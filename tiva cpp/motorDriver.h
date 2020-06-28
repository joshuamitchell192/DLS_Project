class MotorDriver{
    public:
        int calibrate(void);
        void setDriverTimer(double seconds);
        void stepMotor(void);
        void sampleHere(int duration,int avgInterval);
        void goTo(int point);
        static void setStepAmount(int stepAmount);
        void sampleTwoPoints(double sampleDuration, int point1, int point2);
};