//
//  PhotoTagViewController.h
//  SapSession
//
//  Created by bbknightzm on 16/2/4.
//
//

#ifndef PhotoTagViewController_hpp
#define PhotoTagViewController_hpp

#include <stdio.h>
#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class PhotoTagViewController :
public CAViewController
{
    
public:
    
    PhotoTagViewController();
    
    virtual ~PhotoTagViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void buttonCallBack(CAControl* btn, DPoint point);
    
    void requestSubmitPictureMsg();
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void setSubmitImage(std::string sourcefile) { m_file = sourcefile; }
private:
    
    enum
    {
        ID_CANCEL = 0,
        ID_FINISH,
        ID_TAG,
    };
    DSize                           m_winSize;
    
    CAImageView*                    m_submitImage;
    CAActivityIndicatorView*		p_pLoading;
    std::string                     m_file;
    int                             m_tagId;
};

#endif /* PhotoTagViewController_hpp */
