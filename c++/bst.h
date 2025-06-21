#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

//templated class for a node in a search tree
template <typename Key, typename Value>
class Node{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();
    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();
    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;
    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

//explicit node constructor
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) : item_(key, value), parent_(parent), left_(NULL), right_(NULL) {}

//node destructor
template<typename Key, typename Value>
Node<Key, Value>::~Node(){}

//const item getter
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const{
    return item_;
}

//non-const item getter
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem(){
    return item_;
}

//const key getter
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const{
    return item_.first;
}

//const value getter
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const{
    return item_.second;
}

//non-const value getter
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue(){
    return item_.second;
}

//gets parent of node
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const{
    return parent_;
}

//gets left child of node
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const{
    return left_;
}

//gets right child of node
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const{
    return right_;
}

//sets parent of node
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent){
    parent_ = parent;
}

//sets left child of node
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left){
    left_ = left;
}

//sets right child of node
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right){
    right_ = right;
}

//sets value of node
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value){
    item_.second = value;
}

//templated class of a binary search tree
template <typename Key, typename Value>
class BinarySearchTree{
public:
    BinarySearchTree(); 
    virtual ~BinarySearchTree(); 
    virtual void insert(const std::pair<const Key, Value>& new_item); 
    virtual void remove(const Key& key); 
    virtual void deletemin();
    virtual void deletemax();
    void clear();
    bool empty() const;

public:
    //internal BST iterator class for traversal
    class iterator{
    public:
        iterator();
        iterator(Node<Key,Value>* ptr);
        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;
        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;
        iterator& operator++();

    protected:
       friend class BinarySearchTree<Key, Value>;
        Node<Key, Value> *current_;
    };

public:
    virtual iterator begin() const;
    virtual iterator findmin() const;
    virtual iterator end() const;
    virtual iterator findmax() const;
    virtual iterator find(const Key& key) const;

protected:
    Node<Key, Value>* internalFind(const Key& k) const; 
    Node<Key, Value> *getSmallestNode() const;  
    Node<Key, Value> *getLargestNode() const;  
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); 
    virtual void nodeSwap(Node<Key,Value>* n1, Node<Key,Value>* n2);
    void clear2(Node<Key, Value>* node);
    virtual void nochild(Node<Key, Value>* node);
    virtual void onechild(Node<Key, Value>* node);

protected:
    Node<Key, Value>* root_;
};

//explicit BST iterator constructor
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr){
    current_ = ptr;
}

//default BST iterator constructor
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(){
    current_ = NULL;
}

//gives access to item
template<class Key, class Value>
std::pair<const Key,Value>& BinarySearchTree<Key, Value>::iterator::operator*() const{
    return current_->getItem();
}

//gives access to address of item
template<class Key, class Value>
std::pair<const Key,Value>* BinarySearchTree<Key, Value>::iterator::operator->() const{
    return &(current_->getItem());
}

//checks if internals are the same
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const{
    return current_ == rhs.current_;
}

//checks if internals are different
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const{
    return current_ != rhs.current_;
}

//advances iterator item using in order traversal
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++(){
   if(current_->getRight() != NULL){                       
        current_ = current_->getRight();
        while(current_->getLeft() != NULL){                 
            current_ = current_->getLeft();
        }
        return *this;
    }
    Node<Key, Value>* parent = current_->getParent();
    while((parent != NULL) && (current_ == parent->getRight())){      
        current_ = parent;
        parent = parent->getParent();
    }
    current_ = parent;                                                
    return *this;
}

//BST constructor
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL) {}

//BST destructor
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree(){
    clear();
}

//checks if tree is empty
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const{
    return root_ == NULL;
}

//returns iterator to first item in tree
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

//returns an iterator whose value means invalid
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

//returns an iterator to smallest item in tree
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::findmin() const{
    BinarySearchTree<Key, Value>::iterator min(getSmallestNode());
    return min;
}

//returns an iterator to largest item in tree
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::findmax() const{
    BinarySearchTree<Key, Value>::iterator max(getLargestNode());
    return max;
}

//returns iterator to item with given key
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key & key) const{
    BinarySearchTree<Key, Value>::iterator it(internalFind(key));
    return it;
}

//inserts item into tree
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item){
    Node<Key, Value>* add = new Node<Key, Value>(new_item.first, new_item.second, NULL);
    if(root_ == NULL){              
        root_ = add;                
        return;
    }
    Node<Key, Value>* node = root_;
    while(1){                                               
        if(new_item.first == node->getKey()){          
            node->setValue(new_item.second);          
            delete add;
            return;
        }
        else if(new_item.first < node->getKey()){      
            if(node->getLeft() == NULL){                   
                node->setLeft(add);                        
                add->setParent(node);
                return;
            }
            node = node->getLeft();
        }
        else{                                               
            if(node->getRight() == NULL){                  
                node->setRight(add);                       
                add->setParent(node);
                return;
            }
            node = node->getRight();
        }
    }
}

//removes item with given key
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key){
    Node<Key, Value>* node = internalFind(key);
    if(node == NULL){                                                          
        return;
    }
    if((node->getLeft() == NULL) && (node->getRight() == NULL)){              
        nochild(node);                                             
    }
    else if((node->getLeft() == NULL) || (node->getRight() == NULL)){         
        onechild(node);
    }
    else{                                                                       
        nodeSwap(node, predecessor(node));                                                 
        if((node->getLeft() == NULL) && (node->getRight() == NULL)){          
            nochild(node);
        }
        else{                                                                   
            onechild(node);
        }                                         
    }
    delete node;
}

//performs BST remove algorithm on min value of the tree
template<class Key, class Value>
void BinarySearchTree<Key, Value>::deletemin(){
    Node<Key, Value>* min = getSmallestNode();
    remove(min->getKey());
}

//performs BST remove algorithm on max value of the tree
template<class Key, class Value>
void BinarySearchTree<Key, Value>::deletemax(){
    Node<Key, Value>* max = getLargestNode();
    remove(max->getKey());
}
//remove helper function that deals with removing a node with zero children
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nochild(Node<Key, Value>* node){                     
    if(node->getParent() == NULL){                                                     
        root_ = NULL;                                                                   
    }
    else if(node == node->getParent()->getLeft()){                                    
        node->getParent()->setLeft(NULL);                                              
    }
    else{                                                                               
        node->getParent()->setRight(NULL);                                             
    }                           
}

//remove helper function that deals with removing a node with one child
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::onechild(Node<Key, Value>* node){                    
    Node<Key, Value>* child;
    if(node->getLeft() != NULL){                                                            
        child = node->getLeft();
    }
    else{                                                                              
        child = node->getRight();                                                 
    }
    if(node->getParent() == NULL){                                                     
        root_ = child;
    }
    else if(node == node->getParent()->getLeft()){                                    
        node->getParent()->setLeft(child);                                            
    }
    else{                                                                               
        node->getParent()->setRight(child);                                           
    }
    child->setParent(node->getParent());                                              
}

//returns pointer to predecessor of given node
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current){
   if(current->getLeft() != NULL){                      
        current = current->getLeft();
        while(current->getRight() != NULL){                 
            current = current->getRight();
        }
        return current;
    }
    Node<Key, Value>* parent = current->getParent();
    while((parent != NULL) && (current == parent->getLeft())){      
        current = parent;
        parent = parent->getParent();
    }
    return parent;                                                 
}

//clears out and resets tree
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear(){
    if(root_ == NULL){      
        return;
    }
    clear2(root_);         
    root_ = NULL;           
}

//helper function for clear
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear2(Node<Key, Value>* node){  
    if(node == NULL){           
        return;
    }
    clear2(node->getLeft());                
    clear2(node->getRight());               
    delete node;                            
}

//returns pointer to smallest node in the tree
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
    if(root_ == NULL){                          
        return NULL;
    }
    Node<Key, Value>* node = root_;
    while(node->getLeft() != NULL){           
        node = node->getLeft();
    }
    return node;                               
}

//returns pointer to largest node in the tree
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getLargestNode() const
{
    if(root_ == NULL){                          
        return NULL;
    }
    Node<Key, Value>* node = root_;
    while(node->getRight() != NULL){           
        node = node->getRight();
    }
    return node;                               
}

//returns pointer to node with given key
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const{
    if(root_ == NULL){                     
        return NULL;
    }
    Node<Key, Value>* node = root_;
    while(node != NULL){
        if(key == node->getKey()){     
            return node;
        }
        else if(key < node->getKey()){     
            node = node->getLeft();
        }
        else{                               
            node = node->getRight();
        }
    }
    return NULL;                            
}

//helper function that swaps two nodes
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key,Value>* n1, Node<Key,Value>* n2){
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL)) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if((n1p != NULL) && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if((n2p != NULL) && (n2 == n2p->getLeft())) n2isLeft = true;
    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);
    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);
    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);
    if((n1r != NULL) && (n1r == n2)) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if((n2r != NULL) && (n2r == n1)){
        n1->setRight(n2);
        n2->setParent(n1);
    }
    else if((n1lt != NULL) && (n1lt == n2)){
        n2->setLeft(n1);
        n1->setParent(n2);
    }
    else if((n2lt != NULL) && (n2lt == n1)){
        n1->setLeft(n2);
        n2->setParent(n1);
    }
    if((n1p != NULL) && (n1p != n2)){
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if((n1r != NULL) && (n1r != n2)){
        n1r->setParent(n2);
    }
    if((n1lt != NULL) && (n1lt != n2)){
        n1lt->setParent(n2);
    }
    if((n2p != NULL) && (n2p != n1)){
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if((n2r != NULL) && (n2r != n1)){
        n2r->setParent(n1);
    }
    if((n2lt != NULL) && (n2lt != n1)){
        n2lt->setParent(n1);
    }
    if(this->root_ == n1){
        this->root_ = n2;
    }
    else if(this->root_ == n2){
        this->root_ = n1;
    }
}

#endif