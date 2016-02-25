
#include "VoteShakeViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SimpleAudioEngine.h"

/*
VoteShakeViewController::VoteShakeViewController(demoJamMsg* dMsg, voiceMsg* vMsg, eggHikingMsg* eMsg)
: m_view(NULL)
, m_shakeView(NULL)
, m_shakeEndView(NULL)
, m_demoMsg(dMsg)
, m_voiceMsg(vMsg)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_shakeNumLabel(NULL)
, m_projectType(0)
, m_shakeNum(Vote_NotStart)
, m_voted(false)
{
    if (vMsg)
    {
        m_projectType = 1;
    }
}
 */

VoteShakeViewController::VoteShakeViewController(void* msg, int type)
: m_view(NULL)
, m_shakeView(NULL)
, m_shakeEndView(NULL)
//, m_demoMsg(dMsg)
//, m_voiceMsg(vMsg)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_shakeNumLabel(NULL)
, m_projectType(0)
, m_shakeNum(Vote_NotStart)
, m_voted(false)
, m_tye(type)
, m_msg(msg)
{
    
}

VoteShakeViewController::~VoteShakeViewController()
{
    CAApplication::getApplication()->getAccelerometer()->setDelegate(NULL);
}

void VoteShakeViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect((0), (0), m_winSize.width, (120)));
    this->getView()->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(VoteShakeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Vote");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);

	m_view = CAView::createWithFrame(DRect(0, (120), m_winSize.width, m_winSize.height - (120)));
	this->getView()->addSubview(m_view);
    
    CommonUrlImageView* urlImageVIew = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
    urlImageVIew->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (120)));
    urlImageVIew->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    urlImageVIew->setColor(ccc4(128, 128, 128, 255));
    urlImageVIew->setTouchEnabled(false);
    m_view->addSubview(urlImageVIew);
    switch (m_tye) {
        case TYPE_DJ:
            urlImageVIew->setUrl(((demoJamMsg*)m_msg)->m_imageUrl);
            break;
        case TYPE_VOICE:
            urlImageVIew->setUrl(((voiceMsg*)m_msg)->m_imageUrl);
            break;
        case TYPE_EH:
            urlImageVIew->setUrl(((eggHikingMsg*)m_msg)->m_imageUrl);
            break;
        default:
            break;
    }
    /*
    if(m_demoMsg)
    {
        urlImageVIew->setUrl(m_demoMsg->m_imageUrl);
    }
    else if(m_voiceMsg)
    {
        urlImageVIew->setUrl(m_voiceMsg->m_imageUrl);
    }
     */

    m_shakeView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (120)));
    m_shakeView->setColor(CAColor_clear);
    m_view->addSubview(m_shakeView);
    //m_shakeView->setVisible(false);
    
    m_shakeEndView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (120)));
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
        CALabel* label = CALabel::createWithFrame(DRect((40), (300), m_winSize.width - (80), (80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_white);
        label->setFontSize((35));
        label->setText("Voting will start soon!");
        label->setFontName(SAP_FONT_ARIAL);
        m_shakeView->addSubview(label);
    }
    else if (m_voteStatus == Vote_Start)
    {
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("vote/icon_shake.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        imageView->setFrame(DRect((m_winSize.width - (260)) / 2, (150), (260), (260)));
        m_shakeView->addSubview(imageView);
        
        CALabel* label = CALabel::createWithFrame(DRect((40), (450), m_winSize.width - (80), (80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setWordWrap(true);
        label->setColor(CAColor_red);
        label->setFontSize((25));
        switch (m_tye) {
            case TYPE_DJ:
                //urlImageVIew->setUrl(((demoJamMsg*)m_msg)->m_imageUrl);
                label->setText(crossapp_format_string("Shake your phone, Vote for %s", ((demoJamMsg*)m_msg)->m_teamName.c_str()));
                break;
            case TYPE_VOICE:
                //urlImageVIew->setUrl(((voiceMsg*)m_msg)->m_imageUrl);
                label->setText(crossapp_format_string("Shake your phone, Vote for %s", ((voiceMsg*)m_msg)->m_playerName.c_str()));
                break;
            case TYPE_EH:
                label->setText(crossapp_format_string("Shake your phone, Vote for hiking"));
                //urlImageVIew->setUrl(((eggHikingMsg*)m_msg)->m_imageUrl);
                break;
            default:
                break;
        }
        /*
        if (m_demoMsg)
        {
            label->setText(crossapp_format_string("Shake your phone, Vote for %s", m_demoMsg->m_teamName.c_str()));
        }
        else if(m_voiceMsg)
        {
            label->setText(crossapp_format_string("Shake your phone, Vote for %s", m_voiceMsg->m_playerName.c_str()));
        }
         */
        m_shakeView->addSubview(label);
        
        m_shakeNumLabel = CALabel::createWithFrame(DRect((40), (550), m_winSize.width - (80), (80)));
        m_shakeNumLabel->setTextAlignment(CATextAlignmentCenter);
        m_shakeNumLabel->setColor(CAColor_gray);
        m_shakeNumLabel->setFontSize((50));
        m_shakeNumLabel->setText(crossapp_format_string("Shake Number: %d", SHAKE_CNT -  m_shakeNum));
        m_shakeView->addSubview(m_shakeNumLabel);
        
        label = CALabel::createWithFrame(DRect((40), (300), m_winSize.width - (80), (80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_white);
        label->setWordWrap(true);
        label->setFontSize((25));
        /*
        if (m_demoMsg)
        {
            label->setText(crossapp_format_string("You have been voted for %s, thank you!", m_demoMsg->m_teamName.c_str()));
        }
        else if(m_voiceMsg)
        {
            label->setText(crossapp_format_string("You have been voted for %s, thank you!", m_voiceMsg->m_playerName.c_str()));
        }
         */
        m_shakeEndView->addSubview(label);
        switch (m_tye) {
            case TYPE_DJ:
                //urlImageVIew->setUrl(((demoJamMsg*)m_msg)->m_imageUrl);
                label->setText(crossapp_format_string("You have voted for %s, thank you!", ((demoJamMsg*)m_msg)->m_teamName.c_str()));
                break;
            case TYPE_VOICE:
                //urlImageVIew->setUrl(((voiceMsg*)m_msg)->m_imageUrl);
                label->setText(crossapp_format_string("You have voted for %s, thank you!", ((voiceMsg*)m_msg)->m_playerName.c_str()));
                break;
            case TYPE_EH:
                label->setText(crossapp_format_string("You have voted for hiking, thank you!"));
                //urlImageVIew->setUrl(((eggHikingMsg*)m_msg)->m_imageUrl);
                break;
            default:
                break;
        }
    }
    else if(m_voteStatus == Vote_End)
    {
        CALabel* label = CALabel::createWithFrame(DRect((40), (300), m_winSize.width - (80), (80)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_white);
        label->setFontSize((35));
        label->setText("Voting ended!");
        label->setFontName(SAP_FONT_ARIAL);
        m_shakeView->addSubview(label);
    }
}

void VoteShakeViewController::requestMsg(int type)
{
    std::map<std::string, std::string> key_value;
    if (type == 0)
    {
        if (m_tye == TYPE_DJ)
        {
            key_value["tag"] = voteSubmitTag[0];
        }
        else if (m_tye == TYPE_VOICE)
        {
            key_value["tag"] = voteSubmitTag[1];
        }
        else if (m_tye == TYPE_EH)
        {
            key_value["tag"] = voteSubmitTag[2];
        }
        
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(VoteShakeViewController::onRequestFinished));
    }
    else if(type == 1)
    {
        if (m_tye == TYPE_DJ)
        {
            key_value["tag"] = voteSubmitTag[3];
            key_value["vid"] = crossapp_format_string("%d", ((demoJamMsg*)m_msg)->m_projectId);
        }
        else if (m_tye == TYPE_VOICE)
        {
            key_value["tag"] = voteSubmitTag[4];
            key_value["vid"] = crossapp_format_string("%d", ((voiceMsg*)m_msg)->m_projectId);
        }
        else if (m_tye == TYPE_EH)
        {
            key_value["tag"] = voteSubmitTag[5];
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

//#ifdef LOCAL_DEBUG
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
//#endif
        bool canVote = false;
        const CSJson::Value& value = json["result"];
        
        if (m_tye == TYPE_EH)
        {
            m_voteStatus = value["r"].asInt();
            canVote = true;
        }
        else
        {
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
            if (m_tye == TYPE_DJ)
            {
                pId = ((demoJamMsg*)m_msg)->m_projectId;
            }
            else if (m_tye == TYPE_VOICE)
            {
                pId = ((voiceMsg*)m_msg)->m_projectId;
            }
            if (value["fv"].asInt() == pId || value["sv"].asInt() == pId)
                canVote = false;
            else
                canVote = true;
        }
        
        if (!canVote)
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
#ifdef LOCAL_DEBUG
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
#endif
        const CSJson::Value& value = json["result"];
        if(value["r"].asInt() == 1)
        {
            m_voted = true;
            
            m_shakeView->setVisible(false);
            m_shakeEndView->setVisible(true);
            if (m_tye == TYPE_DJ)
            {
                userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
                uInfo->m_demoVoteIdVec.push_back(((demoJamMsg*)m_msg)->m_projectId);
            }
            else if (m_tye == TYPE_VOICE)
            {
                userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
                uInfo->m_voiceVoteIdVec.push_back(((voiceMsg*)m_msg)->m_projectId);
            }
            else if (m_tye == TYPE_EH)
            {
                userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
                uInfo->m_eggVoted = true;
            }
        }
        else
        {
            m_shakeNum = SHAKE_CNT - 5;
            if(m_shakeNumLabel)
                m_shakeNumLabel->setText(crossapp_format_string("Shake Number: %d", SHAKE_CNT - m_shakeNum));
        }
    }
    else
    {
        m_shakeNum = SHAKE_CNT - 5;

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
    
    float dt = 20.f;
    if(m_voteStatus == Vote_Start && !m_voted && m_canVote && (nowGX<-dt || nowGY<-dt))
    {
        m_shakeNum++;
        if (m_shakeNum % 5 == 0 || m_shakeNum == 1)
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/vote.wav");
        }
        if(m_shakeNumLabel)
            m_shakeNumLabel->setText(crossapp_format_string("Shake Number: %d", SHAKE_CNT - m_shakeNum));
        if (m_shakeNum >= SHAKE_CNT)
        {
            m_shakeNum = SHAKE_CNT;
            m_canVote = false;
            requestMsg(1);
        }
    }
    
}
