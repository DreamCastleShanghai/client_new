
#include "SessionDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FServerTime.h"
#include "SurveyViewController.h"

SessionDetailViewController::SessionDetailViewController(sessionMsg& msg)
: m_msg(&msg),
  m_canStore(true),
  m_canLike(true)
{
    m_isStore = msg.m_stored;
	m_isLiked = msg.m_liked;
}

SessionDetailViewController::~SessionDetailViewController()
{

}

void SessionDetailViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Session");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

    
    //////////////////////
    CommonUrlImageView* m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
    //createWithFrame(DRect(_px(30), _px(40), _px(80), _px(80)));
    m_urlImageView->setFrame(DRect(_px(30), _px(40) + _px(120), _px(80), _px(80)));
    //m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    m_urlImageView->setImage(CAImage::create("common/bg.png"));
    m_urlImageView->setUrl(m_msg->m_imageUrl);
    this->getView()->addSubview(m_urlImageView);
    
    CALabel* m_titleLabel = CALabel::createWithFrame(DRect(_px(140), _px(40) + _px(120), m_winSize.width / 2, _px(40)));
    m_titleLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    m_titleLabel->setTextAlignment(CATextAlignmentLeft);
    m_titleLabel->setFontSize(_px(32));
    m_titleLabel->setText(m_msg->m_title);
    this->getView()->addSubview(m_titleLabel);
    
    CALabel* m_timeLabel = CALabel::createWithFrame(DRect(_px(140), _px(90) + _px(120), m_winSize.width / 2, _px(30)));
    m_timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    m_timeLabel->setTextAlignment(CATextAlignmentLeft);
    //m_timeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_timeLabel->setFontSize(_px(28));
    m_timeLabel->setText(crossapp_format_string("%s - %s", timeToString(m_msg->m_startTime).c_str(), timeToString(m_msg->m_endTime).c_str()));
    this->getView()->addSubview(m_timeLabel);
    
    CALabel* m_lectureLabel = CALabel::createWithFrame(DRect(_px(140), _px(130) + _px(120), m_winSize.width / 2, _px(25)));
    m_lectureLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    m_lectureLabel->setTextAlignment(CATextAlignmentLeft);
    m_lectureLabel->setFontSize(_px(24));
    m_lectureLabel->setText(m_msg->m_lecturer);
    this->getView()->addSubview(m_lectureLabel);
    
    CALabel* fromatLabel = CALabel::createWithFrame(DRect(_px(140), _px(170) + _px(120), m_winSize.width, _px(25)));
    fromatLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    fromatLabel->setTextAlignment(CATextAlignmentLeft);
    fromatLabel->setFontSize(_px(24));
    fromatLabel->setText(crossapp_format_string("Track: %s    Format: %s", m_msg->m_track.c_str(), m_msg->m_format.c_str()));
    this->getView()->addSubview(fromatLabel);
    
    CALabel* timeLabel = CALabel::createWithFrame(DRect(_px(140), _px(210) + _px(120), m_winSize.width, _px(25)));
    timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    timeLabel->setTextAlignment(CATextAlignmentLeft);
    timeLabel->setFontSize(_px(24));
    timeLabel->setText(crossapp_format_string("Location: %s", m_msg->m_location.c_str()));
    this->getView()->addSubview(timeLabel);

    
    CALabel* m_lectureDetailLabel = CALabel::createWithFrame(DRect(_px(140), _px(250) + _px(120), m_winSize.width - _px(210), _px(430)));
    m_lectureDetailLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
    m_lectureDetailLabel->setTextAlignment(CATextAlignmentLeft);
    m_lectureDetailLabel->setFontSize(_px(24));
    m_lectureDetailLabel->setText(m_msg->m_detail);
    this->getView()->addSubview(m_lectureDetailLabel);
    

	// tail
	sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
	sView->setFrame(DRect(_px(0), m_winSize.height - _px(100), m_winSize.width, _px(100)));
	this->getView()->addSubview(sView);

	m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
	m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	m_storeBtnImage->setFrame(DRect(_px(35), _px(0), _px(80), _px(80)));
	CAButton* storeBtn = CAButton::createWithFrame(DRect(_px(0), _px(0), _px(150), _px(100)), CAButtonTypeCustom);
	storeBtn->setAllowsSelected(true);
	//storeBtn->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
	storeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	storeBtn->addSubview(m_storeBtnImage);
	if (m_msg->m_stored)
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
	}
	else
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
	}
	sView->addSubview(storeBtn);

	m_likeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
	m_likeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	m_likeBtnImage->setFrame(DRect(_px(35), _px(0), _px(80), _px(80)));
	CAButton* likeBtn = CAButton::createWithFrame(DRect(_px(150), _px(0), _px(150), _px(100)), CAButtonTypeCustom);
	likeBtn->setAllowsSelected(true);
	//likeBtn->setBackGroundViewForState(CAControlStateAll, m_likeBtnImage);
	likeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	likeBtn->addSubview(m_likeBtnImage);
	if (m_msg->m_liked)
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}
	else
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
	}
	sView->addSubview(likeBtn);


	CALabel* m_likeNumLabel = CALabel::createWithFrame(DRect(_px(255), _px(30), _px(50), _px(35)));
	m_likeNumLabel->setColor(CAColor_white);
	m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
	m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	m_likeNumLabel->setFontSize(_px(20));
	m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
	m_likeNumLabel->setTouchEnabled(false);
	sView->addSubview(m_likeNumLabel);

	m_surveyButtonView[0] = CAView::createWithFrame(DRect(_px(0), _px(0), _px(400), _px(100)));
	m_surveyButtonView[0]->setColor(CAColor_clear);
	label = CALabel::createWithFrame(DRect(_px(0), _px(0), _px(400), _px(100)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(35));
	label->setText("Click to win point");
	m_surveyButtonView[0]->addSubview(label);
	m_surveyButtonView[0]->setVisible(false);

	m_surveyButtonView[1] = CAView::createWithFrame(DRect(_px(0), _px(0), _px(300), _px(100)));
	m_surveyButtonView[1]->setColor(CAColor_clear);
	label = CALabel::createWithFrame(DRect(_px(0), _px(10), _px(300), _px(40)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(30));
	label->setText("Click to win point");
	m_surveyButtonView[1]->addSubview(label);

	m_surveyTimeLabel = CALabel::createWithFrame(DRect(_px(0), _px(50), _px(300), _px(30)));
	m_surveyTimeLabel->setTextAlignment(CATextAlignmentCenter);
	m_surveyTimeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	m_surveyTimeLabel->setColor(CAColor_white);
	m_surveyTimeLabel->setFontSize(_px(20));
	m_surveyTimeLabel->setText("(Avalible in 30s)");
	m_surveyButtonView[1]->addSubview(m_surveyTimeLabel);

	button = CAButton::createWithFrame(DRect(m_winSize.width - _px(300), 0, _px(300), _px(100)), CAButtonTypeCustom);
	imageView = CAImageView::createWithImage(CAImage::create("common/dsky_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	imageView = CAImageView::createWithImage(CAImage::create("common/gray_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateDisabled, imageView);
	button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	//button->setControlState(CAControlStateDisabled);
	button->setTag(200);
	sView->addSubview(button);

	button->addSubview(m_surveyButtonView[0]);
	button->addSubview(m_surveyButtonView[1]);
	//label = CALabel::createWithFrame(DRect(_px(0), _px(0), _px(400), _px(100)));
	//label->setTextAlignment(CATextAlignmentCenter);
	//label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	//label->setColor(CAColor_white);
	//label->setFontSize(_px(35));
	//label->setText("Click to win point");
	//button->addSubview(label);

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void SessionDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 200)
    {
		SurveyViewController* vc = new SurveyViewController(m_msg->m_sessionId);
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
}


void SessionDetailViewController::storeBtnCallBack(CAControl* btn, DPoint point)
{
    if (m_canStore)
    {
        std::map<std::string, std::string> key_value;
        key_value["tag"] = sessionViewTag[1];
        key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        //key_value["sto"] = crossapp_format_string("%d", m_isStore ? 1 : 0);
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onStoreRequestFinished));
        
        m_canStore = false;
    }
}

void SessionDetailViewController::onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        m_isStore = !m_isStore;
        m_msg->m_stored = m_isStore;
        if (m_isStore)
        {
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
        }
        else
        {
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
        }
    }
    m_canStore = true;
}