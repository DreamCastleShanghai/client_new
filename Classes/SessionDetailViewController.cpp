
#include "SessionDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FServerTime.h"

SessionDetailViewController::SessionDetailViewController(newsMsg& msg)
: m_msg(&msg),
  m_canStore(true)
{
    m_isStore = msg.m_stored;
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
    
    CAView* view = CAView::createWithFrame(DRect(m_winSize.width - _px(140), _px(40) + _px(120), _px(100), _px(50)));
    
    CAImageView* likeImg = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
    likeImg->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    likeImg->setFrame(DRect(0, 0, _px(50), _px(50)));
    view->addSubview(likeImg);
    
    CALabel* m_likeNumLabel = CALabel::createWithFrame(DRect(_px(50), _px(20), _px(50), _px(30)));
    m_likeNumLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
    m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
    m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_likeNumLabel->setFontSize(_px(28));
    m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
    view->addSubview(m_likeNumLabel);
    this->getView()->addSubview(view);
    
    m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
    m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    CAButton* storeBtn = CAButton::createWithFrame(DRect(m_winSize.width - _px(200), _px(40) + _px(120), _px(50), _px(50)), CAButtonTypeRoundedRect);
    storeBtn->setAllowsSelected(true);
    storeBtn->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
    storeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack),CAControlEventTouchUpInSide);
    if (m_msg->m_stored)
    {
        m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
    }
    else
    {
        m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
    }
    this->getView()->addSubview(storeBtn);
    
    button = CAButton::createWithFrame(DRect(_px(40), _px(800), m_winSize.width - _px(80), _px(120)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    imageView = CAImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    button->setBackGroundViewForState(CAControlStateDisabled, imageView);
    button->setTitleColorForState(CAControlStateAll, ccc4(0x70, 0x70, 0x70, 0xff));
    button->setTitleForState(CAControlStateAll, "Attend");
    button->setTitleFontSize(_px(36));
    button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setControlState(CAControlStateDisabled);
    button->setTag(200);
    this->getView()->addSubview(button);
    
    label = CALabel::createWithFrame(DRect(_px(40), _px(930), m_winSize.width - _px(80), _px(30)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(ccc4(0x79, 0x79, 0x79, 0xff));
    label->setFontSize(_px(24));
    label->setText("* Click 'Attend' to get the points during the session");
    this->getView()->addSubview(label);
    
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