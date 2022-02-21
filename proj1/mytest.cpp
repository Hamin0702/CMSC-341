#include "bufferlist.h"
class Tester{
    public:
    bool BufferEnqueueDequeue(Buffer &buffer, int dataCount);
    bool BufferEnqueueFull(int size);
    bool BufferDequeueEmpty();
    bool BufferCopyConstructor(const Buffer &buffer);
    bool BufListEnqueueDequeue(BufferList &buflist, int dataCount);
    void BufListEnqueuePerformance(int numTrials, int N);
    bool BufListDequeueEmpty();
    bool BufListCopyConstructor(const BufferList &buflist);
    bool BufListAssignmentOperator(BufferList &buflist);
};

int main(){
    Tester tester;
    int bufferSize = 1000;
    Buffer buffer(bufferSize);
    cout << "*********************************" << endl;
    cout << "Buffer Class Testing" << endl;
    cout << "*********************************" << endl;
    {
        //testing insertion/removal for 1 data item
        cout << "\nTest case: Buffer class: Inserting/removing 1 data item:\n";
        if (tester.BufferEnqueueDequeue(buffer, 1))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
     
    {
        //testing insertion/removal for half data size
        cout << "\nTest case: Buffer class: Inserting/removing half data size:\n";
        if (tester.BufferEnqueueDequeue(buffer, bufferSize/2))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing insertion/removal for full data size
        cout << "\nTest case: Buffer class: Inserting/removing full data size:\n";
        if (tester.BufferEnqueueDequeue(buffer, bufferSize))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing insertion in a full buffer
        cout << "\nTest case: Buffer class: Throwing exception while inserting in full buffer:\n";
        if (tester.BufferEnqueueFull(1000))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing removal in an empty buffer
        cout << "\nTest case: Buffer class: Throwing exception while removing in empty buffer:\n";
        if (tester.BufferDequeueEmpty())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing Buffer class copy constructor with one item
        cout << "\nTest case: Buffer class: Copy constructor, one item:\n";
        Buffer buffer(10);
        buffer.enqueue(1);
        if (tester.BufferCopyConstructor(buffer))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing Buffer class copy constructor with half capacity
        cout << "\nTest case: Buffer class: Copy constructor, half capacity:\n";
        Buffer buffer(10);
        for(int i = 0; i < 5; i++){
            buffer.enqueue(i);
        }
        if (tester.BufferCopyConstructor(buffer))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing Buffer class copy constructor with full capacity
        cout << "\nTest case: Buffer class: Copy constructor, full capacity:\n";
        Buffer buffer(10);
        for(int i = 0; i < 10; i++){
            buffer.enqueue(i);
        }
        if (tester.BufferCopyConstructor(buffer))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing Buffer class copy constructor with buffer size less than zero
        cout << "\nTest case: Buffer class: Copy constructor, negative buffer size:\n";
        Buffer buffer(-10);
        if (tester.BufferCopyConstructor(buffer))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    
    cout << "\n*********************************" << endl;
    cout << "BufferList Class Testing" << endl;
    cout << "*********************************" << endl;
    {
        //testing insertion/removal for 1 data item
        cout << "\nTest case: BufferList class: Inserting/removing 1 data item:\n";
        BufferList bufferList(10);
        if (tester.BufListEnqueueDequeue(bufferList, 1))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing insertion/removal for 10000 data item
        cout << "\nTest case: BufferList class: Inserting/removing 10000 data items:\n";
        BufferList bufferList(100);
        if (tester.BufListEnqueueDequeue(bufferList, 10000))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing removal in an empty bufferlist with one buffer
        cout << "\nTest case: BufferList class: Throwing exception while removing in empty bufferlist with one buffer:\n";
        if (tester.BufListDequeueEmpty())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing BufferList class copy constructor with one item
        cout << "\nTest case: BufferList class: Copy constructor, one item:\n";
        BufferList bufList(10);
        bufList.enqueue(1);
        if (tester.BufListCopyConstructor(bufList))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing BufferList class copy constructor with many items
        cout << "\nTest case: BufferList class: Copy constructor, 10000 data items:\n";
        BufferList bufList(1000);
        for(int i = 0; i < 10000; i++){
            bufList.enqueue(1);
        }
        if (tester.BufListCopyConstructor(bufList))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing BufferList class copy constructor with input size less than zero
        cout << "\nTest case: BufferList class: Copy constructor, negative buffer size:\n";
        BufferList bufList(-10);
        if (tester.BufListCopyConstructor(bufList))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing BufferList class assignment operator
        cout << "\nTest case: BufferList class: Assignment Operator:\n";
        BufferList bufList(10);
        bufList.enqueue(1);
        if (tester.BufListAssignmentOperator(bufList))
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //Measuring the efficiency of insertion functionality
        cout << "\nMeasuring the efficiency of insertion functionality:" << endl;
        int M = 4;//number of trials
        int N = 10000;//original input size
        tester.BufListEnqueuePerformance(M,N);
    }
    {
        //an example of BufferList::dump()
        cout << "\nHere is an example of a linked list:" << endl;
        BufferList bufferList(5);
        for (int i=40;i>0;i--)
            bufferList.enqueue(i);
        bufferList.dump();
        
        //an example of BufferList::dump() using Copy Constructor
        cout << "\nHere is the copy of that list (Copy Constructor):" << endl;
        BufferList Constructor(bufferList);
        Constructor.dump();
        
        //an example of BufferList::dump() using Assignment Operator
        cout << "\nHere is the copy of that list (Assignment Operator):" << endl;
        BufferList Operator(bufferList);
        Operator.dump();
    }
    {
        //an example of BufferList::dump()
        cout << "Here is another example of a linked list:" << endl;
        BufferList bufferList(5);
        for (int i = 0; i < 10; i++){
            bufferList.enqueue(i);
        }
        cout << "\nBefore dequeue" << endl;
        bufferList.dump();
        for (int i = 0; i < 6; i++){
            bufferList.dequeue();
        }
        cout << "After dequeue" << endl;
        bufferList.dump();
    }
    return 0;
}

//************************
// Buffer class tests
//************************

//Tests simple enqueue and dequeue cases
bool Tester::BufferEnqueueDequeue(Buffer &buffer, int dataCount){
    try{
        for(int i = 0; i < dataCount; i++){
            buffer.enqueue(1);
        }
        for(int i = 0; i < dataCount; i++){
            buffer.dequeue();
        }
    }
    
    //There should not be any exceptions thrown
    catch(overflow_error &e){
        return false;
    }
    catch(underflow_error &e){
        return false;
    }
    catch(...){
        return false;
    }
    
    return true;
}

//Tests the exception case of enqueueing a full Buffer
bool Tester::BufferEnqueueFull(int size){
    Buffer aBuffer(size);
    for (int i=0;i<size;i++)
        aBuffer.enqueue(i);
    try{
        //trying to insert in a full buffer
        aBuffer.enqueue(size+1);
    }
    catch(overflow_error &e){
        //the exception that we expect
        return true;
    }
    catch(...){
        //any other exception, this case is not acceptable
        return false;
    }
    //no exception thrown, this case is not acceptable
    return false;
}

//Tests the exception case of dequeueing an empty Buffer
bool Tester::BufferDequeueEmpty(){
    Buffer aBuffer(0);
    try{
        //trying to remove in an empty buffer
        aBuffer.dequeue();
    }
    catch(underflow_error &e){
        //the exception that we expect
        return true;
    }
    catch(...){
        //any other exception, this case is not acceptable
        return false;
    }
    //no exception thrown, this case is not acceptable
    return false;
}

//Tests if the copy constructor makes a deep copy
bool Tester::BufferCopyConstructor(const Buffer &buffer){
    Buffer copy(buffer);
    
    //The case if both are empty
    if(copy.count() < 1 && buffer.m_count < 1){
        return true;
        
    //If the pointers don't match but all the data match
    }else if(copy.m_buffer != buffer.m_buffer && copy.count() == buffer.m_count && copy.capacity() == buffer.m_capacity){
        for(int i = 0; i < copy.capacity(); i++){
            if(copy.m_buffer[i] != buffer.m_buffer[i]){
                return false;
            }
        }
        return true;
    }
    
    //Other cases should be false
    return false;
    
}

//************************
// BufferList class tests
//************************

//Tests inserting and dequeing BufferList
bool Tester::BufListEnqueueDequeue(BufferList &buflist, int dataCount){
    try{
        for(int i = 0; i < dataCount; i++){
            buflist.enqueue(1);
        }
        for(int i = 0; i < dataCount; i++){
            buflist.dequeue();
        }
    }
    
    //Should never be full
    catch(overflow_error &e){
        return false;
    }
    //Should throw an underflow if it tries removing from an empty BufferList
    catch(underflow_error &e){
        return true;
    }
    //There shouldn't be other exceptions
    catch(...){
        return false;
    }
    
    return true;
}

//Tests the exception case of dequeueing an empty BufferList
bool Tester::BufListDequeueEmpty(){
    Buffer aBuffer(5);
    try{
        //trying to remove when there's only one empty buffer in the list
        aBuffer.dequeue();
    }
    catch(underflow_error &e){
        //the exception that we expect
        return true;
    }
    catch(...){
        //any other exception, this case is not acceptable
        return false;
    }
    //no exception thrown, this case is not acceptable
    return false;
}

//Tests if the copy constructor makes a deep copy
bool Tester::BufListCopyConstructor(const BufferList &buflist){
    BufferList copy(buflist);
    Buffer* trackCopy = copy.m_cursor->m_next;
    Buffer* trackRhs = buflist.m_cursor->m_next;

    //Loops through the BufferList
    while(trackCopy != copy.m_cursor && trackRhs != buflist.m_cursor){
        
        if(trackCopy->count() < 1 && trackRhs->m_count < 1){
            return true;
            
        //If the pointers don't match but all the data match
        }else if(trackCopy->m_buffer != trackRhs->m_buffer && trackCopy->count() == trackRhs->m_count && trackCopy->capacity() == trackRhs->m_capacity){
            for(int i = 0; i < trackCopy->capacity(); i++){
                if(trackCopy->m_buffer[i] != trackRhs->m_buffer[i]){
                    return false;
                }
            }
            return true;
        }
        
        //Moves on to the next Buffer in the list
        trackCopy = trackCopy->m_next;
        trackRhs = trackRhs->m_next;
    }
    
    //Checking one last time for m_cursor
    if(trackCopy->capacity() == 0 && trackRhs->m_capacity == 0 && trackCopy->count() == 0 && trackRhs->m_count == 0){
        return true;
        
    //If the pointers don't match but all the data match
    }else if(trackCopy->m_buffer != trackRhs->m_buffer && trackCopy->count() == trackRhs->m_count && trackCopy->capacity() == trackRhs->m_capacity){
        for(int i = 0; i < trackCopy->capacity(); i++){
            if(trackCopy->m_buffer[i] != trackRhs->m_buffer[i]){
                return false;
            }
        }
        return true;
    }
    
    //Any other cases
    return false;
}

//Tests if the Assignment Operator makes a deep copy
bool Tester::BufListAssignmentOperator(BufferList &buflist){
    BufferList copy = buflist;
    Buffer* trackCopy = copy.m_cursor->m_next;
    Buffer* trackRhs = buflist.m_cursor->m_next;

    //Loops through the BufferList
    while(trackCopy != copy.m_cursor && trackRhs != buflist.m_cursor){
        
        if(trackCopy->count() < 1 && trackRhs->m_count < 1){
            return true;
            
        //If the pointers don't match but all the data match
        }else if(trackCopy->m_buffer != trackRhs->m_buffer && trackCopy->count() == trackRhs->m_count && trackCopy->capacity() == trackRhs->m_capacity){
            for(int i = 0; i < trackCopy->capacity(); i++){
                if(trackCopy->m_buffer[i] != trackRhs->m_buffer[i]){
                    return false;
                }
            }
            return true;
        }
        
        //Moves on to the next Buffer in the list
        trackCopy = trackCopy->m_next;
        trackRhs = trackRhs->m_next;
    }
    
    //Checking one last time for m_cursor
    if(trackCopy->capacity() == 0 && trackRhs->m_capacity == 0 && trackCopy->count() == 0 && trackRhs->m_count == 0){
        return true;
        
    //If the pointers don't match but all the data match
    }else if(trackCopy->m_buffer != trackRhs->m_buffer && trackCopy->count() == trackRhs->m_count && trackCopy->capacity() == trackRhs->m_capacity){
        for(int i = 0; i < trackCopy->capacity(); i++){
            if(trackCopy->m_buffer[i] != trackRhs->m_buffer[i]){
                return false;
            }
        }
        return true;
    }
    
    //Any other cases
    return false;
}

void Tester::BufListEnqueuePerformance(int numTrials, int N){
    //Measuring the efficiency of inserting with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k = 0; k < numTrials; k++){
        BufferList time(1000);
        start = clock();
        //the algorithm to be analyzed for efficiency
        for(int i = 0; i < N; i++){
            time.enqueue(1);
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Inserting " << N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        N = N * a;//increase the input size by the scaling factor
    }
}
