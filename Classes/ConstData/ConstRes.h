//
//  ConstRes.hpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#ifndef __CONST_RES_H__
#define __CONST_RES_H__

#include <stdio.h>
#include "CrossApp.h"

class ConstRes
{
public:
    ConstRes();
    virtual ~ConstRes();
    
    // Common
    static const string& getResCommonWhiteBG();
    static const string& getResCommonGreyBG();
    static const string& getResCommonBlueBG();
    
    // Login
    static const std::string& getResLoginBG();
    static const string& getResLoginUserBG();
    static const string& getResLoginPwdBG();
};

#endif /* ConstData_hpp */
