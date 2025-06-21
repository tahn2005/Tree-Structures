#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

//templated class of a Splay tree 
template <class Key, class Value>
class SplayTree : public BinarySearchTree<Key, Value>{
public:
    virtual void insert(const std::pair<const Key, Value> &new_item); 
    virtual void remove(const Key& key);  
    virtual void deletemin();
    virtual void deletemax();
    virtual typename BinarySearchTree<Key, Value>::iterator findmin();
    virtual typename BinarySearchTree<Key, Value>::iterator findmax();
    virtual typename BinarySearchTree<Key, Value>::iterator find(const Key& key);
    
protected:
    void splay(Node<Key, Value>* n);
    void left(Node<Key, Value>* n);
    void right(Node<Key, Value>* n);
};

//returns iterator to smallest item in tree and splays item to root
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator SplayTree<Key, Value>::findmin(){
    
    Node<Key, Value>* node = this->getSmallestNode();
    splay(node);
    typename BinarySearchTree<Key, Value>::iterator begin(node);
    return begin;
}

//returns an iterator to largest item in tree and splays item to root
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator SplayTree<Key, Value>::findmax(){
    Node<Key, Value>* node = this->getLargestNode();
    splay(node);
    typename BinarySearchTree<Key, Value>::iterator max(node);
    return max;
}

//returns iterator to item with given key and splays item to root
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator SplayTree<Key, Value>::find(const Key & key){
    Node<Key, Value>* node = this->internalFind(key);
    typename BinarySearchTree<Key, Value>::iterator it(node);
    return it;
}

//splay tree insert algorithm
template<class Key, class Value>
void SplayTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item){
    Node<Key, Value>* add = new Node<Key, Value>(new_item.first, new_item.second, NULL);
    if(this->root_ == NULL){              
        this->root_ = add;                
        return;
    }
    Node<Key, Value>* node = this->root_;
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
                splay(add);
                return;
            }
            node = node->getLeft();
        }
        else{                                               
            if(node->getRight() == NULL){                  
                node->setRight(add);                       
                add->setParent(node);
                splay(add);
                return;
            }
            node = node->getRight();
        }
    }
}

//splay tree remove algorithm
template<class Key, class Value>
void SplayTree<Key, Value>::remove(const Key& key){
    Node<Key, Value>* node = this->internalFind(key);
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
        this->nodeSwap(node, this->predecessor(node));                                                 
        if((node->getLeft() == NULL) && (node->getRight() == NULL)){          
            this->nochild(node);
        }
        else{                                                                   
            this->onechild(node);
        }                                         
    }
    splay(node->getParent());
    delete node;
}

//removes min value from the tree and splays new min value to the root
template<class Key, class Value>
void SplayTree<Key, Value>::deletemin(){
    Node<Key, Value>* min = this->getSmallestNode();
    splay(min);
    remove(min->getKey());
}

//removes max value from the tree and splays new max value to the root
template<class Key, class Value>
void SplayTree<Key, Value>::deletemax(){
    Node<Key, Value>* max = this->getLargestNode();
    splay(max);
    remove(max->getKey());
}

//splay tree splay algorithm
template<class Key, class Value>
void SplayTree<Key, Value>::splay(Node<Key, Value>* n){
    if(n == NULL){
        return;
    }
    while(n->getParent() != NULL){
        Node<Key, Value>* p = n->getParent();
        Node<Key, Value>* g = p->getParent();
        if(g == NULL){
            if(n == p->getLeft()){
                right(p);
            }
            else{
                left(p);
            }
        }
        else{
            if(n == p->getLeft()){
                if(p == g->getLeft()){                                                       
                    right(g);
                    right(p);
                }
                else if(p == g->getRight()){                                                 
                    right(p);
                    left(g);
                }   
            }
            else if(n == p->getRight()){       
                if(p == g->getRight()){                                                      
                    left(g);
                    left(p);
                }
                else if(p == g->getLeft()){                                                  
                    left(p);
                    right(g);
                }
            }
        }
    }
    this->root_ = n;
}

//performs a left rotation
template<class Key, class Value>
void SplayTree<Key, Value>::left(Node<Key, Value>* n){                                                  
    Node<Key, Value>* x = n->getRight();                                                 
    Node<Key, Value>* b = x->getLeft();                                                  
    Node<Key, Value>* p = n->getParent();                                                
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
}

//performs a right rotation
template<class Key, class Value>
void SplayTree<Key, Value>::right(Node<Key, Value>* n){                                                                                    
    Node<Key, Value>* x = n->getLeft();                                                  
    Node<Key, Value>* b = x->getRight();                                                 
    Node<Key, Value>* p = n->getParent();                                                
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
}

#endif