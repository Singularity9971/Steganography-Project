//
//  Compressor.cpp
//  CS 32 Project 4
//
//  Created by Avirudh Theraja on 3/8/15.
//  Copyright (c) 2015 Avirudh Theraja. All rights reserved.
//

#include "provided.h"
#include <string>
#include "HashTable.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

unsigned int computeHash(string s)  //dbj2 algorithm to hash strings, copied from http://www.cse.yorku.ca/~oz/hash.html
{
    unsigned int hashValue = 5381;
    
    int c;
    int i = 0;
    
    while ((c = s[i]))
    {
        hashValue = ((hashValue << 5) + hashValue) + c;
        i++;
        
    }
    
    return hashValue;
}


unsigned int computeHash(unsigned short x)
{
    return x;
}


void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    int capacity = fmin((s.size()/2) + 512, 16384);   //set capacity equal to lesser of the two
    
    HashTable<string, unsigned short> H(2*capacity,capacity);   //load factor = 0.5
    string x;
    
    for(int i = 0; i != 256; i++)       //rest of the algorithm is completely copied from the spec
    {
        x = static_cast<char>(i);
        H.set(x, i,true);
    }
    
    int nextFreeID = 256;
    
    string runSoFar;
    
    numbers.clear();
    
    for(unsigned short i = 0; i != s.size(); i++)
    {
        string expandedRun = runSoFar+s[i];
        unsigned short temp = i;
        if(H.get(expandedRun,temp))
        {
            runSoFar = expandedRun;
        }
        else
        {
            unsigned short x;
            H.get(runSoFar, x);
            numbers.push_back(x);
            H.touch(runSoFar);
            runSoFar = "";
            
            unsigned short cv;
            string check;
            check+=s[i];
            H.get(check, cv);
            numbers.push_back(cv);
            
            if(!H.isFull())
            {
                H.set(expandedRun, nextFreeID);
                nextFreeID++;
            }
            else
            {
                unsigned short temp; string temp1;
                H.discard(temp1, temp);
                H.set(expandedRun, temp);
            }
        }
        
    }
    
    if(runSoFar != "")
    {
        unsigned short x;
        H.get(runSoFar, x);
        numbers.push_back(x);
    }
    
    numbers.push_back(capacity);
    
        
    
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{

    int capacity = numbers[numbers.size() - 1];      //algorithm completely copied from spec
    HashTable<unsigned short, string> H(2*capacity,capacity);
    
    string x;
    for(int i = 0; i != 256; i++)
    {
        x = static_cast<char>(i);
        H.set(i, x,true);
    }
    
    int nextFreeID = 256;
    
    string runSoFar,output;
    
    for(int i = 0; i != numbers.size()-1; i++)
    {
        unsigned short us = numbers[i];
        
        if(us <= 255)
        {
            string temp;
            H.get(us,temp);
            output+=temp;
            
            if(runSoFar == "")
            {
                runSoFar += temp;
            }
            else
            {
                string expandedRun = runSoFar + temp;
                if(!H.isFull())
                {
                    H.set(nextFreeID, expandedRun);
                    nextFreeID++;
                }
                else
                {
                    unsigned short x; string y;
                    H.discard(x, y);
                    H.set(x, expandedRun);
                    
                }
                
                runSoFar = "";

            }
            
        }
        else
        {
            string temp;
            if(!H.get(us, temp))
            {
                return false;
            }
            else
            {
                H.touch(us);
                output += temp;
                runSoFar = temp;
            }
                
        }
    }
    
    s = output;
    return true;
    
}
























