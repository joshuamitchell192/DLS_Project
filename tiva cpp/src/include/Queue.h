#include <string.h>
#define DEFAULT_QUEUE_CAPACITY 300
#define DEFAULT_QUEUE_INSTRUCTION_SIZE 50


// Class for queue
class Queue
{
    char arr[DEFAULT_QUEUE_CAPACITY][DEFAULT_QUEUE_INSTRUCTION_SIZE];     // array to store queue elements
    static const int capacity = DEFAULT_QUEUE_CAPACITY; // maximum capacity of the queue
    int front;    // front points to front element in the queue (if any)
    int rear;     // rear points to last element in the queue
    int count;    // current size of the queue
    const char *FULL_QUEUE = "Queue is full";
    const char *EMPTY_QUEUE = "Queue is empty";

public:
    Queue(); // constructor
    //~Queue(); // destructor

    void dequeue();
    void enqueue(char* item);
    char* peek();
    char* get(int i);
    void clear();
    int size();
    bool isEmpty();
    bool isFull();
    void printToSerial();
};
