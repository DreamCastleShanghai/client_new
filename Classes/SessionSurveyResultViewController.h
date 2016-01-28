//
//  SessionSurveyResultViewController.h
//  SapSession
//
//  Created by bbknightzm on 16/2/5.
//
//

#ifndef SessionSurveyResultViewController_h
#define SessionSurveyResultViewController_h

#include <stdio.h>
#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SessionSurveyResultViewController :
public CAViewController
{
    
public:
    
    SessionSurveyResultViewController();
    
    virtual ~SessionSurveyResultViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void buttonCallBack(CAControl* btn, DPoint point);
    
//    void requestSubmit();
    
    void back();
    
//    void onRequestSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    CC_SYNTHESIZE_IS(bool, m_correct, Correct)
    CC_SYNTHESIZE(int, m_rank, Rank)
    CC_SYNTHESIZE(int, m_point, Point)
    CC_SYNTHESIZE(int, m_addedPoint, AddedPoint)
    
private:
    
//    bool                        m_correct;
    DSize                       m_winSize;
    
    CAActivityIndicatorView*	m_pLoading;
    
    // body
    CAView*                     m_headerView;
    CAButton*                   m_backBtn;
    CAImageView*                m_backBtnBG;
    CALabel*                    m_headerTitle;
    
    CALabel*                    m_messageLabel;
    CALabel*                    m_pointLabel;
    CALabel*                    m_rankLabel;
    CALabel*                    m_ruleChTitle;
    CALabel*                    m_ruleChContent;
    CALabel*                    m_ruleEnTitle;
    CALabel*                    m_ruleEnContent;
    CAScrollView*               m_bodyScrollView;
//    CALabel*                    m_bodyContext;
    CAButton*                   m_submitBtn;
    CAImageView*                m_submitBtnBG;
};

#endif /* SessionSurveyResultViewController_h */
