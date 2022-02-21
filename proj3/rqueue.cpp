// CMSC 341 - Spring 2021 - Project 3
// RQueue:

#include "rqueue.h"
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

//Constructor
RQueue::RQueue(prifn_t priFn){
    
    //Initializing member variables
    priority = priFn;
    _size = 0;
    _heap = nullptr;
}

//Destructor
RQueue::~RQueue(){
    
    if(_size > 0){
        clear();
    }
}

//Copy constructor
RQueue::RQueue(const RQueue& rhs){
    
    //Copies the heap
    _heap = copyHeap(rhs._heap);
    _size = rhs._size;
    priority = rhs.priority;
}

//Assignment operator
RQueue& RQueue::operator=(const RQueue& rhs){
    
    //Checks self assignment
    if(this == &rhs){
        return *this;
    }else{
      
    //Deallocates memory
    clear();
      
    //Copies the heap
    _heap = copyHeap(rhs._heap);
    _size = rhs._size;
    priority = rhs.priority;
    }
    
    return *this;
}

//Insertion method
void RQueue::insertStudent(const Student& input) {
    Node* newNode = new Node(input);
    _heap = merge(_heap, newNode);
    _size = _size + 1;
}

//Extract and remove a node and return it
Student RQueue::getNextStudent() {
  
    //Throw domain_error exception if the queue is empty
    if(_size == 0){
        throw domain_error("Merging heaps with different priority functions");
    }else{
        
        //Extracting the student
        Student student = _heap->_student;
        //Using the helper function to remove the root node and merge the children to make a new heap
        _heap = deletePriority(_heap);
        _size = _size - 1;
        return student;
    }
}

//Merging the host heap and the rhs
void RQueue::mergeWithQueue(RQueue& rhs) {
    
    //Protection against self-merging
    if(this == &rhs){
        return;

    //Throw domain_error exception if the priority functions are different
    }else if(priority != rhs.priority){
        throw domain_error("Merging heaps with different priority functions");
        
    //Else when both heaps exist
    }else{
        Node* h1 = _heap;
        Node* h2 = copyHeap(rhs._heap);
        _heap = merge(h1, h2);
        _size = _size + rhs._size;
        rhs.clear();
    }

}

//Clears the queue and deletes all the nodes in the heap
void RQueue::clear() {
    clearQueue(_heap);
    _size = 0;
}

//Getter function for the number of students in the heap
int RQueue::numStudents() const{
    return _size;
}

//Print the queue using preorder traversal
void RQueue::printStudentQueue() const {
    print(_heap);
}

//Getter function for the current priority function
prifn_t RQueue::getPriorityFn() const {
    return priority;
}

//Sets a new priority function and rebuilds the heap
void RQueue::setPriorityFn(prifn_t priFn) {
    priority = priFn;
    if(_size > 0){
        Student st[_size];
        int size = _size;
        
        //Removes all nodes and places the students in an array
        for(int i = 0; i < size; i++){
            st[i] = getNextStudent();
        }
        
        //Inserts everything back
        for(int i = 0; i < size; i++){
            insertStudent(st[i]);
        }
    }
}

// for debugging
void RQueue::dump() const{
  if (_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(_heap);
    cout << endl;
  }
}

// for debugging
void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->_left);
    cout << priority(pos->_student) << ":" << pos->_student.getName();
    dump(pos->_right);
    cout << ")";
  }
}

// overloaded insertion operator for Student class
ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student: " << student.getName() << ", priority: " << student.getPriority()
       << ", year: " << student.getYear() << ", major: " << student.getMajorStr()
       << ", group: " << student.getGroupStr();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getStudent();
  return sout;
}

//Helper function for the copy constructor and the overloaded assignment operator
//Uses pre-order traversal to make a deep copy of the heap
Node* RQueue::copyHeap(Node* root){
    
    //Returns null if the node is null and ends the recursive call
    if(root == nullptr){
        return nullptr;
    }
    
    //Dynamically creates a new Node
    Node* newRoot = new Node(root->getStudent());
    
    //Copies the children nodes (pre-order)
    newRoot->_left = copyHeap(root->_left);
    newRoot->_right = copyHeap(root->_right);
    
    //Returns the root of the copied heap
    return newRoot;
}

//Helper function for the mergeWithQueue function
//Takes two heaps, merges the two, and returns the root
Node* RQueue::merge(Node* p1, Node* p2){
    Node* temp;
    
    if(p1 == nullptr){
        return p2;
    }else if(p2 == nullptr){
        return p1;
    }else{
        
        //p1 should be at higher priority than p2
        if(priority(p2->_student) < priority(p1->_student)){
            temp = p1;
            p1 = p2;
            p2 = temp;
        }
        
        //Swapping the subtrees of p1
        temp = p1->_left;
        p1->_left = p1->_right;
        p1->_right = temp;
        
        //Recursively merge p2 and the left subtree of p1, which becomes p1's left
        p1->_left = merge(p2, p1->_left);
        
        return p1;
    }
}

//Helper function for the getNextStudent function
//Deletes the original root and returns the new heap by merging the children
Node* RQueue::deletePriority(Node* root){
    if(root == nullptr){
        return nullptr;
    }
    
    //Making a new heap by merging the children of the root
    Node* newRoot = merge(root->_left,root->_right);
    
    //Deleting the original root
    delete root;
    root = nullptr;
    
    return newRoot;
}

//Helper function for the clear function
//Uses post-order traversal to delete the heap
void RQueue::clearQueue(Node* node){
    
    //Does nothing if the node is null and ends the recursive call
    if(node == nullptr) {
        return;
    }
        
    //Deletes the children first
    clearQueue(node->_left);
    clearQueue(node->_right);
        
    //Finally deletes the root node
    delete node;
    node = nullptr;
}

//Helper function for the printStudentQueue
//Print the contents of the queue using pre-order traversal
void RQueue::print(Node* node) const{
    
    //Does nothing if the node is null and ends the recursive call
    if(node == nullptr){
        return;
    }
    
    cout << "[" << priority(node->_student) << "] " << node->_student << endl;
    print(node->_left);
    print(node->_right);
}
