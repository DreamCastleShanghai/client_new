//
//  ConstRect.cpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#include "ConstRect.h"
#include "CDData.h"

DRect gRect[] = {
    DRect((30), (120), (80), (80)),     // session table cell : logo size 80 x 80
    DRect((30), (10), 0, (100)),           // session table cell : title
    DRect((140), (120), 0, (30)),          // session table cell : time
    DRect((140), (165), 0, (30)),          // session table cell : location
    DRect(0, (165), (100), (50)),          // session table cell : like btn
    DRect(0, (173), (50), (30)),           // session table cell : like lable
    DRect(0, (165), (50), (50)),           // session table cell : collection btn
    DRect(0, (100), (50), (50)),           // session table cell : detal arrow

    DRect((40), (470), 0, (1000)),          // session detail : session description
    DRect(0, (870), (80), (80)),           // session detail : speaker logo
    DRect(0, (960), (200), (30)),          // session detail : speaker name lable
};

enum CDRectId {
    CDR_SESSION_TABLE_CELL_LOGO = 0,
    CDR_SESSION_TABLE_CELL_TITLE,
    CDR_SESSION_TABLE_CELL_TIME,
    CDR_SESSION_TABLE_CELL_LOCATION,
    CDR_SESSION_TABLE_CELL_LIKE,
    CDR_SESSION_TABLE_CELL_LIKE_LABLE,
    CDR_SESSION_TABLE_CELL_COLLECTION,
    CDR_SESSION_TABLE_CELL_DETAIL_ARROW,
    
    CDR_SESSION_DETAIL_DESCRIPTION,
    CDR_SESSION_DETAIL_SPEAKER_LOGO,
    CDR_SESSION_DETAIL_SPEAKER_NAME,
};

ConstRect::ConstRect()
{
    
}

ConstRect::~ConstRect()
{
    
}

// Session Table Cell
DRect& ConstRect::getSessionCellLogoRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_LOGO];
}

DRect& ConstRect::getSessionCellTitleRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_TITLE];
}

DRect& ConstRect::getSessionCellTimeRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_TIME];
}

DRect& ConstRect::getSessionCellLocationRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_LOCATION];
}

DRect& ConstRect::getSessionCellLikeRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_LIKE];
}

DRect& ConstRect::getSessionCellLikeLableRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_LIKE_LABLE];
}

DRect& ConstRect::getSessionCellCollectionRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_COLLECTION];
}

DRect& ConstRect::getSessionCellDetailArrowRect()
{
    return gRect[CDR_SESSION_TABLE_CELL_DETAIL_ARROW];
}

// Session Detail
DRect& ConstRect::getSessionDetailDescriptionRect()
{
    return gRect[CDR_SESSION_DETAIL_DESCRIPTION];
}

DRect& ConstRect::getSessionDetailSpeakerLogoRect()
{
    return gRect[CDR_SESSION_DETAIL_SPEAKER_LOGO];
}

DRect& ConstRect::getSessionDetailSpeakerNameRect()
{
    return gRect[CDR_SESSION_DETAIL_SPEAKER_NAME];
    
}


