class Setup {
    public:

        static void SensorADCSetup(void);
        static void SensorGPIOSetup(void);
        static void SensorTimerSetup(void);
    
        static void MotorStepDelayTimerSetup(void);

        static void Uart0TerminalSetup(void);

        static void DriverGPIOSetup(void);
        static void LimitSwitchesGPIOSetup(void);

        static void StepSizeGPIOSetup(void);

        static void SampleDurationTimerSetup(void);

        static void sysTickSamplingTimerSetup(void);

        static void TimeElapsedTimer(void);
};
