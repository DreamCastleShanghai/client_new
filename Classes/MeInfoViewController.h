//
//  MeInfoViewController.hpp
//  SapSession
//
//  Created by bbknightzm on 16/1/30.
//
//

#ifndef MeInfoViewController_hpp
#define MeInfoViewController_hpp

#include <stdio.h>
#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class MeInfoViewController :
	public CAViewController
{
    
public:
    
    MeInfoViewController();
    
    virtual ~MeInfoViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void buttonCallBack(CAControl* btn, DPoint point);
    
    void requestSubmit();
    
    void back();
    
    void onRequestSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    
private:
    
    DSize                       m_winSize;
    
    CATextView*                 m_textView;
    CAActivityIndicatorView*	m_pLoading;
    CALabel*                    m_alertLabel;
    
    // header
    CAScale9ImageView*          m_headerView;
    CAButton*                   m_backBtn;
    CAImageView*                m_backBtnBG;
    CALabel*                    m_headerTitle;
    
    // body
    CAScrollView*               m_bodyScrollView;
//    CALabel*                    m_bodyContext;
//    CAButton*                   m_submitBtn;
//    CAImageView*                m_submitBtnBG;
};

#endif /* MeInfoViewController_hpp */
