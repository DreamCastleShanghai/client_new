
#include "FirstSurveyViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"

FirstSurveyViewController::FirstSurveyViewController()
: m_pLoading(NULL)
{

}

FirstSurveyViewController::~FirstSurveyViewController()
{

}

void FirstSurveyViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;

    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(120)));
    this->getView()->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(FirstSurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Survey");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);
    
    CAScrollView* scrollView = CAScrollView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(240)));
    //scrollView->setViewSize(DSize(m_winSize.width - _px(40), (m_imageHeight));
    scrollView->setHorizontalScrollEnabled(false);
    scrollView->setVerticalScrollEnabled(true);
    scrollView->setBounceHorizontal(false);
    scrollView->setBounds(DRect(0, 0, m_winSize.width, m_winSize.height - _px(240)));
    scrollView->setAnchorPoint(DPoint(0.f, 0.f));
    scrollView->setShowsHorizontalScrollIndicator(false);
    scrollView->setShowsVerticalScrollIndicator(false);
    scrollView->setBackGroundColor(CAColor_clear);
    this->getView()->addSubview(scrollView);
    
    label = CALabel::createWithFrame(DRect(_px(40), _px(40), m_winSize.width - _px(80), m_winSize.height - _px(240)));
    label->setText(surveyDetail[0]);
    label->setVerticalScrollEnabled(true);
    label->setHorizontalScrollEnabled(true);
    label->setColor(CAColor_gray);
    label->setFontSize(_px(30));
    label->setBold(true);
    //label->setItalics(true);
    label->setFontName("fonts/arial.ttf");
    scrollView->addSubview(label);
    /*
    m_textView = CATextView::createWithFrame(DRect(_px(40), _px(550), m_winSize.width - _px(80), _px(300)));
    m_textView->setFontSize(_px(35));
    m_textView->setFontName("fonts/arial.ttf");
    m_textView->setColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
    m_textView->setBackGroundColor(CAColor_gray);
    this->getView()->addSubview(m_textView);
    
    m_alertLabel = CALabel::createWithFrame(DRect(_px(40), _px(860), m_winSize.width - _px(80), _px(40)));
    m_alertLabel->setColor(CAColor_red);
    m_alertLabel->setFontSize(_px(35));
    m_alertLabel->setFontName("fonts/arial.ttf");
    //m_alertLabel->setVisible(false);
    this->getView()->addSubview(m_alertLabel);
    */
    
    button = CAButton::createWithFrame(DRect(_px(40), m_winSize.height - _px(120), m_winSize.width - _px(80), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->setTitleForState(CAControlStateAll, "Submit");
    button->setTitleFontSize(_px(35));
    button->setTitleColorForState(CAControlStateAll, CAColor_white);
    button->addTarget(this, CAControl_selector(FirstSurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(200);
    this->getView()->addSubview(button);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void FirstSurveyViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void FirstSurveyViewController::requestSubmit()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = firstSurveySubmitTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(FirstSurveyViewController::onRequestSubmitFinished));
//    
//	std::map<std::string, std::string> key_value;
//	key_value["tag"] = firstSurveySubmitTag[0];
//    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
//	//key_value["sign"] = getSign(key_value);
//	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(FirstSurveyViewController::onRequestSubmitFinished));
    
    m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
    m_pLoading->setLoadingMinTime(0.5f);
    this->getView()->addSubview(m_pLoading);
}

void FirstSurveyViewController::onRequestSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
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
    }
    else
    {
        CAAlertView *alertView = CAAlertView::createWithText("Network error !", "Please try again !", "OK", NULL);
        alertView->show();
    }
    
    {
        
    }
    
    if (m_pLoading)
    {
        m_pLoading->stopAnimating();
        this->getView()->removeSubview(m_pLoading);
        m_pLoading = NULL;
    }
}

void FirstSurveyViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
	if (btn->getTag() == 200)
	{
        /*
        if (m_textView->getText() == "") {
            CAAlertView *alertView = CAAlertView::createWithText("Waining !", "Submit cannot be null !", "OK", NULL);
            alertView->show();
            return;
        }
         */
        requestSubmit();
	}
}


