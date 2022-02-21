// CMSC 341 - Spring 2021 - Project 3
// mytest:

#include "rqueue.h"
using namespace std;

class Tester{
    public:
    bool basicCorrectNess();
    bool deepCopy();
    bool deepCopyEdge();
    void priorityChange();
    void efficiency(int numTrials, int N);
    bool mergeDifferntPriority();
    bool dequeEmpty();
};

// Priority functions compute an integer priority for a student. Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Student &student);
int priorityFn2(const Student &student);

int main() {
    Tester tester;
    
    {
        //testing basic correctness, creating a queue, inserting students, reading them out
        cout << "\nTest case: Testing basic correctness:\n";
        if (tester.basicCorrectNess())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing normal cases of copy constructor and assignment operator
        cout << "\nTest case: Testing normal cases of making copies:\n";
        if (tester.deepCopy())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing edge cases of copy constructor and assignment operator
        cout << "\nTest case: Testing edge cases of making copies:\n";
        if (tester.deepCopyEdge())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing the functionality with changing the priority function
        cout << "\nTest case: Testing the functionality with changing the priority function:\n";
        cout << "**Read the contents to make sure the rebuilt heap maintains the priority property**\n";
        //Read the contents to make sure the heap is rebuilt and the property of min-skew-heap is kept
        tester.priorityChange();
    }
    {
        //Measuring the efficiency of insertion and extraction with large queues
        cout << "\nTest case: Measuring the efficiency of insertion and extraction with large queues:" << endl;
        cout << "**It should be O(log n), the times should increase by about a factor of 2**\n" << endl;
        int M = 4;//number of trials
        int N = 10000;//original input size
        tester.efficiency(M,N);
    }
    {
        //testing merging two queues with different priority functions
        cout << "\nTest case: Testing merging two queues with different priority functions:\n";
        if (tester.mergeDifferntPriority())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    {
        //testing dequeing from an empty queue
        cout << "\nTest case: Testing dequeing from an empty queue:\n";
        if (tester.dequeEmpty())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }

    return 0;
}

//Testing dequeing from an empty queue,should throw a domain_error exception
bool Tester::dequeEmpty(){
    RQueue queue1(priorityFn1);
    
    try {
        queue1.getNextStudent();
        
    //The error we expect
    }catch (domain_error &e) {
        return true;
        
    //Any other exception, this case is not acceptable
    }catch(...){
        return false;
    }
    
    //No exception thrown, this case is not acceptable
    return false;
}

//Testing merging two queues with different priority functions, should throw a domain_error exception
bool Tester::mergeDifferntPriority(){
    RQueue queue1(priorityFn1);
    RQueue queue2(priorityFn2);
    
    //Merging the two queues with different priority functions
    try {
        queue1.mergeWithQueue(queue2);
        
    //The error we expect
    }catch (domain_error &e) {
        return true;
        
    //Any other exception, this case is not acceptable
    }catch(...){
        return false;
    }
    
    //No exception thrown, this case is not acceptable
    return false;
}

//Testing the efficiency with large queues (expect O(log n))
void Tester::efficiency(int numTrials, int N){
    //Measuring the efficiency of inserting with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k = 0; k < numTrials; k++){
        RQueue queue1(priorityFn1);
        Student student1("Sam",1,1,1,1);
        
        //Insertion
        start = clock();
        //the algorithm to be analyzed for efficiency
        for(int i = 0; i < N; i++){
            queue1.insertStudent(student1);
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Inserting " << N << " students took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        
        //Extraction
        start = clock();
        //the algorithm to be analyzed for efficiency
        for(int i = 0; i < N; i++){
            queue1.getNextStudent();
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Extracting " << N << " students took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!\n" << endl;
        
        N = N * a;//increase the input size by the scaling factor
    }
}

//Testing changing the priority of a heap and also the rebuilt heap
void Tester::priorityChange(){
    //Setting up the queue with 4 students
    RQueue queue1(priorityFn1);
    Student student1("Sam",1,1,1,1);
    Student student2("Liz",5,1,2,0);
    Student student3("Nick",2,0,0,0);
    Student student4("Eva",4,3,2,1);
    queue1.insertStudent(student3);
    queue1.insertStudent(student1);
    queue1.insertStudent(student2);
    queue1.insertStudent(student4);
    
    //Printing the contents before the change
    std::cout << std::endl << "Contents of queue1 before changing the priority function to priorityFn2:\n";
    queue1.printStudentQueue();
    std::cout << "Dump queue1 before changing the priority function to priorityFn2:\n";
    queue1.dump();
    
    //Changing the priority
    queue1.setPriorityFn(priorityFn2);
    //Printing the contents after the change
    std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
    queue1.printStudentQueue();
    std::cout << "Dump queue1 after changing the priority function to priorityFn2:\n";
    queue1.dump();
}

//Testing the edge cases of the copy constructor and the assignment operator
bool Tester::deepCopyEdge(){
    RQueue queue(priorityFn1);
    Student student1("Sam",1,1,1,1);
    
    //Copy constructor
    RQueue copyCon1(queue);
    //Assignment operator
    RQueue copyAsn1(priorityFn2);
    copyAsn1 = queue;
    
    queue.insertStudent(student1);
    //Copy constructor
    RQueue copyCon2(queue);
    //Assignment operator
    RQueue copyAsn2(priorityFn2);
    copyAsn2 = queue;
    
    //Edges cases with 0 students
    if(copyCon1._size != 0 || copyAsn1._size != 0){
        return false;
    }
    
    //Checks self assignment
    try {
        queue = queue;
        if(queue._size != 1)
            return false;
    } catch (...) {
        return false;
    }
    
    queue.getNextStudent();
    //Edge cases with 1 student
    if(copyCon2._size != 1 || copyAsn2._size != 1){
        return false;
    }
    
    return true;
}

//Testing that the copy constructor and the assignment operator creates a deep copy
bool Tester::deepCopy(){
    RQueue queue(priorityFn1);
    Student student1("Sam",1,1,1,1);
    Student student2("Liz",5,1,2,0);
    Student student3("Nick",2,0,0,0);
    Student student4("Eva",4,3,2,1);
    queue.insertStudent(student3);
    queue.insertStudent(student1);
    queue.insertStudent(student2);
    queue.insertStudent(student4);
    
    //Copy constructor
    RQueue copyCon(queue);
    //Assignment operator
    RQueue copyAsn(priorityFn2);
    copyAsn = queue;
    
    //Deleting the students from the original heap
    while (queue.numStudents() > 0) {
        queue.getNextStudent();
    }
    
    //Check if the copies are deep and they still have 4 students
    if(copyCon._size != 4 || copyAsn._size != 4){
        return false;
    }
    
    return true;
}

//Testing the basic correctness
//It should work without errors, and the heap priority should be checked by what's printed out
bool Tester::basicCorrectNess(){
    RQueue queue1(priorityFn1);
    Student student1("Sam",1,1,1,1);
    Student student2("Liz",5,1,2,0);
    Student student3("Nick",2,0,0,0);
    Student student4("Eva",4,3,2,1);
    Student student5("Wally",0,3,0,1);
    Student student6("Marcus",6,1,2,0);
    Student student7("Harry",3,0,1,1);
    Student student8("Luke",4,2,1,1);
    Student student9("Ole",2,3,2,0);
    Student student10("Bruno",5,1,0,1);

    try {
        //Inserting 10 students
        queue1.insertStudent(student3);
        queue1.insertStudent(student1);
        queue1.insertStudent(student2);
        queue1.insertStudent(student4);
        queue1.insertStudent(student5);
        queue1.insertStudent(student6);
        queue1.insertStudent(student7);
        queue1.insertStudent(student8);
        queue1.insertStudent(student9);
        queue1.insertStudent(student10);
        std::cout << "\nDump after inserting 10 students (check for heap property):" << std::endl;
        queue1.dump();

    //Shouldn't have any erros
    } catch (...) {
        return false;
    }
    
    return true;
}

int priorityFn1(const Student&student) {
    //priority value is determined based on some criteria
    //priority value falls in the range [0-6]
    //the smaller value means the higher priority
    int priority = student.getYear() + student.getMajor() + student.getGroup();
    return priority;
}

int priorityFn2(const Student&student) {
    //priority is determined by an officer in registrar office
    //priority value falls in the range [0-6]
    //the smaller value means the higher priority
    return student.getPriority();
}
