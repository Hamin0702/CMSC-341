#include "bufferlist.h"

//Alternative constructor
BufferList::BufferList(int minBufCapacity){
    
    //If the input value is less than 1, use the default minimun capacity value
    if(minBufCapacity < 1){
        
        //Creating the first buffer that becomes m_cursor and m_cursor's next
        Buffer* B = new Buffer(DEFAULT_MIN_CAPACITY);
        m_cursor = B;
        m_cursor->m_next = m_cursor;
        
        //Initializing member variables
        m_minBufCapacity = DEFAULT_MIN_CAPACITY;
        m_listSize = 1;
        
    //All other cases
    }else{
        
        //Creating the first buffer that becomes m_cursor and m_cursor's next
        Buffer* B = new Buffer(minBufCapacity);
        m_cursor = B;
        m_cursor->m_next = m_cursor;
        
        //Initializing member variables
        m_minBufCapacity = minBufCapacity;
        m_listSize = 1;
    }
}

//Destructor
BufferList::~BufferList(){
    clear();
}

//Clear function
void BufferList::clear(){
    
    //Pointer to loop through the list
    Buffer* loop = m_cursor->m_next;
    Buffer* track = m_cursor->m_next->m_next;
    //Deallocates memory by clearing all the Buffers in the list
    while(loop != m_cursor){
        loop->clear();
        delete loop;
        loop = track;
        if(track != m_cursor){
            track = track->m_next;
        }
        
    }
    loop->clear();
    delete loop;
    
    //Resinitializing member variables
    m_listSize = 0;
    m_minBufCapacity = 0;
}

//Enqueue function
void BufferList::enqueue(const int& data) {
    
    //First try to enqueue a data item to the last Buffer in the linked list
    try {
        m_cursor->enqueue(data);
        
    //Catch error if the Buffer was full and threw an error
    } catch (overflow_error &e) {
        
        //The size of the new Buffer is the last Buffer's size times INCREASE_FACTOR
        int size = m_cursor->capacity() * INCREASE_FACTOR;
        //If the size is greater than the maximum limit, the new Buffer's size is m_minBufCapacity
        if(size > (MAX_FACTOR * m_minBufCapacity)){
            size = m_minBufCapacity;
        }
        
        //Creates the new Buffer and adds it to the linked list and makes it m_cursor
        Buffer* B = new Buffer(size);
        B->m_next = m_cursor->m_next;
        m_cursor->m_next = B;
        m_cursor = B;
        m_listSize++;
        
        //Finally enqueue the data item
        m_cursor->enqueue(data);
    }
}

//Dequeue function
int BufferList::dequeue() {
    int data = 0;
    
    //First try to dequeue a data item from the oldest Buffer in the linked list
    try {
        data = m_cursor->m_next->dequeue();
        
    //Catch error if the Buffer was empty and threw an error
    } catch (underflow_error &e) {
        
        //The case where m_cursor->next is empty but is also the only Buffer in the linked list
        if(m_listSize == 1){
            throw underflow_error("Oops, the linked list has one empty Buffer");
        //All other cases where m_cursor->next is empty
        }else{
            
            //Remove the empty Buffer in the linked list and updates the pointers
            Buffer *empty = m_cursor->m_next;
            m_cursor->m_next = m_cursor->m_next->m_next;
            //Deallocating memory
            empty->clear();
            delete empty;
            
            //Dequeue from the new linked list
            data = m_cursor->m_next->dequeue();
            m_listSize--;
        }
    }
    return data;
}

//Copy constructor
BufferList::BufferList(const BufferList & rhs){
    
    //Copy member variables
    m_listSize = rhs.m_listSize;
    m_minBufCapacity = rhs.m_minBufCapacity;
    
    //Sets up Buffer pointers for the loop
    Buffer* source = rhs.m_cursor;
    Buffer* curr = new Buffer(*rhs.m_cursor);
    m_cursor = curr;
    
    source = source->m_next;
    //Loops through and copies the Buffers and connects them
    while(source != rhs.m_cursor){
        curr->m_next = new Buffer(*source);
        source = source->m_next;
        curr = curr->m_next;
    }
    //Connecting the last Buffer from the loop to m_cursor (makes it circular)
    curr->m_next = m_cursor;
}

const BufferList & BufferList::operator=(const BufferList & rhs){
    
    //Checks self assignment
    if(this == &rhs){
        return *this;
    }else{
        
        //Deallocates memory 
        clear();
        
        //Copy member variables
        m_listSize = rhs.m_listSize;
        m_minBufCapacity = rhs.m_minBufCapacity;
        
        //Sets up Buffer pointers for the loop
        Buffer* source = rhs.m_cursor;
        Buffer* curr = new Buffer(*rhs.m_cursor);
        m_cursor = curr;
        
        source = source->m_next;
        //Loops through and copies the Buffers and connects them
        while(source != rhs.m_cursor){
            curr->m_next = new Buffer(*source);
            source = source->m_next;
            curr = curr->m_next;
        }
        //Connecting the last Buffer from the loop to m_cursor (makes it circular)
        curr->m_next = m_cursor;
    }
    return *this;
}

//Dump function
void BufferList::dump(){
    Buffer* temp = m_cursor->m_next;
    for(int i=0;i<m_listSize;i++){
        temp->dump();
        temp = temp->m_next;
        cout << endl;
    }
    cout << endl;
}
