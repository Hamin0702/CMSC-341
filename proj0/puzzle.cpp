#include "puzzle.h"

//Alternative constructor
Puzzle::Puzzle(int size){
    
    //no memory is allocated if size is 0
    if (size < 0){
        m_size = 0;
        
    //else it sets m_size and allocates memory and initializes values
    } else {
        m_size = size;
        makeMem();
        fill();
    }
    
}

//Destructor
Puzzle::~Puzzle(){
    clear();
}

//Deallocates memory for m_table
void Puzzle::clear(){
    
    //checks if there is memory to be deallocated
    if (m_size > 0){
        
        //memory is deallocated
        for(int i = 0; i < m_size; i++){
            delete [] m_table[i];
        }
        delete [] m_table;
        
    }
    
    m_size = 0;
}

//Allocates memory for m_table
void Puzzle::makeMem(){
    
    //checks if there is memory to be allocated
    if (m_size > 0){
        
        //memory is allocated based on m_size
        m_table = new char*[m_size];
        for(int i = 0; i < m_size; i++){
            m_table[i] = new char[m_size];
        }
    }
    
}

//Copy constructor
Puzzle::Puzzle(const Puzzle& rhs){
    
    //copies size
    m_size = rhs.m_size;
    
    //allocates memory and copies in the values
    if(m_size > 0){
        makeMem();
        
        int i = 0;
        int j = 0;
        while(i < m_size){
            while(j < m_size){
                m_table[i][j] = rhs.m_table[i][j];
                j++;
            }
            j = 0;
            i++;
        }
    }
    
}

//Assignment operator
const Puzzle& Puzzle::operator=(const Puzzle& rhs){
    
    //checks self assignment
    if(this == &rhs){
        return *this;
    }else{
        
        //first cleans up the existing m_table
        clear();
        m_size = rhs.m_size;
        
        //allocates memory and copies in the values
        if(m_size > 0){
            makeMem();
            
            int i = 0;
            int j = 0;
            while(i < m_size){
                while(j < m_size){
                    m_table[i][j] = rhs.m_table[i][j];
                    j++;
                }
                j = 0;
                i++;
            }
        }
        return *this;
    }
}

//Printing function
void Puzzle::dump(){
    if (m_size > 0){
        int i=0;
        int j=0;
        while(i<m_size){
            while(j<m_size){
                cout << m_table[i][j] << " ";
                j++;
            }
            cout << endl;
            j=0;
            i++;
        }
        cout << endl;
    }
}

//Initializes values of m_table
void Puzzle::fill(){
    srand(time(NULL));//create the seed value using the current time
    int i=0;
    int j=0;
    while(i<m_size){
        while(j<m_size){
            m_table[i][j] = ALPHA[rand() % MAX];//find a random char from the chars list
            j++;
        }
        j=0;
        i++;
    }
}
