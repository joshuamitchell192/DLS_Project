class Serial
{
    public:
        static void SendSampleAverage(int &sampleTotal, int &numSamples);
        static void SendTime(float Time);
        static void WriteChar(unsigned int value);
        static unsigned int ReadChar();
        static void WriteString(const char* string);
        static void SendInt(int input);
        static void SendFloat(float input);
};