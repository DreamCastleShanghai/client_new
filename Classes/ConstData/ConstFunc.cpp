//
//  ConstFunc.cpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#include "ConstFunc.h"

ConstFunc::ConstFunc()
{
    
}

ConstFunc::~ConstFunc()
{
    
}

void ConstFunc::trim(string& str)
{
    string::size_type pos = str.find_last_not_of(' ');
    if(pos != string::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != string::npos) str.erase(0, pos);
    }
    else
        str.erase(str.begin(), str.end());
}
