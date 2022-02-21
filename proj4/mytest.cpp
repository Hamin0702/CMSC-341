// CMSC 341 - Spring 2021 - Project 4
// mytest.cpp
// Simple driver program for use with HashTable class
#include "hash.h"
#include <random>

// Using a fixed seed value generates always the same sequence
// of pseudorandom numbers, e.g. reproducing scientific experiments
// here it helps us with testing since the same sequence repeats
std::mt19937 generator(10);// 10 is the fixed seed value

// We distribute the pseudorandom numbers uniformaly within
// the range [1000,99999]
std::uniform_int_distribution<> distribution(1000, 99999);

// The hash function used by HashTable class
unsigned int hashCode(const string str);

// the function to generate random serial numbers
unsigned int vaccineSerial() {return distribution(generator);}

class Tester{ // Tester class to implement test functions
    public:
    Tester(){}
    ~Tester(){}
    void testBasicInsertion();
    void testFunctionality();
    void testProbeChange();
    bool testEdgeCase();
    private:
};

int main(){
    Tester tester;
    
    {
        //Testing simple insertion operation
        cout << "****Testing basic insertion****\n" << endl;
        tester.testBasicInsertion();
    }
    {
        //Testing other functionality
        cout << "****Testing functionality****\n" << endl;
        tester.testFunctionality();
    }
    {
        //Testing probe change
        cout << "****Testing chaning the probing method****\n" << endl;
        tester.testProbeChange();
    }
    {
        //Testing edge cases
        cout << "****Testing edge cases****\n" << endl;
        if (tester.testEdgeCase())
            cout << "\tTest passed!\n";
        else
            cout << "\tTest failed!\n";
    }
    
    return 0;
}

//Testing edge cases
bool Tester::testEdgeCase(){
    HashTable tableA(MINPRIME-1,hashCode);
    HashTable tableB(MAXPRIME+1,hashCode);
    
    //Testing edge cases with table size
    if(tableA.m_capacity != MINPRIME){
        return false;
    }
    if(tableB.m_capacity != MAXPRIME){
        return false;
    }
    
    //Testing edge cases with insertion/deletion
    if(tableA.remove(Person("John", 1000))){
        return false;
    }
    tableB.insert(Person("John", 1000));
    if(tableB.insert(Person("John", 1000))){
        return false;
    }
    
    return true;
}

//Testing if changing the probing method triggers a rehash
void Tester::testProbeChange(){
    HashTable aTable(101,hashCode);
    
    //Inserting 50 data points
    for (int i=0;i<50;i++){
        int temp = vaccineSerial();
        aTable.insert(Person("John",temp));
    }
    cout << "Message: Hash table before rehashing" << endl;
    aTable.dump();
    cout << endl;
    
    aTable.setProbingPolicy(QUADRATIC);
    cout << "Message: Hash table after changing probing method should trigger a rehash" << endl;
    aTable.dump();
    cout << endl;
}

//Testing functionality of the hash table
//From the given driver class
void Tester::testFunctionality(){
    // We use this temp array to store some data points for later use
    int tempSerials[50] = {0};
    int secondIndex = 0;
    // create a hash table with the size 101, and the hash function hashCode
    HashTable aTable(101,hashCode);
    int temp = 0;
    // we insert 50 Person objects with the same key, person's name is the key
    for (int i=0;i<50;i++){
       temp = vaccineSerial();// generates a random number
       if (i%3 == 0){//this saves 17 numbers from the generated numbers for later use
          tempSerials[secondIndex] = temp;
          cout << temp << " was saved for later use." << endl;
          secondIndex++;
       }
       // print the random numbers which are inserted
       cout << "Insertion # " << i << " => " << temp << endl;
       aTable.insert(Person("John", temp));
    }
    cout << "Message: dump after 50 insertions in a table with 101 buckets:" << endl;
    aTable.dump();
    
    // We insert a data point to change the load factor, this should initiate a rehash
    temp = vaccineSerial();
    cout << "Insertion # 51" << " => " << temp << endl;
    aTable.insert(Person("John", temp));
    cout << "Message: dump after 51st insertion, the rehash should have happended to a table with 211 buckets:" << endl;
    aTable.dump();
    
    // Now we remove some data points which were saved in tempSerials array
    // Deleting 13 data points changes the deleteRatio, it should initiate a rehash
    for (int i = 0;i<14;i++)
       aTable.remove(Person("John", tempSerials[i]));
    cout << "Message: dump after 14 removes, a rehash should have happened to a table with 157 buckets," << endl;
    cout << "Message: after 13 removals the rehash happens, then the 14th removal will be tagged as DELETED in the new table." << endl;
    aTable.dump();
    cout << endl;
}

//Testing basic insertion
void Tester::testBasicInsertion(){
    HashTable aTable(101,hashCode);
    
    cout << "Message: Initial data size" <<endl;
    cout << "Size of table: " << aTable.m_capacity << endl;
    cout << "Size of data: " << aTable.m_size << endl;
    
    //Adding non-colliding data points
    aTable.insert(Person("John", vaccineSerial()));
    aTable.insert(Person("Tim", vaccineSerial()));
    aTable.insert(Person("Jake", vaccineSerial()));
    aTable.insert(Person("Ian", vaccineSerial()));
    aTable.insert(Person("Marcus", vaccineSerial()));
    aTable.insert(Person("Bruno", vaccineSerial()));
    
    cout << "Message: after inserting 6 non-colliding data points" <<endl;
    aTable.dump();
    
    cout << "Message: Data size after insertion" <<endl;
    cout << "Size of table: " << aTable.m_capacity << endl;
    cout << "Size of data: " << aTable.m_size << endl;
    
    cout << endl;
}

// The implementation of the hash function
// you can copy this function to your mytest.cpp file
unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for ( int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

