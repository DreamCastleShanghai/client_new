
#include "VoteShakeViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SimpleAudioEngine.h"

VoteShakeViewController::VoteShakeViewController(demoJamMsg* dMsg, voiceMsg* vMsg)
: p_alertView(NULL)
, p_pLoading(NULL)
, m_shakeNumLabel(NULL)
, m_demoMsg(dMsg)
, m_voiceMsg(vMsg)
, m_projectType(0)
, m_shakeNum(Vote_NotStart)
, m_voted(false)
{
    if (vMsg)
    {
        m_projectType = 1;
    }
}

VoteShakeViewController::~VoteShakeViewController()
{

}

void VoteShakeViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(VoteShakeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Vote");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

	m_view = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
	this->getView()->addSubview(m_view);
    
    CommonUrlImageView* urlImageVIew = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
    urlImageVIew->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
    urlImageVIew->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    urlImageVIew->setColor(ccc4(128, 128, 128, 255));
    urlImageVIew->setTouchEnabled(false);
    m_view->addSubview(urlImageVIew);
    if(m_demoMsg)
    {
        urlImageVIew->setUrl(m_demoMsg->m_imageUrl);
    }
    else if(m_voiceMsg)
    {
        urlImageVIew->setUrl(m_voiceMsg->m_imageUrl);
    }

    m_shakeView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
    m_shakeView->setColor(CAColor_clear);
    m_view->addSubview(m_shakeView);
    //m_shakeView->setVisible(false);
    
    m_shakeEndView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
    m_shakeEndView->setColor(CAColor_clear);
    m_view->addSubview(m_shakeEndView);
    m_shakeEndView->setVisible(false);
    
    requestMsg(0);
    
    CAApplication::getApplication()->getAccelerometer()->setDelegate(this);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void VoteShakeViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void VoteShakeViewController::initView()
{
    if (m_voteStatus == Vote_NotStart)
    {
        CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(300), m_winSize.width - _px(80), _px(80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_white);
        label->setFontSize(_px(35));
        label->setText("Vote event will be start after the show!");
        label->setFontName("fonts/arial.ttf");
        m_shakeView->addSubview(label);
    }
    else if (m_voteStatus == Vote_Start)
    {
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("vote/icon_shake.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        imageView->setFrame(DRect((m_winSize.width - _px(260)) / 2, _px(150), _px(260), _px(260)));
        m_shakeView->addSubview(imageView);
        
        CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(450), m_winSize.width - _px(80), _px(80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_red);
        label->setFontSize(_px(35));
        if (m_demoMsg)
        {
            label->setText(crossapp_format_string("Shake your phone, Vote for %s", m_demoMsg->m_teamName.c_str()));
        }
        else if(m_voiceMsg)
        {
            label->setText(crossapp_format_string("Shake your phone, Vote for %s", m_voiceMsg->m_playerName.c_str()));
        }
        label->setFontName("fonts/arial.ttf");
        m_shakeView->addSubview(label);
        
        m_shakeNumLabel = CALabel::createWithFrame(DRect(_px(40), _px(550), m_winSize.width - _px(80), _px(80)));
        m_shakeNumLabel->setTextAlignment(CATextAlignmentCenter);
        m_shakeNumLabel->setColor(CAColor_gray);
        m_shakeNumLabel->setFontSize(_px(50));
        m_shakeNumLabel->setText(crossapp_format_string("Shake Number: %d", m_shakeNum));
        m_shakeNumLabel->setFontName("fonts/arial.ttf");
        m_shakeView->addSubview(m_shakeNumLabel);
        
        label = CALabel::createWithFrame(DRect(_px(40), _px(300), m_winSize.width - _px(80), _px(80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_white);
        label->setFontSize(_px(35));
        if (m_demoMsg)
        {
            label->setText(crossapp_format_string("You have been voted for %s, thank you!", m_demoMsg->m_teamName.c_str()));
        }
        else if(m_voiceMsg)
        {
            label->setText(crossapp_format_string("You have been voted for %s, thank you!", m_voiceMsg->m_playerName.c_str()));
        }
        label->setFontName("fonts/arial.ttf");
        m_shakeEndView->addSubview(label);
    }
    else if(m_voteStatus == Vote_End)
    {
        CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(300), m_winSize.width - _px(80), _px(80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_white);
        label->setFontSize(_px(35));
        label->setText("This event has been over!");
        label->setFontName("fonts/arial.ttf");
        m_shakeView->addSubview(label);
    }
}

void VoteShakeViewController::requestMsg(int type)
{
    std::map<std::string, std::string> key_value;
    if (type == 0)
    {
        if (m_demoMsg)
        {
            key_value["tag"] = voteSubmitTag[0];
        }
        else
        {
            key_value["tag"] = voteSubmitTag[1];
        }
        
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(VoteShakeViewController::onRequestFinished));
    }
    else if(type == 1)
    {
        if (m_demoMsg)
        {
            key_value["tag"] = voteSubmitTag[2];
            key_value["vid"] = crossapp_format_string("%d", m_demoMsg->m_projectId);
        }
        else
        {
            key_value["tag"] = voteSubmitTag[3];
            key_value["vid"] = crossapp_format_string("%d", m_voiceMsg->m_projectId);
        }
        
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(VoteShakeViewController::onRequestVoteFinished));
    }
	
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
    }
}

void VoteShakeViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
		//requestMsg(1);
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 100)
    {
		requestMsg(0);
    }
	else if (btn->getTag() >= 300 && btn->getTag() < 305)
	{
	}
}

void VoteShakeViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        
        if (value["r"].asInt() == 0)
        {
            m_voteStatus = Vote_NotStart;
        }
        else if (value["r"].asInt() == 1)
        {
            m_voteStatus = Vote_Start;
        }
        else if (value["r"].asInt() == 2)
        {
            m_voteStatus = Vote_End;
        }
        int pId = 0;
        if (m_demoMsg)
        {
            pId = m_demoMsg->m_projectId;
        }
        else
        {
            pId = m_voiceMsg->m_projectId;
        }
        if (value["fv"].asInt() == pId || value["sv"].asInt() == pId)
        {
            m_voted = true;
            m_shakeView->setVisible(false);
            m_shakeEndView->setVisible(true);
        }
        else
        {
            m_voted = false;
            m_shakeView->setVisible(true);
            m_shakeEndView->setVisible(false);
        }
        
        initView();
    }
    else
    {
        //showAlert();
    }
#ifdef LOCALTEST
    {
		
    }
#endif
    
    m_canVote = true;
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

void VoteShakeViewController::onRequestVoteFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if(value["r"].asInt() == 1)
        {
            m_voted = true;
            
            m_shakeView->setVisible(false);
            m_shakeEndView->setVisible(true);
            if (m_demoMsg)
            {
                userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
                uInfo->m_demoVoteIdVec.push_back(m_demoMsg->m_projectId);
            }
            else
            {
                userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
                uInfo->m_voiceVoteIdVec.push_back(m_voiceMsg->m_projectId);
            }
        }

    }
    else
    {
        m_shakeNum = 29;
        if(m_shakeNumLabel)
            m_shakeNumLabel->setText(crossapp_format_string("Shake Number: %d", SHAKE_CNT - m_shakeNum));
    }
#ifdef LOCALTEST
    {
        
    }
#endif
    
    m_canVote = true;
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

void VoteShakeViewController::didAccelerate(CCAcceleration* pAccelerationValue)
{
    float nowGX = (pAccelerationValue->x)*9.81f;
    float nowGY = (pAccelerationValue->y)*9.81f;
    
    float dt = 30.f;
    if(m_voteStatus == Vote_Start && !m_voted && m_canVote && (nowGX<-dt||nowGX>dt || nowGY<-dt||nowGY>dt))
    {
        m_shakeNum++;
        if (m_shakeNum % 5 == 0 || m_shakeNum == 1)
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/vote.wav");
        }
        if(m_shakeNumLabel)
            m_shakeNumLabel->setText(crossapp_format_string("Shake Number: %d", SHAKE_CNT - m_shakeNum));
        if (m_shakeNum >= 30)
        {
            m_shakeNum = 30;
            m_canVote = false;
            requestMsg(1);
        }
    }
    
}
