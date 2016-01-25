//
//  ConstData.hpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#ifndef __ConstData_h__
#define __ConstData_h__

#include <stdio.h>
#include "CrossApp.h"

class ConstData
{
public:
    ConstData();
    virtual ~ConstData();
    
    static DRect& getSessionCellLogoRect();
    static DRect& getSessionCellTitleRect();
    static DRect& getSessionCellTimeRect();
    static DRect& getSessionCellLocationRect();
    static DRect& getSessionCellLikeRect();
    static DRect& getSessionCellLikeLableRect();
    static DRect& getSessionCellCollectionRect();
    static DRect& getSessionCellDetailArrowRect();
};

#endif /* ConstData_hpp */
