//
//  ConstId.cpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#include "ConstId.h"

enum MYID_VALUE {
    BACK_ID = 20,
};

ConstId::ConstId()
{
    
}

ConstId::~ConstId()
{
    
}

int ConstId::getBackBtnId()
{
    return BACK_ID;
}

