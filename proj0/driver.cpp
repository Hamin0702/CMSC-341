#include "puzzle.h"

class Tester{//this is your tester class, you add your test functions in this class
    public:
    bool testCopyConstructor(const Puzzle& puzzle);
    void measureInsertionTime(int numTrials, int N);
};

int main() {
    Tester tester;
    {
        //test deep copy, object with many members
        Puzzle p1(1000);
        cout << "Test case, Copy Constructor: same members, same size, different pointers (deep copy):" << endl;
        if (tester.testCopyConstructor(p1)) cout << "Copy constructor passed!" << endl;
        else cout << "Copy constructor failed!" << endl;
    }
    {
        //test the edge case, object with 1 member
        cout << "\nTest case, Copy Constructor: 1 member:" << endl;
        Puzzle p1(1);
        if (tester.testCopyConstructor(p1)) cout << "Copy constructor passed!" << endl;
        else cout << "Copy constructor failed!" << endl;
    }
    {
        //test the edge case, 0 member, i.e. empty object
        cout << "\nTest case, Copy Constructor: zero members:" << endl;
        Puzzle p1(0);
        if (tester.testCopyConstructor(p1)) cout << "Copy constructor passed!" << endl;
        else cout << "Copy constructor failed!" << endl;
    }
    {
        //test the user error case, creating object with table zise less than 0 
        cout << "\nTest case, Copy Constructor: table size less than 0:" << endl;
        Puzzle p1(-10);
        if (tester.testCopyConstructor(p1)) cout << "Copy constructor passed!" << endl;
        else cout << "Copy constructor failed!" << endl;
    }
    {
        //Measuring the efficiency of insertion functionality
        cout << "\nMeasuring the efficiency of insertion functionality:" << endl;
        int M = 5;//number of trials
        int N = 1000;//original input size
        tester.measureInsertionTime(M, N);
    }
    {
        //an example of dump
        cout << "\nHere is an example of a table:" << endl;
        Puzzle p1(10);
        p1.dump();
        cout << endl;
    }
    return 0;
}

bool Tester::testCopyConstructor(const Puzzle& puzzle){
    Puzzle copy(puzzle);
    //the case of empty object
    if (puzzle.m_size == 0 && copy.m_size == 0) return true;
    //the case that sizes are the same and the table pointers are not the same
    else if (puzzle.m_size == copy.m_size && puzzle.m_table != copy.m_table){
        for (int i=0;i<puzzle.m_size;i++){
            for (int j=0;j<puzzle.m_size;j++)
            {
                if ((puzzle.m_table[i][j] != copy.m_table[i][j]) || //check the value
                    (puzzle.m_table[i] == copy.m_table[i])){        //check the ith pointer
                    return false;
                }
            }
        }
        return true;
    }
    //everthing else
    else return false;
}
void Tester::measureInsertionTime(int numTrials, int N)
{
    //Measuring the efficiency of insertion algorithm with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k=0;k<numTrials-1;k++)
    {
        start = clock();
        Puzzle p1(N);//the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Inserting " << N * N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        N = N * a;//increase the input size by the scaling factor
    }
}
