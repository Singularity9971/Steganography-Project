//
//  BinaryConverter.cpp
//  CS 32 Project 4
//
//  Created by Avirudh Theraja on 3/8/15.
//  Copyright (c) 2015 Avirudh Theraja. All rights reserved.
//

#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    string message;
    
    
    for(int i = 0; i != numbers.size();i++)   //convert numbers to a string of binary digits
    {
        message+=convertNumberToBitString(numbers[i]);
    }
        
    for(int i = 0; i != message.size();i++)   //replace 1's and 0's with spaces and tabs
    {
        if(message[i] == '0')
            message[i] = ' ';
        else
            message[i] = '\t';
    }
    
    
    return message;                     //return encoded string
}

bool BinaryConverter::decode(const string& bitString,
                             vector<unsigned short>& numbers)
{
    if(bitString.size() % 16 != 0 || bitString == "")  //if bitString is empty or not a multiple of 16, quit
        return false;
    
    numbers.clear();            //function to empty a vector
    
    string binary;
    
    for(int i = 0; i != bitString.size();i++)
    {
        if(bitString[i] == ' ')     //convert spaces and tabs to 0's and 1's, add them to binary string
            binary += "0";
        
        else if(bitString[i] == '\t')
            binary += "1";
        
        else                       //if string has any other character, return false
            return false;
    }
    
    unsigned short x;
    
    for(int i = 0; i != binary.size(); i += 16) //connvert every 16 length sequence to a number
    {
        convertBitStringToNumber(binary.substr(i,16), x);
        numbers.push_back(x);                   //add to the vector
    }
    
    
    return true;
}

string convertNumberToBitString(unsigned short number)
{
    string result(BITS_PER_UNSIGNED_SHORT, '0');
    for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
    {
        k--;
        if (number % 2 == 1)
            result[k] = '1';
    }
    return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
    if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
        return false;
    number = 0;
    for (size_t k = 0; k < bitString.size(); k++)
    {
        number *= 2;
        if (bitString[k] == '1')
            number++;
        else if (bitString[k] != '0')
            return false;
    }
    return true;
}

