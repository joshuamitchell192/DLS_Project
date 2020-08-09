class Serial
{
    public:
        static void SendSample(int sampleAverage);
        static void SendTime(int Time);
        static void WriteChar(unsigned int value);
        static unsigned int ReadChar();
        static void WriteString(const char* string);
};