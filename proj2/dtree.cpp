/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */

#include "dtree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
DTree::~DTree() {
    
    //Helper function to delete the DTree and deallocate memory
    clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree& DTree::operator=(const DTree& rhs) {
    
    //Checks self assignment
    if(this == &rhs){
        return *this;
    }else{
        
        //Deallocates memory
        clear();
        
        //Copies the tree
        _root = copyTree(rhs._root);
    }
    
    return *this;
}

/**
 * Dynamically allocates a new DNode in the tree.
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct) {
    
    //If tree is empty
    if(_root == nullptr){
        _root = new DNode(newAcct);
        return true;
    }
    
    //Ends the insertion if there is already a node with the same discriminator (skips vacant nodes)
    if(retrieve(newAcct._disc) != nullptr){
        return false;
        
    //Ends insertion if there's a vacant node that has the same discriminator
    }else if(doesExist(_root, newAcct._disc)){
        return false;
        
    //The other cases where you insert the new account
    }else{
        _root = insertNode(_root, newAcct);
        return true;
    }
}

/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool DTree::remove(int disc, DNode*& removed) {
    removed = nullptr;
    removeNode(_root, disc, removed);
    if(removed != nullptr){
        return true;
    }else{
        return false;
    }
}

/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode* DTree::retrieve(int disc) {
    return search(_root, disc);
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void DTree::clear() {
    deleteTree(_root);
}

/**
 * Prints all accounts' details within the DTree.
 */
void DTree::printAccounts() const {
    printInOrder(_root);
}

/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const {
    
    // # of nodes in the tree - # vacant nodes = # of non-vacant nodes
    int numUsers = 0;
    if(_root != nullptr){
        numUsers = _root->_size - _root->_numVacant;
    }
    return numUsers;
}

/**
 * Updates the size of a node based on the imedaite children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode* node) {
    
    //Default size if it's just the single node
    int newSize = 1;
    
    //Adds the size of the children nodes
    if(node->_left != nullptr){
        newSize = newSize + node->_left->_size;
    }
    if(node->_right != nullptr){
        newSize = newSize + node->_right->_size;
    }
    
    //Updates the size of the node
    node->_size = newSize;
}


/**
 * Updates the number of vacant nodes in a node's subtree based on the immediate children
 * @param node DNode object in which the number of vacant nodes in the subtree will be updated
 */
void DTree::updateNumVacant(DNode* node) {

    //Starts with 0 vacant nodes and adds one if the node itself is vacant
    int vacant = 0;
    if(node->_vacant){
        vacant = 1;
    }
    
    //Adds the the number of vacant nodes of the children nodes
    if(node->_left != nullptr){
        vacant = vacant + node->_left->_numVacant;
    }
    if(node->_right != nullptr){
        vacant = vacant + node->_right->_numVacant;
    }
    
    //Updates the number of vacant nodes
    node->_numVacant = vacant;
}

/**
 * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
 * @param checkImbalance DNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
bool DTree::checkImbalance(DNode* node) {
    
    //Size of the children stay 0 if they're null
    int sizeLeft = 0;
    int sizeRight = 0;
    
    //Getting the size of the children nodes
    if(node->_left != nullptr){
        sizeLeft = node->_left->_size;
    }
    if(node->_right != nullptr){
        sizeRight = node->_right->_size;
    }
    
    //No imbalance if both the children's size is less than 4
    if(sizeLeft < 4 && sizeRight < 4){
        return false;
        
    //The other cases where the size of a child is bigger than the other child
    }else if(sizeLeft < sizeRight){
        float left = sizeLeft;
        float right = sizeRight;
        
        //Checks if the the size of one child is at least 50% bigger than the other child
        left = left * 1.5;
        if(right >= left){
            return true;
        }else{
            return false;
        }
    }else if(sizeLeft > sizeRight){
        float left = sizeLeft;
        float right = sizeRight;
        
        //Checks if the the size of one child is at least 50% bigger than the other child
        right = right * 1.5;
        if(left >= right){
            return true;
        }else{
            return false;
        }
        
    //Else if the sizes are equal
    }else{
        return false;
    }
}

/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
DNode* DTree::rebalance(DNode* node){
    DNode** balanceArray = new DNode*[node->getSize() - node->_numVacant];
    int arrayIndex = 0;
    fillArray(node, balanceArray, arrayIndex);
    DNode* newRoot = balanceTree(balanceArray, 0, node->getSize() - node->_numVacant - 1);
    delete[] balanceArray;
    return newRoot;
}

/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream& operator<<(ostream& sout, const Account& acct) {
    sout << "Account name: " << acct.getUsername() <<
            "\n\tDiscriminator: " << acct.getDiscriminator() <<
            "\n\tNitro: " << acct.hasNitro() <<
            "\n\tBadge: " << acct.getBadge() <<
            "\n\tStatus: " << acct.getStatus();
    return sout;
}

//Helper function for the PrintAccounts function
//Uses inorder traversal and the overloaded insertion operator
void DTree::printInOrder(DNode* node) const{
    
    //Does nothing if the node is null and ends the recursive call
    if(node == nullptr){
        return;
    }
    
    //Visit the left node, print value of root, visit right node
    printInOrder(node->_left);
    //Don't print if the node is vacant
    if(!(node->_vacant)){
        cout << node->_account << endl;
    }
    printInOrder(node->_right);
}

//Helper function for the clear function
//Uses post-order traversal to delete the tree
void DTree::deleteTree(DNode* node){
    
    //Does nothing if the node is null and ends the recursive call
    if(node == nullptr) {
        return;
    }
        
    //Deletes the children first
    deleteTree(node->_left);
    deleteTree(node->_right);
        
    //Finally deletes the root node
    delete node;
    node = nullptr;
}

//Helper function for the overloaded assignment operator
//Uses pre-order traversal to make a deep copy of the tree
DNode* DTree::copyTree(DNode* node){
    
    //Returns null if the node is null and ends the recursive call
    if(node == nullptr){
        return nullptr;
    }
    
    //Dynamically creates a new DNode and copies the member variables
    DNode* newNode = new DNode(node->_account);
    newNode->_size = node->_size;
    newNode->_numVacant = node->_numVacant;
    newNode->_vacant = node->_vacant;
    
    //Copies the children nodes
    newNode->_left = copyTree(node->_left);
    newNode->_right = copyTree(node->_right);
    
    //Returns the copy node that is the root of the copied tree
    return newNode;
}


//Helper function for the retrieve function
//Uses pre-order traversal to search through the tree
DNode* DTree::search(DNode* node, int disc){
    
    //Returns null if the node is null and ends the recursive call
    if(node == nullptr){
        return nullptr;
        
    //When the node is the one we're searching for
    }else if(node->getDiscriminator() == disc){
        if(node->_vacant){
            return nullptr;
        }else{
            return node;
        }
        
    //Recursive search through the children
    }else{
        if(disc < node->getDiscriminator()){
            return search(node->_left, disc);
        }else if(disc > node->getDiscriminator()){
            return search(node->_right, disc);
        }
    }
    
    return nullptr;
}

//Helper function to insert
DNode* DTree::insertNode(DNode* node, Account acc){
    
    //If the root is null, create a new node and return it
    if(node == nullptr){
        DNode* newNode = new DNode(acc);
        return newNode;
    }
    
    //If the new account's discrimintator already exists in the tree
    if(node->getDiscriminator() == acc._disc){
        return node;
    }
    
    //If the new account can fit in a vacant node
    if(node->isVacant()){
        if(node->_left == nullptr && node->_right == nullptr){
            fillVacant(node, acc);
            return node;
        }else if(node->_left != nullptr && node->_right != nullptr){
            if(node->_left->getDiscriminator() < acc._disc && node->_right->getDiscriminator() > acc._disc){
                fillVacant(node, acc);
                return node;
            }
        }else if(node->_left == nullptr && node->_right != nullptr){
            if(node->_right->getDiscriminator() > acc._disc){
                fillVacant(node, acc);
                return node;
            }
        }else if(node->_left != nullptr && node->_right == nullptr){
            if(node->_left->getDiscriminator() < acc._disc){
                fillVacant(node, acc);
                return node;
            }
        }
    }
    
    //The other cases where the new account would be added
    if(acc._disc < node->getDiscriminator()){
        node->_left = insertNode(node->_left, acc);
    }else if(acc._disc > node->getDiscriminator()){
        node->_right = insertNode(node->_right, acc);
    }
    updateSize(node);
    updateNumVacant(node);
    if(checkImbalance(node)){
        node = rebalance(node);
    }

    return node;
}

//Helper function for the insert method
//Checks if a node with the certain discrimination already exists
bool DTree::doesExist(DNode* node, int disc){
    
    //loops through the find the node
    while(node != nullptr){
        if(node->getDiscriminator() == disc){
            return true;
        }else if(node->getDiscriminator() > disc){
            node = node->_left;
        }else{
            node = node->_right;
        }
    }
    
    return false;
}

//Helper function used by the rebalance function
//Takes the subtree and puts them in an ordered array
void DTree::fillArray(DNode* node, DNode ** array, int& index){
    if (node == nullptr){
        return;
    }
    
    fillArray(node->_left, array, index);
    if(!(node->_vacant)){
        array[index] = node;
        index = index + 1;
    }
    fillArray(node->_right, array, index);
    if(node->_vacant){
        delete node;
        node = nullptr;
    }
}

//Helper function used by the rebalance function
//Builds the tree in order and returns the root node
DNode* DTree::balanceTree(DNode** array, int start, int end){
    
    //End case
    if(start > end){
        return nullptr;
    }
    
    //Splits the array in half and chooses the node in the middle
    int mid = (start + end)/2;
    DNode* root = array[mid];

    //Builds the left subtree and the right subtree
    root->_left = balanceTree(array, start, mid - 1);
    root->_right = balanceTree(array, mid + 1, end);
    
    //Updates numVacant and size after rebalance
    updateNumVacant(root);
    updateSize(root);
    
    //Returns the subroot node
    return root;
}

//Helper function used by remove function
//Marks the node vacant to "remove" and updates numVacant and size
DNode* DTree::removeNode(DNode* node, int disc, DNode*& removed){
    
    //Returns null if the node is null and ends the recursive call
    if(node == nullptr){
        return nullptr;
        
    //When the node is the one we're searching for
    }else if(node->getDiscriminator() == disc){
        if(!node->_vacant){
            node->_vacant = true;
            updateNumVacant(node);
            removed = node;
            return node;
        }else{
            return node;
        }
        
    //Recursive search through the children
    }else{
        if(disc < node->getDiscriminator()){
            node->_left = removeNode(node->_left, disc, removed);
        }else if(disc > node->getDiscriminator()){
            node->_right = removeNode(node->_right, disc, removed);
        }
    }
    updateSize(node);
    updateNumVacant(node);
    
    return node;
}

//Helper function for the insertNode function
//Places the account in the vacant node
void DTree::fillVacant(DNode* node, Account acc){
    node->_account = acc;
    node->_numVacant = node->_numVacant - 1;
    node->_vacant = false;
}

