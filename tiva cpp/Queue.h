#include <string.h>
#define DEFAULT_QUEUE_CAPACITY 100
#define DEFAULT_QUEUE_INSTRUCTION_SIZE 10
// Class for queue
class Queue
{
    char arr[DEFAULT_QUEUE_CAPACITY][DEFAULT_QUEUE_INSTRUCTION_SIZE];     // array to store queue elements
    static const int capacity = DEFAULT_QUEUE_CAPACITY; // maximum capacity of the queue
    int front;    // front points to front element in the queue (if any)
    int rear;     // rear points to last element in the queue
    int count;    // current size of the queue

public:
    Queue(); // constructor
    //~Queue();               // destructor

    void dequeue();
    void enqueue(char* item);
    char* peek();
    int size();
    bool isEmpty();
    bool isFull();
};
