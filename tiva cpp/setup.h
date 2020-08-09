class Setup {
    public:

        static void SensorADCSetup(void);
        static void SensorGPIOSetup(void);
        static void SensorTimerSetup(void);
    
        static void SampleDurationTimer(void);

        static void Uart0TerminalSetup(void);

        static void DriverGPIOSetup(void);
        static void LimitSwitchesGPIOSetup(void);

        static void StepSizeGPIOSetup(void);

        static void DriverTimerSetup(void);

        static void sysTickSamplingTimerSetup(void);

};
