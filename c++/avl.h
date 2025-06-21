#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

//special type of BST node that keeps track of height
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>{
public:
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();
    int getHeight() const;
    void setHeight(int height);
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;
protected:
    int height_;
};

//explicit AVL node constructor
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) : Node<Key, Value>(key, value, parent), height_(1){}

//AVL node destructor
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode(){}

//AVL node height getter
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const{
    return height_;
}

//AVL node height setter
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height){
    height_ = height;
}

//gets parent of AVL node
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

//gets left child of AVL node
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

//gets right child of AVL node
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

//templated class of an AVL tree 
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>{
public:
    virtual void insert(const std::pair<const Key, Value> &new_item); 
    virtual void remove(const Key& key);  
    virtual void deletemin();
    virtual void deletemax();
protected:
    virtual void nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void insertfix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void left(AVLNode<Key, Value>* n);
    void right(AVLNode<Key, Value>* n);
    int sub(AVLNode<Key, Value>* n);
    void removefix(AVLNode<Key, Value>* n);

};

//AVL tree insert algorithm
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item){
    AVLNode<Key, Value>* add = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    if(this->root_ == NULL){
        this->root_ = add;
        return;
    }
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->root_);
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
                if(node->getHeight() == 1){                
                    node->setHeight(2);                    
                    insertfix(node, add);                  
                }
                return;
            }
            node = node->getLeft();
        }
        else{                                               
            if(node->getRight() == NULL){                  
                node->setRight(add);                       
                add->setParent(node);
                if(node->getHeight() == 1){                
                    node->setHeight(2);                    
                    insertfix(node, add);                  
                }
                return;
            }
            node = node->getRight();
        }
    }
}

//AVL tree remove algorithm
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key){
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(node == NULL){                                                                              
        return;
    }
    if((node->getLeft() == NULL) && (node->getRight() == NULL)){                                  
        this->nochild(node);                                             
    }
    else if((node->getLeft() == NULL) || (node->getRight() == NULL)){                             
        this->onechild(node);
    }
    else{                                                                                           
        nodeSwap(node, static_cast<AVLNode<Key, Value>*>(this->predecessor(node)));                                                                         
        if((node->getLeft() == NULL) && (node->getRight() == NULL)){                              
            this->nochild(node);
        }
        else{                                                                                       
            this->onechild(node);
        }                                         
    }
    AVLNode<Key, Value>* p = node->getParent();
    delete node;                                                                                   
    removefix(p);                                                                                   
}

//performs AVL remove algorithm on min value of the tree
template<class Key, class Value>
void AVLTree<Key, Value>::deletemin(){
    AVLNode<Key, Value>* min = static_cast<AVLNode<Key, Value>*>(this->getSmallestNode());
    remove(min->getKey());
}

//performs AVL remove algorithm on max value of the tree
template<class Key, class Value>
void AVLTree<Key, Value>::deletemax(){
    AVLNode<Key, Value>* max = static_cast<AVLNode<Key, Value>*>(this->getLargestNode());
    remove(max->getKey());
}

//balancing helper function for insert algorithm
template<class Key, class Value>
void AVLTree<Key, Value>::insertfix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){                
    AVLNode<Key, Value>* g = p->getParent();
    if(g == NULL){                                                                            
        return;
    }
    int height = std::max(sub(g->getLeft()), sub(g->getRight())) + 1;
    if(g->getHeight() == height){
        return;                                                                                     
    }
    g->setHeight(height);
    if(std::abs(sub(g->getLeft()) - sub(g->getRight())) < 2){                           
        insertfix(g, p);
    }
    else{                                                                                           
        if(n == p->getLeft()){
            if(p == g->getLeft()){                                                       
                right(g);
            }
            else if(p == g->getRight()){                                                 
                right(p);
                left(g);
            }
        }
        else if(n == p->getRight()){       
            if(p == g->getRight()){                                                      
                left(g);
            }
            else if(p == g->getLeft()){                                                  
                left(p);
                right(g);
            }
        }
    }
}

//performs a left rotation
template<class Key, class Value>
void AVLTree<Key, Value>::left(AVLNode<Key, Value>* n){                                                  
    AVLNode<Key, Value>* x = n->getRight();                                                 
    AVLNode<Key, Value>* b = x->getLeft();                                                  
    AVLNode<Key, Value>* p = n->getParent();                                                
    x->setLeft(n);                                                                          
    if(b != NULL){
        b->setParent(n);                                                                    
    }
    x->setParent(p);                                                                        
    n->setRight(b);                                                                         
    n->setParent(x);                                                                
    if(p == NULL){
        this->root_ = x;                                                                    
    }
    else if(n == p->getLeft()){
        p->setLeft(x);                                                                     
    }
    else{
        p->setRight(x);                                                                     
    }
    n->setHeight(std::max(sub(n->getLeft()), sub(n->getRight())) + 1);
    x->setHeight(std::max(sub(x->getLeft()), sub(x->getRight())) + 1);
    if(p != NULL){
        p->setHeight(std::max(sub(p->getLeft()), sub(p->getRight())) + 1);
    }
}

//performs a right rotation
template<class Key, class Value>
void AVLTree<Key, Value>::right(AVLNode<Key, Value>* n){                                                                                    
    AVLNode<Key, Value>* x = n->getLeft();                                                  
    AVLNode<Key, Value>* b = x->getRight();                                                 
    AVLNode<Key, Value>* p = n->getParent();                                                
    x->setRight(n);                                                                         
    if(b != NULL){
        b->setParent(n);                                                                    
    }
    x->setParent(p);                                                                        
    n->setLeft(b);
    n->setParent(x);
    if(p == NULL){  
        this->root_ = x;                                                                    
    }
    else if(n == p->getLeft()){
        p->setLeft(x);                                                                     
    }
    else{
        p->setRight(x);                                                                     
    }
    n->setHeight(std::max(sub(n->getLeft()), sub(n->getRight())) + 1);
    x->setHeight(std::max(sub(x->getLeft()), sub(x->getRight())) + 1);
    if(p != NULL){
        p->setHeight(std::max(sub(p->getLeft()), sub(p->getRight())) + 1);
    }
}

//returns height of an AVL node's subtree
template<class Key, class Value>
int AVLTree<Key, Value>:: sub(AVLNode<Key, Value>* n){                                     
    if(n == NULL){
        return 0;
    }
    return n->getHeight();
}

//balancing helper function for remove algorithm
template<class Key, class Value>
void AVLTree<Key, Value>::removefix(AVLNode<Key,Value>* n){                                
    if(n == NULL){                                                                      
        return;
    }
    if(std::abs(sub(n->getLeft()) - sub(n->getRight())) > 1){                       
        AVLNode<Key, Value>* c;                                                            
        AVLNode<Key, Value>* g;                                                            
        AVLNode<Key, Value>* p = n->getParent();                                        
        if(sub(n->getLeft()) > sub(n->getRight())){                                 
            c = n->getLeft();
        }
        else{                                                                               
            c = n->getRight();
        }
        if(sub(c->getLeft()) > sub(c->getRight())){                                         
            g = c->getLeft();
        }
        else if(sub(c->getLeft()) < sub(c->getRight())){                                    
            g = c->getRight();
        }
        else{                                                                               
            if(c == n->getLeft()){
                g = c->getLeft();
            }
            else{
                g = c->getRight();
            }
        }
        if(g == c->getLeft()){
            if(c == n->getLeft()){                                                      
                right(n);
            }
            else if(c == n->getRight()){                                                
                right(c);
                left(n);
            }
        }
        else if(g == c->getRight()){       
            if(c == n->getRight()){                                                     
                left(n);
            }
            else if(c == n->getLeft()){                                                 
                left(c);
                right(n);
            }
        }
        removefix(p);                                                                                 
    }
    int height = std::max(sub(n->getLeft()), sub(n->getRight())) + 1;               
    if(height == n->getHeight()){                                                       
        return;
    }
    n->setHeight(height);
    if(std::abs(sub(n->getLeft()) - sub(n->getRight())) < 2){                       
        removefix(n->getParent());                                                     
    }
}

//swaps two AVL nodes
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2){
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif