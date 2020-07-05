#include "Queue.h"
#include "helpers.h"

// TODO - Add error messaging

// Constructor to initialize queue
Queue::Queue()
{
    front = 0;
    rear = -1;
    count = 0;
}

// Destructor to free memory allocated to the queue
Queue::~Queue()
{
   for (int i = 0; i < DEFAULT_QUEUE_CAPACITY; i++){
       for (int j = 0; j < DEFAULT_QUEUE_INSTRUCTION_SIZE; j++){
           delete [] arr;
       }
       delete arr[i]
   }
   
}

// Utility function to remove front element from the queue
void Queue::dequeue()
{
    // check for queue underflow
    if (isEmpty())
    {
        Helpers::WriteString(EMPTY_QUEUE);
    }

    front = (front + 1) % capacity;
    count--;
}

// Utility function to add an item to the queue
void Queue::enqueue(char* item)
{
    // check for queue overflow
    if (isFull())
    {
        Helpers::WriteString(FULL_QUEUE);
        return;
    }

    // cout << "Inserting " << item << '\n';

    rear = (rear + 1) % capacity;
    strcpy(arr[rear], item);
    count++;
}

// Utility function to return front element in the queue
char* Queue::peek()
{
    if (isEmpty())
    {
        return NULL;
    }
    return arr[front];
}

char* Queue::get(int i){
    return arr[i];
}

// Utility function to return the size of the queue
int Queue::size()
{
    return count;
}

// Utility function to check if the queue is empty or not
bool Queue::isEmpty()
{
    return (size() == 0);
}

// Utility function to check if the queue is full or not
bool Queue::isFull()
{
    return (size() == capacity);
}