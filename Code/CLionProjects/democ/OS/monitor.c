#include<stdio.h>

#define BUFFER_SIZE 100
typedef int bool;
#define true 1
#define false 0

int in, out, count;
int buffer[BUFFER_SIZE];
condition empty, full;
condition x, y;
bool busy_put, busy_out;

// To add anything that you think is necessary
void put (int *item) {
// Your code here
    if(count == BUFFER_SIZE)
            full.wait();

    if(busy_put)
        x.wait();
    busy_put = true;

    buffer[in] = *item;
    in = (in+1) % BUFFER_SIZE;
    count++;

    empty.signal();
    x.signal();
    busy_put = false;
}
void get (int *item) {
// your code here
    if(count == 0)
        empty.wait();

    if(busy_out)
        y.wait();
    busy_out = true;

    *item = buffer[out];
    out = (out+1) % BUFFER_SIZE;
    count--;

    full.signal();
    y.signal();
    busy_out = false;
}
initialization code() {
// your code here
    busy_put = false;
    busy_out = false;
}

/*
monitor ProducerConsumer{
        int in, out, count;
        Item buffer[BUFFER_SIZE];
        condition empty, full;
// To add anything that you think is necessary
        void put (Item &item) {
// Your code here
        }
        void get (Item &item) {
// your code here
        }
        initialization code() {
// your code here
        }
}


// Process Producer :
while(true) {
// produce an item in next_produced
ProducerConsumer. put(next_produced);
}
// Process Consumer :
while(true) {
ProducerConsumer. get(next_consumed);
// consume the item in next_consumed
}

*/

void put (int *item) {
// Your code here
    if(count == BUFFER_SIZE)
        full.wait();

    x.wait();

    buffer[in] = *item;
    in = (in+1) % BUFFER_SIZE;
    count++;

    empty.signal();
    x.signal();
}
void get (int *item) {
// your code here
    if(count == 0)
        empty.wait();

    y.wait();

    *item = buffer[out];
    out = (out+1) % BUFFER_SIZE;
    count--;

    full.signal();
    y.signal();
}
initialization code() {
// your code here
    busy_put = false;
    busy_out = false;
}
