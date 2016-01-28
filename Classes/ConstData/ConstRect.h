//
//  ConstRect.hpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#ifndef __CONST_RECT_H__
#define __CONST_RECT_H__

#include <stdio.h>
#include "CrossApp.h"

class ConstRect
{
public:
    ConstRect();
    virtual ~ConstRect();
        
    // Session Table Cell
    static DRect& getSessionCellLogoRect();
    static DRect& getSessionCellTitleRect();
    static DRect& getSessionCellTimeRect();
    static DRect& getSessionCellLocationRect();
    static DRect& getSessionCellLikeRect();
    static DRect& getSessionCellLikeLableRect();
    static DRect& getSessionCellCollectionRect();
    static DRect& getSessionCellDetailArrowRect();
    
    // Session Detail
    static DRect& getSessionDetailDescriptionRect();
    static DRect& getSessionDetailSpeakerLogoRect();
    static DRect& getSessionDetailSpeakerNameRect();
};

#endif /* ConstData_hpp */
