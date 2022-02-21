// CMSC 341 - Spring 2021 - Project 4
// hash.cpp
// The impleme ntation of the class HashTable
#include "hash.h"
#include "math.h"

//Alternative constructor
//Memver variables are initialized
HashTable::HashTable(unsigned size, hash_fn hash){
    
    //Initializing member variables
    if(size <= MINPRIME){
        m_capacity = MINPRIME;
    }else if(size >= MAXPRIME){
        m_capacity = MAXPRIME;
    }else{
        m_capacity = findNextPrime(size);
    }
    m_size = 0;
    m_numDeleted = 0;
    m_hash = hash;
    m_currentProb = LINEAR;
    
    //Initializing the array for the hash table and filling them with the EMPTY person
    m_table = new Person[m_capacity];
    for(int i = 0; i < m_capacity; i++){
        m_table[i] = EMPTY;
    }
}

//Destructor
//Deallocates memory
HashTable::~HashTable(){
    delete[] m_table;
    m_table = nullptr;
}

//Insertion function
bool HashTable::insert(Person person){
    
    //Checks for duplicate person
    for(int i = 0; i < m_capacity; i++){
        if(!(m_table[i] == EMPTY) && m_table[i] == person)
            return false;
    }
    
    int i = 0;
    int index = 0;
    
    //If linear probing is being used to find the empty index to insert
    if(m_currentProb == LINEAR){
        
        //Iteration cap of m_capacity
        for(int cap = 0; cap < m_capacity; cap++){
            index = (m_hash(person.key()) + i) % m_capacity;
            i++;
            
            //Insertion
            if(m_table[index] == EMPTY){
                m_table[index] = person;
                m_size++;
                checkRehash();
                return true;
            }
        }
    }

    //If quadratic probing is being used to find the empty index to insert
    if(m_currentProb == QUADRATIC){
        
        //Iteration cap of m_capacity
        for(int cap = 0; cap < m_capacity; cap++){
            index = ((m_hash(person.key()) % m_capacity) + (i*i)) % m_capacity;
            i++;
            
            //Insertion
            if(m_table[index] == EMPTY){
                m_table[index] = person;
                m_size++;
                checkRehash();
                return true;
            }
        }
    }

    //If nothing was inserted after looping over the iteration cap
    return false;
}

//Removal function
bool HashTable::remove(Person person){
    
    int i = 0;
    int index = 0;
    
    //If linear probing is being used
    if(m_currentProb == LINEAR){
        
        //Find the person to delete
        
        //Iteration cap of m_capacity
        for(int cap = 0; cap < m_capacity; cap++){
            index = (m_hash(person.key()) + i) % m_capacity;
            i++;
           
            //Removal
            if(m_table[index] == person){
                m_table[index] = DELETED;
                m_numDeleted++;
                checkRehash();
                return true;
            }
        }
    }

    //If quadratic probing is being used
    if(m_currentProb == QUADRATIC){
        
        //Find the person to delete
        
        //Iteration cap of m_capacity
        for(int cap = 0; cap < m_capacity; cap++){
            index = ((m_hash(person.key()) % m_capacity) + (i*i)) % m_capacity;
            i++;
            
            //Removal
            if(m_table[index] == person){
                m_table[index] = DELETED;
                m_numDeleted++;
                checkRehash();
                return true;
            }
        }
    }
    
    //If nothing was deleted after looping over the iteration cap
    return false;
}

//Changes the probing policy and rehashes the data into a new table
void HashTable::setProbingPolicy(probing prob){
    
    //Changes the probing policy
    m_currentProb = prob;
    
    //Doesn't do rehash if the table is empty
    if(m_size == 0)
        return;
    
    //Rehashing process
    
    //If there are only deleted persons in the table
    if(m_size == m_numDeleted){
        
        //Flushing the table of deleted persons and making a fresh table
        delete[] m_table;
        m_table = nullptr;
        m_table = new Person[MINPRIME];
        
        //Reinitializing the member variables
        m_capacity = MINPRIME;
        m_size = 0;
        m_numDeleted = 0;
        
    //If rehash needs to happen
    }else{
        
        int numData = m_size - m_numDeleted;
        
        //Putting the non-deleted persons in a temporary array
        Person* temp = new Person[numData];
        int tempIndex = 0;
        for(int i = 0; i < m_capacity; i++){
            if(!(m_table[i] == EMPTY) && !(m_table[i] == DELETED)){
                temp[tempIndex] = m_table[i];
                tempIndex++;
            }
        }
        
        //Making the new table
        m_capacity = findNextPrime(4*numData);
        m_size = 0;
        m_numDeleted = 0;
        delete[] m_table;
        m_table = nullptr;
        m_table = new Person[m_capacity];
        
        //Inserting the people in the new table
        for(int i = 0; i < numData; i++){
            insert(temp[i]);
        }
        
        //Clean up the temp array
        delete[] temp;
        temp = nullptr;
    }

}

//Returns the ratio of occupied buckets to the table size
float HashTable::lambda() const {
    return float(m_size) / float(m_capacity);
}

//Returns the ratio of the deleted buckets to the total number of occupied buckets
float HashTable::deletedRatio() const {
    return float(m_numDeleted) / float(m_size);
}

//Dumps the contents of the hash table in index order
void HashTable::dump() const {
  for (int i = 0; i < m_capacity; i++) {
    cout << "[" << i << "] : " << m_table[i] << endl;
  }
}

//Returns true if number is prime
bool HashTable::isPrime(int number){
    // If number is prime this function returns true
    // otherwise it returns false
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

//Returns the smallest prime bumber greater than current
int HashTable::findNextPrime(int current){
    //we won't go beyond MAXPRIME
    //the smallest prime would be next after MINPRIME
    if (current < MINPRIME) current = MINPRIME;
    for (int i=current; i<MAXPRIME; i++) {
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0)
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

//Helper function that begins the rehash process if it meets a certain criteria
void HashTable::checkRehash(){
    
    //Finding the other probing method
    probing newProbing = LINEAR;
    if(m_currentProb == LINEAR){
        newProbing = QUADRATIC;
    }
    if(m_currentProb == QUADRATIC){
        newProbing = LINEAR;
    }
    
    //Rehashing the table and switching the probing policy if it meets a certain criteria
    if(lambda() > .5){
        setProbingPolicy(newProbing);
    }
    if(deletedRatio() > .25){
        setProbingPolicy(newProbing);
    }
}
