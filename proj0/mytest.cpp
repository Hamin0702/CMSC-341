#include "puzzle.h"

//Tester class with the test functions
class Tester{
    public:
    bool testAssignmentOperator(const Puzzle& puzzle);
    void measureCopyTime(int numTrials, int N);
    bool testSelfAssignment(int numRows);
};

int main() {
    Tester tester;
    {
        //test deep copy, object with many members
        Puzzle p1(1000);
        cout << "Test case, Assignment Operator: same members, same size, different pointers (deep copy):" << endl;
        if (tester.testAssignmentOperator(p1)) cout << "Assignment Operator passed!" << endl;
        else cout << "Assignment Operator failed!" << endl;
    }
    {
        //test the edge case, object with 1 member
        cout << "\nTest case, Assignment Operator: 1 member (edge case):" << endl;
        Puzzle p1(1);
        if (tester.testAssignmentOperator(p1)) cout << "Assignment Operator passed!" << endl;
        else cout << "Assignment Operator failed!" << endl;
    }
    {
        //test the edge case, 0 member, i.e. empty object
        cout << "\nTest case, Assignment Operator: zero members (edge case):" << endl;
        Puzzle p1(0);
        if (tester.testAssignmentOperator(p1)) cout << "Assignment Operator passed!" << endl;
        else cout << "Assignment Operator failed!" << endl;
    }
    {
        //test the user error case, creating object with table size less than 0
        cout << "\nTest case, Assignment Operator: table size less than 0 (user error case):" << endl;
        Puzzle p1(-10);
        if (tester.testAssignmentOperator(p1)) cout << "Assignment Operator passed!" << endl;
        else cout << "Assignment Operator failed!" << endl;
    }
    {
        //Measuring the efficiency of insertion functionality
        cout << "\nMeasuring the efficiency of copy constructor:" << endl;
        cout << "*Note*: Running time should increase by a factor of 4 since the puzzle size increase by 4 (Linear)" << endl;
        int M = 5;//number of trials
        int N = 1000;//original input size
        tester.measureCopyTime(M, N);
    }
    {
        //Testing self assignment with 100 rows
        cout << "\nTest case, self assignment with 100 rows:" << endl;
        if (tester.testSelfAssignment(100)) cout << "Self assignment test passed!" << endl;
        else cout << "Self assignment test failed!" << endl;
    }
    {
        //Testing self assignment with 0 rows
        cout << "\nTest case, self assignment with empty puzzle:" << endl;
        if (tester.testSelfAssignment(0)) cout << "Self assignment test passed!" << endl;
        else cout << "Self assignment test failed!" << endl;
    }
     
    return 0;
}

bool Tester::testAssignmentOperator(const Puzzle& puzzle){
    Puzzle copy = puzzle;
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

void Tester::measureCopyTime(int numTrials, int N){
    //Measuring the efficiency of Copy Constructor with system clock ticks
    //Clock ticks are units of time of a constant but system-specific length, as those returned by function clock().
    //Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k=0;k<numTrials-1;k++)
    {
        Puzzle p1(N);
        start = clock();
        Puzzle p2(p1);//the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Copying " << N * N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        N = N * a;//increase the input size by the scaling factor
    }
}

//Test should be working if it doesn't crash but also checking that it has the same size and pointer
bool Tester::testSelfAssignment(int numRows){
    int tempSize;
    char** tempPoint;
    Puzzle puzzle(numRows);
    tempSize = puzzle.m_size;
    tempPoint = puzzle.m_table;
    puzzle = puzzle;
    
    //Checks if the size and the pointer are the same
    if(puzzle.m_size == 0 && tempSize ==0){
        return true;
    }else if (puzzle.m_size == tempSize && puzzle.m_table == tempPoint){
        return true;
    } else{
        return false;
    }
    
}
