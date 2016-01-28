//
//  ConstFunc.hpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#ifndef __CONST_FUNC_H_
#define __CONST_FUNC_H_

#include <stdio.h>
#include "CrossApp.h"

class ConstFunc
{
public:
    ConstFunc();
    virtual ~ConstFunc();
    
    static void trim(string& str);
};

#endif /* ConstFunc_hpp */
