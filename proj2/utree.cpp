/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {

    //Deallocate memory
    clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes -
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree.
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
    
    //If root node is null
    if(_root == nullptr){
        _root = new UNode();
        _root->_dtree->insert(newAcct);
        return true;
        
    //If the node doesn't exist
    }else if(retrieve(newAcct.getUsername()) == nullptr){
        _root = insertNode(_root, newAcct);
        return true;
        
    //If the node with the username exists
    }else{
        //If the account with the same discriminator exists
        if(retrieveUser(newAcct.getUsername(), newAcct.getDiscriminator()) != nullptr){
            return false;
        //If the account doesn't already exist (or the one with the same discriminator is vacant)
        }else{
            retrieve(newAcct.getUsername())->_dtree->insert(newAcct);
            return true;
        }
    }
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode*& removed) {
    
    //If the user we're trying to remove doesn't exist
    if(retrieve(username) == nullptr){
        return false;
    }
    if(retrieveUser(username, disc) == nullptr){
        return false;
    }

    //Remove the user from the UNode
    retrieve(username)->_dtree->remove(disc, removed);

    //If there are no more nodes in the DTree
    if(retrieve(username)->_dtree->getNumUsers() == 0){
        //Remove the UNode
        _root = deletion(_root, username);
    }

    //Return true if user was removed
    if(removed != nullptr){
        return true;
    }else{
        return false;
    }
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
    return search(_root, username);
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
    return retrieve(username)->_dtree->retrieve(disc);
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
    return retrieve(username)->getDTree()->getNumUsers();
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
    deleteTree(_root);
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
    printInOrder(_root);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
    
    //If the node is a leaf node
    if(node->_left == nullptr && node->_right == nullptr){
        node->_height = 0;
        
    //If the node has one child
    }else if(node->_left != nullptr && node->_right == nullptr){
        node->_height = node->_left->_height + 1;
    }else if(node->_left == nullptr && node->_right != nullptr){
        node->_height = node->_right->_height + 1;
        
    //If the node has both children
    }else if(node->_left != nullptr && node->_right != nullptr){
        //If the left child's height is bigger
        if(node->_left->_height > node->_right->_height){
            node->_height = node->_left->_height + 1;
        //If the right child's height is bigger
        }else if(node->_left->_height < node->_right->_height){
            node->_height = node->_right->_height + 1;
        //If the height of both children are the same
        }else{
            node->_height = node->_left->_height + 1;
        }
    }
}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
    int heightLeft = -1;
    int heightRight = -1;
    
    if(node != nullptr){
        //Getting the height of the children
        if(node->_left != nullptr){
            heightLeft = node->_left->_height;
        }
        if(node->_right != nullptr){
            heightRight = node->_right->_height;
        }
    }
    return heightLeft - heightRight;
}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
UNode* UTree::rebalance(UNode* node) {
    
    //If the tree is left heavy
    if(leftHeavy(node)){
        
        //One simple right rotation
        if(leftHeavy(node->_left)){
            return rightRotation(node);
        
        //Left-Right rotation
        }else if(rightHeavy(node->_left)){
            node->_left = leftRotation(node->_left);
            return rightRotation(node);
        }
    
    //If the tree is right heavy
    }else if(rightHeavy(node)){
        
        //One simple left rotation
        if(rightHeavy(node->_right)){
            return leftRotation(node);
            
        //Right-Left rotation
        }else if(leftHeavy(node->_right)){
            node->_right = rightRotation(node->_right);
            return leftRotation(node);
        }
    }
    
    return node;
}

//Helper function used by the retrieve function
//Goes through the tree and returns the node or nullptr if the node doesn't exist
UNode* UTree::search(UNode* node, string username){
    
    //Returns null if the node is null and ends the recursive call
    if(node == nullptr){
        return nullptr;
        
    //When the node is the one we're searching for
    }else if(node->getUsername() == username){
        return node;
        
    //Recursive search through the children
    }else{
        if(username < node->getUsername()){
            return search(node->_left, username);
        }else if(username > node->getUsername()){
            return search(node->_right, username);
        }
    }
    
    return nullptr;
}

//Helper function for the PrintUsers function
//Uses inorder traversal and the overloaded insertion operator
void UTree::printInOrder(UNode* node) const{
    
    //Does nothing if the node is null and ends the recursive call
    if(node == nullptr){
        return;
    }
    
    //Visit the left node, print all the accounts of node's DTree, visit right node
    printInOrder(node->_left);
    node->getDTree()->printAccounts();
    printInOrder(node->_right);
}

//Helper function for the clear function
//Goes through the tree and deallocate the memory
void UTree::deleteTree(UNode* node){
    
    //Does nothing if the node is null and ends the recursive call
    if(node != nullptr) {
        //Deletes the children first
        deleteTree(node->_left);
        deleteTree(node->_right);
            
        //Finally deletes the root node
        delete node;
        node = nullptr;
    }
}

UNode* UTree::insertNode(UNode* node, Account acc){
    
    //If the root is null, create a new node and return it
    if(node == nullptr){
        UNode* newNode = new UNode();
        newNode->_dtree->insert(acc);
        return newNode;
    }
    
    //The other cases where the new account would be added
    if(acc.getUsername() < node->getUsername()){
        node->_left = insertNode(node->_left, acc);
    }else if(acc.getUsername() > node->getUsername()){
        node->_right = insertNode(node->_right, acc);
    }
    
    //update height
    updateHeight(node);
    //check imbalance
    if(abs(checkImbalance(node)) > 1){
        //rebalance
        node = rebalance(node);
    }
    
    return node;
}

//Helper function used by the rebalance function
//Implements a right rotation (when the node is left-heavy)
UNode* UTree::rightRotation(UNode* node){
    //The rotating nodes
    UNode* Y = node;
    UNode* X = Y->_left;
    //The rotating subtrees
    UNode* a = X->_left;
    UNode* b = X->_right;
    UNode* c = Y->_right;
    
    //Updating and rotating the tree
    Y->_left = b;
    Y->_right = c;
    updateHeight(Y);
    X->_left = a;
    X->_right = Y;
    updateHeight(X);
    
    return X;
}

//Helper function used by the rebalance function
//Implements a left rotation (when the node is right-heavy)
UNode* UTree::leftRotation(UNode* node){
    //The rotating nodes
    UNode* X = node;
    UNode* Y = X->_right;
    //The rotating subtrees
    UNode* a = X->_left;
    UNode* b = Y->_left;
    UNode* c = Y->_right;
    
    //Updating and rotating the tree
    X->_left = a;
    X->_right = b;
    updateHeight(X);
    Y->_left = X;
    Y->_right = c;
    updateHeight(Y);
    
    return Y;
}

//Helper function used by the rebalance function
//Checks if the node is left-heavy
bool UTree::leftHeavy(UNode* node){
    int heightLeft = -1;
    int heightRight = -1;
    
    //Getting the height of the children
    if(node->_left != nullptr){
        heightLeft = node->_left->_height;
    }
    if(node->_right != nullptr){
        heightRight = node->_right->_height;
    }
    
    //If node is left-heavy return true
    if(heightLeft > heightRight){
        return true;
    }else{
        return false;
    }
}

//Helper function used by the rebalance function
//Checks if the node is right-heavy
bool UTree::rightHeavy(UNode* node){
    int heightLeft = -1;
    int heightRight = -1;
    
    //Getting the height of the children
    if(node->_left != nullptr){
        heightLeft = node->_left->_height;
    }
    if(node->_right != nullptr){
        heightRight = node->_right->_height;
    }
    
    //If node is right-heavy return true
    if(heightLeft < heightRight){
        return true;
    }else{
        return false;
    }
}

//Helper function that finds the minimum node
UNode* UTree::findMin(UNode* node){
    if(node == nullptr){
        return nullptr;
    }else if(node->_left == nullptr){
        return node;
    }else{
        return findMin(node->_left);
    }
}

//Helper function for RemoveUser
UNode* UTree::deletion(UNode* node, string username){
    UNode* temp;
    
    //If the node that needs to be deleted isn't found
    if(node == nullptr){
        return nullptr;
        
    //Searching for node
    }else if(username < node->getUsername()){
        node->_left = deletion(node->_left, username);
    }else if(username > node->getUsername()){
        node->_right = deletion(node->_right, username);
        
    //Node is found
    //With two children
    }else if(node->_left != nullptr && node->_right != nullptr){
        temp = findMin(node->_right);
        *node->_dtree = *temp->_dtree;
        node->_right = deletion(node->_right, node->getUsername());
        
    //With one or zero child
    }else{
        temp = node;
        if(node->_left == nullptr){
            node = node->_right;
        }else if(node->_right == nullptr){
            node = node->_left;
        }
        delete temp;
        temp = nullptr;
    }
    
    if(node == nullptr){
        return node;
    }
    
    updateHeight(node);
    if(abs(checkImbalance(node)) > 1){
        node = rebalance(node);
    }
    
    return node;
}
