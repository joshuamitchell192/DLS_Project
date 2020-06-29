#include <cstring.h>

// Class for queue
class Queue
{
    char arr[100][10];     // array to store queue elements
    int capacity = 100; // maximum capacity of the queue
    int front;    // front points to front element in the queue (if any)
    int rear;     // rear points to last element in the queue
    int count;    // current size of the queue

public:
    Queue(); // constructor
    ~Queue();               // destructor

    void dequeue();
    void enqueue(char* item);
    char* peek();
    int size();
    bool isEmpty();
    bool isFull();
};
