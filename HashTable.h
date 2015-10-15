//
//  HashTable.h
//  CS 32 Project 4
//
//  Created by Avirudh Theraja on 3/6/15.
//  Copyright (c) 2015 Avirudh Theraja. All rights reserved.
//

#include <string>
#include <iostream>
using namespace std;

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_


template<typename KeyType, typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity)
    {
        size = capacity;                //store numBuckets and capacity into member variables
        buckets = numBuckets;
        
        startHash = new Node*[buckets]; //startHash is Node**, pointer to pointer to Node
        
        for(int i = 0; i != buckets; i++) //initialize all the buckets to nullptr
            *(startHash+i) = nullptr;
                
        indexOfOldest = -1;             //set indices to arbitrary negative value, these represent buckets
        indexOfRecent = -1;
        
        latest = nullptr;               //set latest to nullptr, latest points to newest non perm node
        oldest = nullptr;               //set oldest to nullptr, oldest points to oldest non perm node
        
        count = 0;                      //number of items to 0
    }
    
    ~HashTable()
    {
        for(Node** a = startHash; a != startHash + buckets; a++) //delete all nodes in non empty linked lists, then delete array of nodes
        {
            Node* b = *a;
            while(b != nullptr)
            {
                Node* temp = b;
                b = b->next;
                delete temp;
            }
        }
        delete [] startHash;
    }
    
    bool isFull() const     //if number of items equals capacity
    {
        return count==size;
    }

    
    bool set(const KeyType& key, const ValueType& value, bool permanent = false)
    {
        
        Node* check = foundNode(key);       //function checks if there is a node with keytype key
        if(check != nullptr)
        {
            check->val = value;         //change value, update index and node if node is not permanent
            if(updateNode(check))
                indexOfRecent = getIndex(key);
            
            return true;
        }
        

        
     if(isFull())            //return false if our hash table is full
            return false;
        
     int index = getIndex(key);
    
     Node* temp = lastItemInList(key);      //finds last item in list corresponding to bucket of key
        
     Node* creator = new Node(key, value, permanent); //else, create a new node
     
     if(temp == nullptr)        //if temp is nullptr, then new item is the first one in this list
     startHash[index] = creator;
     else
     {
        temp->next = creator;       //else modify existing pointers of nodes
        creator->prev = temp;
     }
     
     if(!creator->permanent)    //if new node is not permanent, update our other linked list
     {
        if(indexOfOldest == -1)
        {
            indexOfOldest = index;
            oldest = creator;
        }
        else
        {
            latest->nextAdded = creator;
            creator->prevAdded = latest;
        }
         indexOfRecent = index;
         latest = creator;
     }
     
     count++;           //increment count and return true
     return true;
     
     
    }
     
    
    bool get(const KeyType& key, ValueType& value) const
    {
        Node* check = foundNode(key);   //if node is found, modify value and return true
        if(check != nullptr)
        {
            value = check->val;
            return true;
        }
        else
            return false;

    }
    
    bool touch(const KeyType& key)      //if node is found, update index and and node
    {
        Node* check = foundNode(key);
        if(check != nullptr)
            if(updateNode(check))
               {
                   indexOfRecent = getIndex(key);
                   return true;
               }
        
        return false;
    }
    
    
    bool discard(KeyType& key, ValueType& value)       //discards oldest non permanent node in the hashtable
    {
        if(oldest == nullptr)
            return false;
        
        if(oldest->nextAdded != nullptr)            //set oldest's next added's previous added to null
            oldest->nextAdded->prevAdded = nullptr;
        
        if(oldest->next != nullptr && oldest->prev != nullptr)  //modify other linked list
        {
            oldest->prev->next = oldest->next;
            oldest->next->prev = oldest->prev;
        }
        else if(oldest->next != nullptr)    //if oldest is first item in its list
        {
            oldest->next->prev = nullptr;
            *(startHash+indexOfOldest) = oldest->next;
        }
        else if(oldest->prev != nullptr)    //if oldest is last item in its list
            oldest->prev->next = nullptr;
        else
        {
            *(startHash+indexOfOldest) = nullptr;   //if oldest is the only item in its list
        }
        
        key = oldest->key;
        value = oldest->val;
        
        
        Node* temp = oldest;
        
        oldest = oldest->nextAdded; //reassign oldest
        
        delete temp;
        temp = nullptr;
        count--;
        
        if(oldest == nullptr)   //basically, hashtable is now empty
        {
            indexOfOldest = indexOfRecent = -1;
            latest = nullptr;
        }
        else                    //update index of oldest which now gives bucket of oldest item
        {
            indexOfOldest = getIndex(oldest->key);
        }
        
        return true;
        
    }
    
private:
    
    struct Node
    {
        Node(KeyType num, ValueType nm, bool perm)
        {
            val = nm;
            key = num;
            next = nullptr;
            prev = nullptr;
            nextAdded = nullptr;
            prevAdded = nullptr;
            permanent = perm;
        }
        ValueType val;
        KeyType key;
        Node* next;
        Node* prev;
        Node* nextAdded;
        Node* prevAdded;
        bool permanent;
    };
    
    Node* foundNode(KeyType key) const  //return pointer to node with key, else return nullptr
    {
        int index = getIndex(key);  //get the bucket
        
        Node* traverse = *(startHash + index);
        
        while(traverse != nullptr)
        {
            if(traverse->key == key)
                break;
            
            traverse = traverse->next;
        }
        
        return traverse;
    }
    
    bool updateNode(Node* traverse)
    {
        if(traverse->permanent)     //return false, node can't be updated
            return false;
        
        if(traverse == oldest)  //reassign oldest if needed
        {
            if(traverse->nextAdded != nullptr)
            {
                oldest = traverse->nextAdded;
                indexOfOldest = getIndex(traverse->nextAdded->key);
                
            }
            
        }
        
        
        if(traverse->nextAdded != nullptr && traverse->prevAdded != nullptr)    //modify the linked list, change the latest pointer
        {
            //modify all the pointers
            traverse->prevAdded->nextAdded = traverse->nextAdded;
            traverse->nextAdded->prevAdded = traverse->prevAdded;
            traverse->nextAdded = nullptr;
            traverse->prevAdded = latest;
            latest->nextAdded = traverse;
            latest = traverse;
        }
        else if(traverse->nextAdded != nullptr && traverse->prevAdded == nullptr)
        {
            traverse->nextAdded->prevAdded = nullptr;
            traverse->nextAdded = nullptr;
            latest->nextAdded = traverse;
            latest = traverse;
        }

        return true;
    }
    
    int getIndex(KeyType key) const     //return corresponding bucket for given key
    {
        unsigned int computeHash(KeyType);
        return computeHash(key) % buckets;
    }
    
    Node* lastItemInList(KeyType key) const
    {
        int index = getIndex(key);
        Node* traverse = startHash[index];
        
        if(traverse == nullptr)
            return traverse;
        
        while(traverse->next != nullptr)
        {
            traverse = traverse->next;
        }
        
        return traverse;        //return null or the last node in the linked list
    }
    
    
    int indexOfOldest;
    
    int indexOfRecent;
    
    int size;
    
    int count;
    
    int buckets;
    
    Node* latest;
    
    Node* oldest;
    
    Node** startHash;
    
    HashTable(const HashTable&);
    
    HashTable& operator=(const HashTable&);
 
};


















#endif