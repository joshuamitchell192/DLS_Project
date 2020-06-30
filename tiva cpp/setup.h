#include "registers.h"
// #include "motorDriver.h"
#include "helpers.h"

class Setup {
    public:

        static void SensorADCSetup(void);
        static void SensorGPIOSetup(void);
        static void SensorTimerSetup(void);
    
        static void SampleDurationTimer(void);

        static void Uart0TerminalSetup(void);

        void DriverGPIOSetup(void);
        void LimitSwitchesGPIOSetup(void);

        void StepSizeGPIOSetup(void);

        void DriverTimerSetup(void);

        void sysTickSamplingTimerSetup(void);

};
