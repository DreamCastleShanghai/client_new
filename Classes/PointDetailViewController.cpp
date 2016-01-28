
#include "PointDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SimpleAudioEngine.h"

PointDetailViewController::PointDetailViewController(photoMsg& msg, int type)
: p_pLoading(NULL)
, m_msg(&msg)
, m_canLike(true)
, m_type(type)
{
    
}

PointDetailViewController::~PointDetailViewController()
{

}

void PointDetailViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(PointDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Moments Detail");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

    //m_view = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
    //this->getView()->addSubview(m_view);
    
    m_scrollView= CAScrollView::createWithFrame(DRect(0, _px(120),m_winSize.width,m_winSize.height - _px(120)));
    m_scrollView->setMinimumZoomScale(0.2f);
    m_scrollView->setMaximumZoomScale(5.0f);
    m_scrollView->setShowsHorizontalScrollIndicator(false);
    m_scrollView->setShowsVerticalScrollIndicator(false);
    this->getView()->addSubview(m_scrollView);
    
    m_urlImageVIew = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
    m_urlImageVIew->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
    m_urlImageVIew->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    m_urlImageVIew->setImage(CAImage::create("common/bg.png"));
    m_urlImageVIew->setUrl(m_msg->m_imageUrl);
    m_urlImageVIew->setTouchEnabled(false);
    m_scrollView->addSubview(m_urlImageVIew);
    
    if (m_type == 0)
    {
        CAView* view = CAView::createWithFrame(DRect(0, m_winSize.height - _px(120), m_winSize.width, _px(120)));
        view->setColor(ccc4(0, 0, 0, 100));
        this->getView()->addSubview(view);
        
        CommonUrlImageView* temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
        temImage->setFrame(DRect(_px(40), _px(20), _px(80), _px(80)));
        //CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        temImage->setImage(CAImage::create("common/bg.png"));
        temImage->setUrl(m_msg->m_iconUrl);
        temImage->setTouchEnabled(false);
        view->addSubview(temImage);
        
        label = CALabel::createWithFrame(DRect(_px(140), _px(20), m_winSize.width - _px(140), _px(35)));
        label->setTextAlignment(CATextAlignmentLeft);
        label->setColor(CAColor_blue);
        label->setFontSize(_px(30));
        label->setText(m_msg->name);
        label->setFontName("fonts/arial.ttf");
        view->addSubview(label);
        
        label = CALabel::createWithFrame(DRect(_px(140), _px(60), m_winSize.width - _px(140), _px(30)));
        label->setTextAlignment(CATextAlignmentLeft);
        label->setColor(CAColor_white);
        label->setFontSize(_px(25));
        label->setText(crossapp_format_string("#%s", m_msg->caterory.c_str()));
        label->setFontName("fonts/arial.ttf");
        view->addSubview(label);
        
        button = CAButton::createWithFrame(DRect(m_winSize.width - _px(160), _px(40), _px(50), _px(50)), CAButtonTypeCustom);
        m_likeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
        m_likeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_likeBtnImage->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
        button->setBackGroundViewForState(CAControlStateAll, m_likeBtnImage);
        button->addTarget(this, CAControl_selector(PointDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
        button->setTag(300);
        view->addSubview(button);
        
        if (m_msg->liked)
        {
            m_canLike = false;
            m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
        }
        else
        {
            m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
        }
        
        m_likeNumLabel = CALabel::createWithFrame(DRect(m_winSize.width - _px(100), _px(50), _px(200), _px(30)));
        m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
        m_likeNumLabel->setColor(CAColor_white);
        m_likeNumLabel->setFontSize(_px(25));
        m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->likeNum));
        m_likeNumLabel->setFontName("fonts/arial.ttf");
        view->addSubview(m_likeNumLabel);
    }

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void PointDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void PointDetailViewController::initView()
{
}

void PointDetailViewController::imageViewRequestFinished(CommonUrlImageView* imageView)
{
    m_scrollView->setViewSize(DSize(m_urlImageVIew->getImage()->getContentSize()));
}

void PointDetailViewController::requestLikeSubmit()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = momentsTag[2];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    key_value["pwid"] = crossapp_format_string("%d", m_msg->picId);
    key_value["v"] = crossapp_format_string("%d", 1);
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(PointDetailViewController::onRequestLikeSubmitFinished));
}

void PointDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if(btn->getTag() == 300) // like btn
    {
        if (m_canLike)
        {
            requestLikeSubmit();
        }
        m_canLike = false;
    }
}

void PointDetailViewController::onRequestLikeSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        
        if (value["r"].asInt() == 1)
        {
            m_canLike = false;
            m_msg->liked = true;
            m_msg->likeNum += 1;
            m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->likeNum));
            m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
        }
        else
        {
            m_canLike = true;
        }
    }
    else
    {
        m_canLike = true;
    }
#ifdef LOCALTEST
    {
		
    }
#endif
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}
