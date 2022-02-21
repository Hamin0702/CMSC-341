#include "utree.h"
#include <random>

#define NUMACCTS 20
#define RANDDISC (distAcct(rng))

std::mt19937 rng(10);
std::uniform_int_distribution<> distAcct(0, 9999);

class Tester {
public:
    bool testBasicDTreeInsert(DTree& dtree);
    bool testDTreeDeepCopy();
    bool testDTreeBalance();
    bool testDTreeBSTrule();
    bool testDTreeRemove();
    bool testDTreeEdgeCase();
    bool testBasicUTreeInsert(UTree& utree);
    bool testUTreeBalance();
    bool testUTreeBSTrule();
    bool testUTreeRemove();
    bool testUTreeEdgeCase();
    void testDeepCopy();
};

int main() {
    Tester tester;

    /* Basic dtree tests */
    DTree dtree;
    
    cout << "Testing DTree insertion...";
    if(tester.testBasicDTreeInsert(dtree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting DTree:" << endl;
    dtree.dump();
    cout << endl;
 
    //Testing DTree overloaded assignment operator
    cout << "\nTesting DTree Overloaded Assignment Operator...";
    if(tester.testDTreeDeepCopy()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }
 
    //Testing the balance of DTree after insertion
    cout << "\nTesting DTree balance after insertion...";
    if(tester.testDTreeBalance()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    //Testing the DTree BST property after insertion
    cout << "\nTesting DTree BST property after insertion...";
    if(tester.testDTreeBSTrule()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    //Testing the remove functionality of DTree
    cout << "\nTesting DTree remove...";
    if(tester.testDTreeRemove()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    //Testing the edge cases of DTree
    cout << "\nTesting DTree edge cases...";
    if(tester.testDTreeEdgeCase()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }
    
    /* Basic UTree tests */
    /*UTree utree;

    cout << "\n\nTesting UTree insertion...";
    if(tester.testBasicUTreeInsert(utree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting UTree:" << endl;
    utree.dump();
    cout << endl;
    */
    
    //Testing the balance of UTree after insertion
    cout << "\nTesting balance of UTree after insertion...";
    if(tester.testUTreeBalance()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }
  
    //Testing the BST property of UTree after insertion
    cout << "\nTesting BST property of UTree after insertion...";
    if(tester.testUTreeBSTrule()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    //Testing the remove functionality of UTree
    cout << "\nTesting UTree remove...";
    if(tester.testUTreeRemove()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }
     
    //Testing UTree edge cases
    cout << "\nTesting UTree edge cases...";
    if(tester.testUTreeEdgeCase()) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }
     
    return 0;
}

bool Tester::testUTreeEdgeCase(){
    bool testPass = true;
    UTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("00hamin", 1, false, "member", "Idle");
    
    //Removing from an empty tree
    DNode* removed = nullptr;
    testPass = !tree.removeUser("No Body", 2, removed);
    if(!testPass)return false;
    
    //Removing a different node that doesn't exist
    tree.insert(ac0);
    testPass = !tree.removeUser("No Body", 0, removed);
    if(!testPass)return false;
    
    //Inserting a node that already exists
    testPass = !tree.insert(ac0);
    if(!testPass)return false;
    
    //Simple removal, makes the tree empty
    testPass = tree.removeUser("00hamin", 0, removed);
    if(!testPass)return false;
    
    //Removing a node that's already removed
    testPass = !tree.removeUser("00hamin", 0, removed);
    if(!testPass)return false;
    
    //Removing a user that has been marked vacant
    tree.insert(ac0);
    tree.insert(ac1);
    tree.removeUser("00hamin", 1, removed);
    testPass = !tree.removeUser("00hamin", 1, removed);
    if(!testPass)return false;
    
    return testPass;
}

bool Tester::testUTreeRemove(){
    UTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    Account ac4 = Account("04hamin", 4, false, "member", "Idle");
    Account ac5 = Account("05hamin", 5, false, "member", "Idle");
    Account ac6 = Account("06hamin", 6, false, "member", "Idle");
    Account ac7 = Account("07hamin", 7, false, "member", "Idle");
    Account ac8 = Account("08hamin", 8, false, "member", "Idle");
    Account ac9 = Account("09hamin", 9, false, "member", "Idle");
    
    tree.insert(ac0);
    tree.insert(ac1);
    tree.insert(ac2);
    tree.insert(ac3);
    tree.insert(ac4);
    tree.insert(ac5);
    tree.insert(ac6);
    tree.insert(ac7);
    tree.insert(ac8);
    tree.insert(ac9);
    
    bool passTest = true;
    
    //Removing a node that has a right child but no left child
    DNode* removed = nullptr;
    if(!tree.removeUser("08hamin", 8, removed)){
        return false;
    }
    if(tree.retrieve("08hamin") != nullptr){
        return false;
    }
    
    //Removing a node that has no children
    if(!tree.removeUser("09hamin", 9, removed)){
        return false;
    }
    if(tree.retrieve("09hamin") != nullptr){
        return false;
    }
        
    //Removing a node that's in the middle of the tree, with two children
    if(!tree.removeUser("03hamin", 3, removed)){
        return false;
    }
    if(tree.retrieve("03hamin") != nullptr){
        return false;
    }
    
    //Removing a node on the left side, with no children
    if(!tree.removeUser("00hamin", 0, removed)){
        return false;
    }
    if(tree.retrieve("00hamin") != nullptr){
        return false;
    }

    return passTest;
}

bool Tester::testUTreeBSTrule(){
    UTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    Account ac4 = Account("04hamin", 4, false, "member", "Idle");
    Account ac5 = Account("05hamin", 5, false, "member", "Idle");
    Account ac6 = Account("06hamin", 6, false, "member", "Idle");
    Account ac7 = Account("07hamin", 7, false, "member", "Idle");
    Account ac8 = Account("08hamin", 8, false, "member", "Idle");
    Account ac9 = Account("09hamin", 9, false, "member", "Idle");
    
    tree.insert(ac0);
    tree.insert(ac1);
    tree.insert(ac2);
    tree.insert(ac3);
    tree.insert(ac4);
    tree.insert(ac5);
    tree.insert(ac6);
    tree.insert(ac7);
    tree.insert(ac8);
    tree.insert(ac9);
    
    bool passTest = true;
    
    //After all the insertion and the balancing, check each node that it's following the BST Property
    
    //00
    if(tree.retrieve("00hamin")->_left != nullptr){
        if(tree.retrieve("00hamin")->_left->getUsername() > tree.retrieve("00hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("00hamin")->_right != nullptr){
        if(tree.retrieve("00hamin")->_right->getUsername() < tree.retrieve("00hamin")->getUsername()){
            return false;
        }
    }
    
    //01
    if(tree.retrieve("01hamin")->_left != nullptr){
        if(tree.retrieve("01hamin")->_left->getUsername() > tree.retrieve("01hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("01hamin")->_right != nullptr){
        if(tree.retrieve("01hamin")->_right->getUsername() < tree.retrieve("01hamin")->getUsername()){
            return false;
        }
    }
    
    //02
    if(tree.retrieve("02hamin")->_left != nullptr){
        if(tree.retrieve("02hamin")->_left->getUsername() > tree.retrieve("02hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("02hamin")->_right != nullptr){
        if(tree.retrieve("02hamin")->_right->getUsername() < tree.retrieve("02hamin")->getUsername()){
            return false;
        }
    }
    
    //03
    if(tree.retrieve("03hamin")->_left != nullptr){
        if(tree.retrieve("03hamin")->_left->getUsername() > tree.retrieve("03hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("03hamin")->_right != nullptr){
        if(tree.retrieve("03hamin")->_right->getUsername() < tree.retrieve("03hamin")->getUsername()){
            return false;
        }
    }
    
    //04
    if(tree.retrieve("04hamin")->_left != nullptr){
        if(tree.retrieve("04hamin")->_left->getUsername() > tree.retrieve("04hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("04hamin")->_right != nullptr){
        if(tree.retrieve("04hamin")->_right->getUsername() < tree.retrieve("04hamin")->getUsername()){
            return false;
        }
    }
    
    //05
    if(tree.retrieve("05hamin")->_left != nullptr){
        if(tree.retrieve("05hamin")->_left->getUsername() > tree.retrieve("05hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("05hamin")->_right != nullptr){
        if(tree.retrieve("05hamin")->_right->getUsername() < tree.retrieve("05hamin")->getUsername()){
            return false;
        }
    }
    
    //06
    if(tree.retrieve("06hamin")->_left != nullptr){
        if(tree.retrieve("06hamin")->_left->getUsername() > tree.retrieve("06hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("06hamin")->_right != nullptr){
        if(tree.retrieve("06hamin")->_right->getUsername() < tree.retrieve("06hamin")->getUsername()){
            return false;
        }
    }
    
    //07
    if(tree.retrieve("07hamin")->_left != nullptr){
        if(tree.retrieve("07hamin")->_left->getUsername() > tree.retrieve("07hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("07hamin")->_right != nullptr){
        if(tree.retrieve("07hamin")->_right->getUsername() < tree.retrieve("07hamin")->getUsername()){
            return false;
        }
    }
    
    //08
    if(tree.retrieve("08hamin")->_left != nullptr){
        if(tree.retrieve("08hamin")->_left->getUsername() > tree.retrieve("08hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("08hamin")->_right != nullptr){
        if(tree.retrieve("08hamin")->_right->getUsername() < tree.retrieve("08hamin")->getUsername()){
            return false;
        }
    }
    
    //09
    if(tree.retrieve("09hamin")->_left != nullptr){
        if(tree.retrieve("09hamin")->_left->getUsername() > tree.retrieve("09hamin")->getUsername()){
            return false;
        }
    }
    if(tree.retrieve("09hamin")->_right != nullptr){
        if(tree.retrieve("09hamin")->_right->getUsername() < tree.retrieve("09hamin")->getUsername()){
            return false;
        }
    }
    
    
    return passTest;
}

bool Tester::testUTreeBalance(){
    UTree tree;
    UTree second;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    Account ac4 = Account("04hamin", 4, false, "member", "Idle");
    Account ac5 = Account("05hamin", 5, false, "member", "Idle");
    Account ac6 = Account("06hamin", 6, false, "member", "Idle");
    Account ac7 = Account("07hamin", 7, false, "member", "Idle");
    Account ac8 = Account("08hamin", 8, false, "member", "Idle");
    Account ac9 = Account("09hamin", 9, false, "member", "Idle");
    
    tree.insert(ac0);
    tree.insert(ac1);
    tree.insert(ac2);
    tree.insert(ac3);
    tree.insert(ac4);
    tree.insert(ac5);
    tree.insert(ac6);
    tree.insert(ac7);
    tree.insert(ac8);
    tree.insert(ac9);
    
    bool passTest = true;
    
    //After insertion and the balancing, check each node that it's balanced
    
    if(tree.checkImbalance(tree.retrieve("00hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("01hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("02hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("03hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("04hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("05hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("06hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("07hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("08hamin")) > 1) return false;
    if(tree.checkImbalance(tree.retrieve("09hamin")) > 1) return false;
    
    second.insert(ac9);
    second.insert(ac8);
    second.insert(ac7);
    second.insert(ac6);
    second.insert(ac5);
    second.insert(ac4);
    second.insert(ac3);
    second.insert(ac2);
    second.insert(ac1);
    second.insert(ac0);
    
    //Same thing, but with another tree
    
    if(tree.checkImbalance(second.retrieve("00hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("01hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("02hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("03hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("04hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("05hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("06hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("07hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("08hamin")) > 1) return false;
    if(tree.checkImbalance(second.retrieve("09hamin")) > 1) return false;
    
    return passTest;
}

bool Tester::testDTreeEdgeCase(){
    bool testPass = true;
    DTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    
    //Removing from an empty tree
    DNode* removed = nullptr;
    testPass = !tree.remove(3, removed);
    if(!testPass)return false;
    
    //Removing a different node that doesn't exist
    tree.insert(ac0);
    testPass = !tree.remove(2, removed);
    if(!testPass)return false;
    
    //Inserting a node that already exists
    testPass = !tree.insert(ac0);
    if(!testPass)return false;
    
    //Simple removal, makes the tree empty
    testPass = tree.remove(0, removed);
    if(!testPass)return false;
    
    //Removing a node that's already removed
    testPass = !tree.remove(0, removed);
    if(!testPass)return false;
    
    return testPass;
}

bool Tester::testDTreeRemove(){
    bool testPass = true;
    DTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    Account ac4 = Account("04hamin", 4, false, "member", "Idle");
    Account ac5 = Account("05hamin", 5, false, "member", "Idle");
    Account ac6 = Account("06hamin", 6, false, "member", "Idle");
    Account ac7 = Account("07hamin", 7, false, "member", "Idle");
    Account ac8 = Account("08hamin", 8, false, "member", "Idle");
    Account ac9 = Account("09hamin", 9, false, "member", "Idle");
    
    tree.insert(ac0);
    tree.insert(ac1);
    tree.insert(ac2);
    tree.insert(ac3);
    tree.insert(ac4);
    
    DNode* removed = nullptr;
    testPass = tree.remove(3, removed);
    removed = nullptr;
    testPass = tree.remove(4, removed);
    
    tree.insert(ac5);
    tree.insert(ac6);
    tree.insert(ac7);
    tree.insert(ac8);
    tree.insert(ac9);
    
    //After inserting some nodes, deleting few, and inserting more to cause a rebalance, checks if the nodes are deleted
    testPass = !tree.retrieve(3);
    testPass = !tree.retrieve(4);
    
    return testPass;
}

bool Tester::testDTreeBSTrule(){
    DTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    Account ac4 = Account("04hamin", 4, false, "member", "Idle");
    Account ac5 = Account("05hamin", 5, false, "member", "Idle");
    Account ac6 = Account("06hamin", 6, false, "member", "Idle");
    Account ac7 = Account("07hamin", 7, false, "member", "Idle");
    Account ac8 = Account("08hamin", 8, false, "member", "Idle");
    Account ac9 = Account("09hamin", 9, false, "member", "Idle");
    
    tree.insert(ac0);
    tree.insert(ac1);
    tree.insert(ac2);
    tree.insert(ac3);
    tree.insert(ac4);
    tree.insert(ac5);
    tree.insert(ac6);
    tree.insert(ac7);
    tree.insert(ac8);
    tree.insert(ac9);
    
    //After insertion and the balancing, checks that each node follows the BST property
    
    bool passTest = true;
    for(int i = 0; i < 9; i++){
        if(tree.retrieve(i)->_left != nullptr){
            if(tree.retrieve(i)->_left->getDiscriminator() > tree.retrieve(i)->getDiscriminator()){
                return false;
            }
        }
        if(tree.retrieve(i)->_right != nullptr){
            if(tree.retrieve(i)->_right->getDiscriminator() < tree.retrieve(i)->getDiscriminator()){
                return false;
            }
        }
    }
    return passTest;
}

bool Tester::testDTreeBalance(){
    DTree tree;
    Account ac0 = Account("00hamin", 0, false, "member", "Idle");
    Account ac1 = Account("01hamin", 1, false, "member", "Idle");
    Account ac2 = Account("02hamin", 2, false, "member", "Idle");
    Account ac3 = Account("03hamin", 3, false, "member", "Idle");
    Account ac4 = Account("04hamin", 4, false, "member", "Idle");
    Account ac5 = Account("05hamin", 5, false, "member", "Idle");
    Account ac6 = Account("06hamin", 6, false, "member", "Idle");
    Account ac7 = Account("07hamin", 7, false, "member", "Idle");
    Account ac8 = Account("08hamin", 8, false, "member", "Idle");
    Account ac9 = Account("09hamin", 9, false, "member", "Idle");
    
    tree.insert(ac0);
    tree.insert(ac1);
    tree.insert(ac2);
    tree.insert(ac3);
    tree.insert(ac4);
    tree.insert(ac5);
    tree.insert(ac6);
    tree.insert(ac7);
    tree.insert(ac8);
    tree.insert(ac9);
    
    //After insertion and the balancing, checks that each node is balanced
    
    bool passTest = true;
    for(int i = 0; i < 9; i++){
        if(tree.checkImbalance(tree.retrieve(i))){
            passTest = false;
        }
    }
    return passTest;
}

bool Tester::testDTreeDeepCopy(){
    bool passTest = true;
    DTree tree;
    Account ac1 = Account("Hamin0702", 2, true, "mod", "Playing league");
    tree.insert(ac1);
    DTree copy;
    copy = tree;
    
    //Checks if the node is copied
    if(!(tree.getUsername() == copy.getUsername())){
        passTest = false;
    }
    
    DNode* removed = nullptr;
    tree.remove(2, removed);
    //Checks if the copy is a deep copy
    if(tree.getNumUsers() == copy.getNumUsers()){
        passTest = false;
    }
    
    return passTest;
}

bool Tester::testBasicDTreeInsert(DTree& dtree) {
    bool allInserted = true;
    for(int i = 0; i < NUMACCTS; i++) {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if(!dtree.insert(newAcct)) {
            cout << "Insertion on node " << disc << " did not return true" << endl;
            allInserted = false;
        }
    }
    return allInserted;
}

bool Tester::testBasicUTreeInsert(UTree& utree) {
    string dataFile = "accounts.csv";
    try {
        utree.loadData(dataFile);
    } catch(std::invalid_argument e) {
        std::cerr << e.what() << endl;
        return false;
    }
    return true;
}
