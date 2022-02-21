#include "buffer.h"

//Overloaded Constructor
Buffer::Buffer(int capacity){
    m_capacity = capacity;
    if(capacity < 1){
        m_capacity = 0;
    }
    
    //Allocating memory for the array based on given size
    if(m_capacity > 0){
        m_buffer = new int[m_capacity];
        for(int i = 0; i < m_capacity; i++){
            m_buffer[i] = 0;
        }
    }
    
    //Initializing member variables
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

//Clear function
void Buffer::clear(){
    
    //Deallocating memory of the array
    if(m_capacity > 0){
        delete [] m_buffer;
        m_buffer = nullptr;
    }
    
    //Reinitializing member variables
    m_capacity = 0;
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

//Destructor
Buffer::~Buffer(){
    clear();
}

//Getter function for m_count
int Buffer::count(){
    return m_count;
}

//Getter function for m_capacity
int Buffer::capacity(){
    return m_capacity;
}

//Checks if the array is full
bool Buffer::full(){
    if(m_count == m_capacity){
        return true;
    }else{
        return false;
    }
}

//Checks if the array is empty
bool Buffer::empty(){
    if(m_count == 0){
        return true;
    }else{
        return false;
    }
}

//Enqueue method that adds to the last element
void Buffer::enqueue(int data){
    
    //Checks if the array is full
    if(full()){
        //throw exception overflow_error
        throw overflow_error("Trying to enqueue when it's full");
    }else{
        
        //If there is nothing in the array, m_end and m_start stay pointing to the same element after enqueue
        if(count() == 0){
            m_buffer[m_end] = data;
            m_count++;
            
        //If the last element of the queue is the last element of the array
        }else if(m_end+1 == capacity()){
            m_end = 0;
            m_buffer[m_end] = data;
            m_count++;
            
        //All other cases
        }else{
            m_end++;
            m_buffer[m_end] = data;
            m_count++;
        }
        
    }
}

//Dequeue method that takes out the first element
int Buffer::dequeue(){
    int value = 0;
    
    //Checks if the array is empty
    if(empty()){
        //throw exception underflow_error
        throw underflow_error("Trying to dequeue into a full array");
    }else{
        
        //If there's only one element, m_start and m_end stay pointing to the same element after dequeue
        if(count() == 1){
            value = m_buffer[m_start];
            m_buffer[m_start] = 0;
            m_count--;
            
        //If the first element of the queue is the last element of the array
        }else if(m_start+1 == capacity()){
            value = m_buffer[m_start];
            m_buffer[m_start] = 0;
            m_start = 0;
            m_capacity--;
            
        //All other cases
        }else{
            value = m_buffer[m_start];
            m_buffer[m_start] = -1;
            m_start++;
            m_count--;
        }
    }
    return value;
}


//Copy constructor
Buffer::Buffer(const Buffer & rhs){
    
    //Copys the size
    m_capacity = rhs.m_capacity;
    
    //Allocates memory for the copy
    if(m_capacity > 0){
           m_buffer = new int[m_capacity];
       }
    
    //Deep copying the values of the array.
    for(int i = 0; i < m_capacity; i++){
        m_buffer[i] = rhs.m_buffer[i];
    }
       
    //Copying the member variables
    m_count = rhs.m_count;
    m_start = rhs.m_start;
    m_end = rhs.m_end;
}

//Assignment operator
const Buffer & Buffer::operator=(const Buffer & rhs){
    
    //Checks self assignment
    if(this == &rhs){
        return *this;
    }else{
        
        //Deallocates memory of existing array
        clear();
        
        //Allocates memory for the copy
        m_capacity = rhs.m_capacity;
        if(m_capacity > 0){
               m_buffer = new int[m_capacity];
           }
        
        //Deep copying the values of the array.
        for(int i = 0; i < m_capacity; i++){
            m_buffer[i] = rhs.m_buffer[i];
        }
           
        //Copying the member variables
        m_count = rhs.m_count;
        m_start = rhs.m_start;
        m_end = rhs.m_end;
    }
    
    return *this;
}

//Dump function that prints the buffer
void Buffer::dump(){
    int start = m_start;
    //int end = m_end;
    int counter = 0;
    cout << "Buffer size: " << m_capacity << " : ";
    if (!empty()){
        while(counter < m_count){
            cout << m_buffer[start] << "[" << start << "]" << " ";
            start = (start + 1) % m_capacity;
            counter++;
        }
        cout << endl;
    }
    else cout << "Buffer is empty!" << endl;
}
