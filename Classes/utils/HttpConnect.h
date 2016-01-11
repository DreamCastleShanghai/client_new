//
//  HttpConnect.hpp
//  SapSession
//
//  Created by csh on 15/12/10.
//
//

#ifndef __HttpConnect__
#define __HttpConnect__

#include <iostream>
#include <string>
#include <map>

#include "CrossApp.h"

USING_NS_CC;
using namespace std;

string getSign(std::map<std::string,std::string> key_value);
string getSign(std::string value);
CAImage* createQRImage(std::string, int width, int height);

#endif /* HttpConnect_hpp */
