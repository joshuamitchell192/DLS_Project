#include <WString.h>
// define default capacity of the queue
#define SIZE 10

// Class for queue
class Queue
{
    String *arr;     // array to store queue elements
    int capacity; // maximum capacity of the queue
    int front;    // front points to front element in the queue (if any)
    int rear;     // rear points to last element in the queue
    int count;    // current size of the queue

public:
    Queue(int size = SIZE); // constructor
    ~Queue();               // destructor

    void dequeue();
    void enqueue(String x);
    String peek();
    int size();
    bool isEmpty();
    bool isFull();
};
