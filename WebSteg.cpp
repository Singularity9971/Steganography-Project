//
//  WebSteg.cpp
//  CS 32 Project 4
//
//  Created by Avirudh Theraja on 3/8/15.
//  Copyright (c) 2015 Avirudh Theraja. All rights reserved.
//

#include "http.h"
#include "provided.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    string page;
    if(HTTP().get(url, page))
       if(Steg::hide(page, msg, host))      //if retreival and hiding were successful, return true
           return true;
    
    return false;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string page;
    if(HTTP().get(url, page))
        if(Steg::reveal(page, msg))   //if retreival and reveal were successful, return true
            return true;
    
    return false;
            
}
