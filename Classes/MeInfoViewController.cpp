//
//  MeInfoViewController.cpp
//  SapSession
//
//  Created by bbknightzm on 16/1/30.
//
//

#include "MeInfoViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"

MeInfoViewController::MeInfoViewController()
: m_textView(NULL)
, m_pLoading(NULL)
, m_alertLabel(NULL)
, m_headerView(NULL)
, m_backBtn(NULL)
, m_backBtnBG(NULL)
, m_headerTitle(NULL)
, m_bodyScrollView(NULL)
//, m_bodyContext(NULL)
//, m_submitBtn(NULL)
//, m_submitBtnBG(NULL)
{
    requestSubmit();
}

MeInfoViewController::~MeInfoViewController()
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

void MeInfoViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    // header
    m_headerView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    if (m_headerView)
    {
        m_headerView->setFrame(DRect((0), (0), m_winSize.width, (120)));
        this->getView()->addSubview(m_headerView);
        
        // header back btn
        m_backBtn = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
        if (m_backBtn)
        {
            m_backBtn->addTarget(this, CAControl_selector(MeInfoViewController::buttonCallBack), CAControlEventTouchUpInSide);
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
        
        m_headerTitle = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (40)));
        m_headerTitle->setTextAlignment(CATextAlignmentCenter);
        m_headerTitle->setColor(CAColor_white);
        m_headerTitle->setFontSize(SAP_TITLE_FONT_SIZE);
        m_headerTitle->setText("Rules of Credits");
        m_headerTitle->setFontName(SAP_FONT_ARIAL);
        m_headerTitle->setTouchEnabled(false);
        m_headerView->addSubview(m_headerTitle);
    }
    
    /*
    // body scroll view
    m_bodyScrollView = CAScrollView::createWithFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (240)));
    if (m_bodyScrollView) {
        //scrollView->setViewSize(DSize(m_winSize.width - (40), (m_imageHeight));
        m_bodyScrollView->setHorizontalScrollEnabled(false);
        m_bodyScrollView->setVerticalScrollEnabled(true);
        m_bodyScrollView->setBounceHorizontal(false);
        m_bodyScrollView->setBounds(DRect(0, 0, m_winSize.width, m_winSize.height - (240)));
        m_bodyScrollView->setAnchorPoint(DPoint(0.f, 0.f));
        m_bodyScrollView->setShowsHorizontalScrollIndicator(false);
        m_bodyScrollView->setShowsVerticalScrollIndicator(false);
        m_bodyScrollView->setBackgroundColor(CAColor_clear);
        this->getView()->addSubview(m_bodyScrollView);
        
        m_bodyContext = CALabel::createWithFrame(DRect((40), (40), m_winSize.width - (80), m_winSize.height - (240)));
        if (m_bodyContext) {
            //m_bodyContext->setText();//surveyDetail[0]
            m_bodyContext->setVerticalScrollEnabled(true);
            m_bodyContext->setHorizontalScrollEnabled(true);
            m_bodyContext->setColor(CAColor_gray);
            m_bodyContext->setFontSize((30));
            m_bodyContext->setBold(true);
            //label->setItalics(true);
            m_bodyContext->setFontName(SAP_FONT_ARIAL);
            m_bodyScrollView->addSubview(m_bodyContext);
        }
    }
     */
    
    /*
    m_submitBtn = CAButton::createWithFrame(DRect((40), m_winSize.height - (120), m_winSize.width - (80), (100)), CAButtonTypeCustom);
    if (m_submitBtn) {
        m_submitBtn->setTitleForState(CAControlStateAll, "I know.");
        m_submitBtn->setTitleFontSize((35));
        m_submitBtn->setTitleColorForState(CAControlStateAll, CAColor_white);
        m_submitBtn->addTarget(this, CAControl_selector(MeInfoViewController::buttonCallBack), CAControlEventTouchUpInSide);
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

void MeInfoViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void MeInfoViewController::requestSubmit()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = meViewTag[0];
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MeInfoViewController::onRequestSubmitFinished));
    
    if (!m_pLoading) {
        m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
    }
    if (m_pLoading) {
        m_pLoading->setLoadingMinTime(0.5f);
        this->getView()->addSubview(m_pLoading);
    }
}

void MeInfoViewController::onRequestSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive maps == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        int isSucceed = value["r"].asInt();
        std::string tag = value["i"].asString();
        if (tag == meViewTag[0] && isSucceed == 1) {
            const CSJson::Value& map = value["me"];
            int length = map.size();
            int headHight = 120;
            if (m_bodyScrollView == NULL) {
                m_bodyScrollView = CAScrollView::createWithFrame(DRect((0), headHight, m_winSize.width, (m_winSize.height - headHight) * length));
            }
            if (m_bodyScrollView) {
                //scrollView->setViewSize(DSize(m_winSize.width - (40), (m_imageHeight));
                m_bodyScrollView->setHorizontalScrollEnabled(true);
                m_bodyScrollView->setVerticalScrollEnabled(true);
                //m_bodyScrollView->setBounceHorizontal(false);
                m_bodyScrollView->setBounces(false);
                m_bodyScrollView->setBounds(DRect(0, headHight, m_winSize.width, m_winSize.height - headHight));
                m_bodyScrollView->setAnchorPoint(DPoint(0.f, headHight));
                m_bodyScrollView->setShowsHorizontalScrollIndicator(true);
                m_bodyScrollView->setShowsVerticalScrollIndicator(true);
                m_bodyScrollView->setBackgroundColor(CAColor_clear);
                m_bodyScrollView->setMinimumZoomScale(1.0f);
                m_bodyScrollView->setMaximumZoomScale(4.0f);
                m_bodyScrollView->setDisplayRange(true);
                m_bodyScrollView->setMultitouchGesture(CAScrollView::Zoom);
                this->getView()->addSubview(m_bodyScrollView);
            }
            for (int i = 0; i < length; ++i) {
                newsPage mapRes;
                mapRes.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), map[i]["Resource"].asCString());
                //map[i]["Resource"].asString();
                mapRes.m_lable = map[i]["ResLable"].asString();
                
                CommonUrlImageView* content = CommonUrlImageView::createWithFrame(DRect(0, (m_winSize.height - headHight) * i, m_winSize.width, m_winSize.height - 120));
                if (content) {
                    content->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
                    content->setImage(CAImage::create("common/bg.png"));
                    content->setUrl(mapRes.m_imageUrl);
                }
                if (m_bodyScrollView) {
                    m_bodyScrollView->addSubview(content);
                }
                //m_maps.push_back(mapRes);
            }
        }
        /*
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if (value["r"] == 1)
        {
            CAAlertView *alertView = CAAlertView::createWithText("Succeed !", "Thanks for take this survey !", "OK", NULL);
            alertView->show();
            
            userInfo* info = FDataManager::getInstance()->getUserInfo();
            info->m_greenAmb = true;
        }
        else
        {
            CAAlertView *alertView = CAAlertView::createWithText("Sorry !", "You have taken this survey !", "OK", NULL);
            alertView->show();
        }
        back();
         */
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

void MeInfoViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        back();
    }
    if (btn->getTag() == 200)
    {
        requestSubmit();
    }
}

void MeInfoViewController::back()
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}
