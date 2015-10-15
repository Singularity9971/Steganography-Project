//
//  Steg.cpp
//  CS 32 Project 4
//
//  Created by Avirudh Theraja on 3/8/15.
//  Copyright (c) 2015 Avirudh Theraja. All rights reserved.
//

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut)
{
    if(hostIn == "")        //if hostIn is empty, then return false
        return false;
    else
    {
        hostOut = "";       //set hostOut to empty string just in case a non empty string was passed in
        
        vector<unsigned short> numbers;
        Compressor::compress(msg, numbers);            //pass a vector along with msg to compress
        string compressed = BinaryConverter::encode(numbers);   //get the compressed string
        unsigned long sizeOfCompressed = compressed.size();     //get the size of the compressed string
        
        int subsProcessed = 0;              //to keep track of how many substrings have been processed
        unsigned long numberOflines = 0;
        vector<int> posOfLine;              //vector to hold position of newlines in the string
        string temp;
        
        for(int i = 0; i != hostIn.size(); i++) //loop which records all positions of newlines
        {
            if(hostIn[i] == '\n')
            {
                numberOflines++;
                posOfLine.push_back(i);
            }
            else if(hostIn[i] == '\r')
            {
                if(i != hostIn.size() -1 && hostIn[i+1] == '\n')
                {
                    numberOflines++;
                    posOfLine.push_back(i);
                    i++;
                }
            }
            else if(i == hostIn.size() - 1 && hostIn[i] != '\n')
            {
                numberOflines++;
                i++;
                posOfLine.push_back(i);
            }
        }
        for(int i = 0; i != hostIn.size()+1;i++)//if we don't hit a newline character, keep adding to a temp string
        {
            if(subsProcessed == numberOflines)
                break;
            
            if(i != posOfLine[subsProcessed])
                temp+=hostIn[i];
            else
            {
                bool isR = false;           //bool to keep track of '\r' character
                if(hostIn[i] == '\r')
                {
                    i++;    //if newline started with '\r', increment i because next character will be '\n'
                    isR = true;
                }
                string strip;
                int j;
                for(j = static_cast<int>(temp.size() - 1); j >= 0 ; j--)
                    if(temp[j] != '\t' && temp[j] != ' ')
                        break;
                
                for(int k = 0; k <= j; k++)  //now strip contains no trailing whitespace
                    strip +=  temp[k];
                
                unsigned long sizeofSubstr;
                
                if(subsProcessed < (sizeOfCompressed % numberOflines))
                    sizeofSubstr = (sizeOfCompressed/numberOflines) + 1;
                else
                    sizeofSubstr = sizeOfCompressed/numberOflines;
                                                        //size of substring has been set
                
                string tempMsg = compressed.substr(0,sizeofSubstr);     //get string to be added using size
                
                
                
                if(compressed.size() > sizeofSubstr)
                    compressed = compressed.substr(sizeofSubstr);
                else
                    compressed = "";        //set compressed to empty if all strings have been processed
                                
                if(isR)
                    hostOut += strip + tempMsg + '\r'+'\n'; //add '\r' if we had a newline character with r
                else
                    hostOut += strip + tempMsg + '\n';
                
                subsProcessed++;    //increase number of substrings processed
                temp = "";          //reset temp to empty string
                        
                    
            }
        }
        
     
        
    }
    
    return true;
}

bool Steg::reveal(const string& hostIn, string& msg)
{
    string convert;         //create empty string
    for(int i = 0; i != hostIn.size(); i++)
    {
        //if we hit a newline character, add the trailing whitespaces to our convert string
        if(hostIn[i] == '\n' || (hostIn[i] == '\r' &&  i != hostIn.size() - 1 && hostIn[i+1] == '\n'))
        {
            string tempMsg;
            int j;
            for(j = i-1;j >= 0;j--)
                if(hostIn[j] != '\t' && hostIn[j] != ' ')
                {
                    j++;
                    break;
                }
            
            for(; j != i; j++)
            {
                tempMsg += hostIn[j];
            }
            
            convert += tempMsg;
            if(hostIn[i] == '\r')   //increment i as next character will be '\n'
                i++;
        }
    }
    
    
    vector<unsigned short> numbers;
    if(BinaryConverter::decode(convert, numbers))
        if(Compressor::decompress(numbers, msg))      //if decode and decompress was successful, return true
            return true;
    
    return false;
}
















