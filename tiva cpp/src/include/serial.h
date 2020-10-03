class Serial
{
    public:
        
        static const char sample_type;
        static const char time_type;
        static const char position_type;
        
        static void SendSampleAverage(int &sampleTotal, int &numSamples);
        static void SendTime(float Time);
        static void WriteChar(unsigned int value);
        static void WriteChar(unsigned int value, char type, bool sendCrc);
        static void WriteCrc(unsigned char * value);
        static unsigned int ReadChar();
        static void WriteString(const char* string);
        static void SendInt(int input);
        static void SendFloat(float input);
        static void WriteFlag(int flag);
    
        static unsigned char * floatToBytes(unsigned char bytes[4], float input);
        static unsigned char * intToBytes(unsigned char bytes[2], int input);
};