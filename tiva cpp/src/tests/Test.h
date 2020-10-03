class Test{
    public:
        void testStepperMotorDrift();

        // Test by sending values from the tiva and wait for them to be returned
        void testSerialCommunication();

        void testAdc();

        // Ensure that the queue is enquing and dequeing effectively etc as expected.
        void testQueue();

        
};