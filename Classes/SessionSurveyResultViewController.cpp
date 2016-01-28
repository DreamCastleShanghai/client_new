//
//  SessionSurveyResultViewController.cpp
//  SapSession
//
//  Created by bbknightzm on 16/2/5.
//
//

#include "SessionSurveyResultViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "FDataManager.h"


SessionSurveyResultViewController::SessionSurveyResultViewController()
: m_correct(false)
, m_pLoading(NULL)
//, m_alertLabel(NULL)
, m_headerView(NULL)
, m_backBtn(NULL)
, m_backBtnBG(NULL)
, m_headerTitle(NULL)
, m_bodyScrollView(NULL)
//, m_bodyContext(NULL)
, m_submitBtn(NULL)
, m_submitBtnBG(NULL)
{
    
}

SessionSurveyResultViewController::~SessionSurveyResultViewController()
{
    /*
     CC_SAFE_DELETE(m_textView);
     CC_SAFE_DELETE(m_pLoading);
     CC_SAFE_DELETE(m_alertLabel);
     
     CC_SAFE_DELETE(m_headerView);
     CC_SAFE_DELETE(m_backBtn);
     CC_SAFE_DELETE(m_backBtnBG);
     CC_SAFE_DELETE(m_headerTitle);
     
     CC_SAFE_DELETE(m_bodyScrollView);
     CC_SAFE_DELETE(m_bodyContext);
     CC_SAFE_DELETE(m_submitBtn);
     CC_SAFE_DELETE(m_submitBtnBG);
     */
}

void SessionSurveyResultViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    int hight = 0;
    int upHight = (520);
    // header
    if (m_headerView == NULL)
        m_headerView = CAView::createWithFrame(DRect((0), (0), m_winSize.width, upHight));
    
    if (m_headerView)
    {
        if (isCorrect()) {
            m_headerView->setColor(SAP_DEFAULT_COLOR);
        } else {
            m_headerView->setColor(SAP_GRAY);
        }
//        m_headerView->setFrame(DRect((0), (0), m_winSize.width, (420)));
        this->getView()->addSubview(m_headerView);
        
        // header back btn
        m_backBtn = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
        if (m_backBtn)
        {
            m_backBtn->addTarget(this, CAControl_selector(SessionSurveyResultViewController::buttonCallBack), CAControlEventTouchUpInSide);
            m_backBtn->setTag(20);
            m_headerView->addSubview(m_backBtn);
            
            m_backBtnBG = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
            if (m_backBtnBG)
            {
                m_backBtnBG->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
                m_backBtnBG->setFrame(DRect((20), (20), (80), (80)));
                m_backBtn->setBackgroundViewForState(CAControlStateAll, m_backBtnBG);
            }
        }
        
        m_headerTitle = CALabel::createWithCenter(DRect(m_winSize.width / 2, (80), m_winSize.width, (50)));
        m_headerTitle->setTextAlignment(CATextAlignmentCenter);
        m_headerTitle->setColor(CAColor_white);
        m_headerTitle->setFontSize(SAP_TITLE_FONT_SIZE);
        m_headerTitle->setText("Your Points");
        m_headerTitle->setFontName(SAP_FONT_ARIAL);
        m_headerTitle->setTouchEnabled(false);
        m_headerView->addSubview(m_headerTitle);
        
        hight += (70) + (15) + (90);
        
        m_messageLabel = CALabel::createWithCenter(DRect(m_winSize.width / 2, hight, m_winSize.width, (50)));
        if (m_messageLabel) {
            m_messageLabel->setTextAlignment(CATextAlignmentCenter);
            m_messageLabel->setColor(CAColor_white);
            m_messageLabel->setFontSize(40);
            m_messageLabel->setFontName(SAP_FONT_ARIAL);
            m_messageLabel->setTouchEnabled(false);
            m_headerView->addSubview(m_messageLabel);
            if (isCorrect()) {
                m_messageLabel->setText(crossapp_format_string("+ %d", getAddedPoint()));
            } else {
                m_messageLabel->setText("Sorry, wrong answer. :(");
            }
        }
        
        hight += (10) + (30) + (100);
        
        m_pointLabel = CALabel::createWithCenter(DRect(m_winSize.width / 2, hight, m_winSize.width, (160)));
        if (m_pointLabel) {
            m_pointLabel->setTextAlignment(CATextAlignmentCenter);
            m_pointLabel->setColor(CAColor_white);
            m_pointLabel->setFontSize(150);
            m_pointLabel->setFontName(SAP_FONT_ARIAL);
            m_pointLabel->setTouchEnabled(false);
            m_headerView->addSubview(m_pointLabel);
            m_pointLabel->setText(crossapp_format_string("%d", getPoint()));
        }
        
        hight += (50) + (70);// + (50);
        
        m_rankLabel = CALabel::createWithCenter(DRect(m_winSize.width / 2, hight, m_winSize.width, (50)));
        if (m_rankLabel) {
            m_rankLabel->setTextAlignment(CATextAlignmentCenter);
            m_rankLabel->setColor(CAColor_white);
            m_rankLabel->setFontSize(40);
            m_rankLabel->setFontName(SAP_FONT_ARIAL);
            m_rankLabel->setTouchEnabled(false);
            m_headerView->addSubview(m_rankLabel);
            m_rankLabel->setText(crossapp_format_string("Ranking %d", getRank()));
        }
        
    }
    
    // body scroll view
    m_bodyScrollView = CAScrollView::createWithFrame(DRect((0), upHight, m_winSize.width, m_winSize.height - upHight));
    if (m_bodyScrollView) {
        //scrollView->setViewSize(DSize(m_winSize.width - (40), (m_imageHeight));
        m_bodyScrollView->setHorizontalScrollEnabled(false);
        m_bodyScrollView->setVerticalScrollEnabled(true);
        m_bodyScrollView->setBounceHorizontal(false);
        m_bodyScrollView->setBounds(DRect(0, 0, m_winSize.width, m_winSize.height - upHight - (120)));
        m_bodyScrollView->setAnchorPoint(DPoint(0.f, 0.f));
        m_bodyScrollView->setShowsHorizontalScrollIndicator(false);
        m_bodyScrollView->setShowsVerticalScrollIndicator(false);
        m_bodyScrollView->setBackgroundColor(CAColor_clear);
        this->getView()->addSubview(m_bodyScrollView);
        
        hight = (40);
        
        m_ruleChTitle = CALabel::createWithFrame(DRect((40), hight, m_winSize.width - (80), (30)));
        if (m_ruleChTitle) {
            m_ruleChTitle->setColor(CAColor_black);
            m_ruleChTitle->setFontSize((30));
            m_ruleChTitle->setBold(true);
            m_ruleChTitle->setText("Game Rule");
            m_ruleChTitle->setFontName(SAP_FONT_ARIAL);
            m_bodyScrollView->addSubview(m_ruleChTitle);
        }
        
        hight += (30) + (30);
        m_ruleChContent = CALabel::createWithFrame(DRect((40), hight, m_winSize.width - (80), (120)));
        if (m_ruleChContent) {
            m_ruleChContent->setColor(CAColor_black);
            m_ruleChContent->setFontSize((30));
            m_ruleChContent->setBold(true);
            m_ruleChContent->setText(gameRuleContent);
            m_ruleChContent->setFontName(SAP_FONT_ARIAL);
            m_bodyScrollView->addSubview(m_ruleChContent);
        }
        
        /*
        hight += (120) + (30);
        
        m_ruleChTitle = CALabel::createWithFrame(DRect((40), hight, m_winSize.width - (80), (30)));
        if (m_ruleChTitle) {
            m_ruleChTitle->setColor(CAColor_black);
            m_ruleChTitle->setFontSize((30));
            m_ruleChTitle->setBold(true);
            m_ruleChTitle->setText("Game Rule");
            m_ruleChTitle->setFontName(SAP_FONT_ARIAL);
            m_bodyScrollView->addSubview(m_ruleChTitle);
        }
        
        hight += (30);
        
        m_ruleChContent = CALabel::createWithFrame(DRect((40), hight, m_winSize.width - (80), (120)));
        if (m_ruleChContent) {
            m_ruleChContent->setColor(CAColor_black);
            m_ruleChContent->setFontSize((30));
            m_ruleChContent->setBold(true);
            m_ruleChContent->setText("Game Rule Content");
            m_ruleChContent->setFontName(SAP_FONT_ARIAL);
            m_bodyScrollView->addSubview(m_ruleChContent);
        }*/
        
    }
    /*
     m_submitBtn = CAButton::createWithFrame(DRect((40), m_winSize.height - (120), m_winSize.width - (80), (100)), CAButtonTypeCustom);
     if (m_submitBtn) {
         m_submitBtn->setTitleForState(CAControlStateAll, "I know.");
         m_submitBtn->setTitleFontSize((35));
         m_submitBtn->setTitleColorForState(CAControlStateAll, CAColor_white);
         m_submitBtn->addTarget(this, CAControl_selector(SessionSurveyResultViewController::buttonCallBack), CAControlEventTouchUpInSide);
         m_submitBtn->setTag(200);
         this->getView()->addSubview(m_submitBtn);
     
         m_submitBtnBG = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
         if (m_submitBtnBG) {
             m_submitBtnBG->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
             m_submitBtn->setBackgroundViewForState(CAControlStateAll, m_submitBtnBG);
         }
     }
     */
}

void SessionSurveyResultViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
/*
void SessionSurveyResultViewController::requestSubmit()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = firstSurveySubmitTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionSurveyResultViewController::onRequestSubmitFinished));
    
    if (!m_pLoading) {
        m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
    }
    if (m_pLoading) {
        m_pLoading->setLoadingMinTime(0.5f);
        this->getView()->addSubview(m_pLoading);
    }
}

void SessionSurveyResultViewController::onRequestSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        const CSJson::Value& value = json["result"];
        SessionSurveyResultViewController* srv = new SessionSurveyResultViewController();
        srv->setAddedPoint(value["add"].asInt());
        srv->setRank(value["rank"].asInt());
        srv->setPoint(value["points"].asInt());
        if (value["r"] == 1)
        {
//            CAAlertView *alertView = CAAlertView::createWithText("Succeed !", "Thanks for take this survey !", "OK", NULL);
//            alertView->show();
            FDataManager::getInstance()->setUserDirty(true);
//            userInfo* info = FDataManager::getInstance()->getUserInfo();
//            info->m_greenAmb = true;
            srv->setCorrect(true);
        }
        else
        {
//            CAAlertView *alertView = CAAlertView::createWithText("Sorry !", "You have taken this survey !", "OK", NULL);
//            alertView->show();
            srv->setCorrect(false);
        }
        srv->autorelease();
        back();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(srv, true);
    }
    else
    {
        CAAlertView *alertView = CAAlertView::createWithText("Network error !", "Please try again !", "OK", NULL);
        alertView->show();
    }
    
    if (m_pLoading)
    {
        m_pLoading->stopAnimating();
        this->getView()->removeSubview(m_pLoading);
    }
}
*/
void SessionSurveyResultViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        back();
    }
    /*
    if (btn->getTag() == 200)
    {
        back();
    }
     */
}

void SessionSurveyResultViewController::back()
{
    RootWindow::getInstance()->getRootNavigationController()->popToRootViewControllerAnimated(true);
    //RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(false);
}
