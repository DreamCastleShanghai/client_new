
#include "MapViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

MapViewController::MapViewController()
: m_pLoading(NULL)
, m_leftBtn(NULL)
, m_rightBtn(NULL)
, m_pageOne(NULL)
, m_pageTwo(NULL)
, m_pageOneAddressLabel(NULL)
, m_pageOneImage(NULL)
, m_pageTwoImage(NULL)
{
    requestInsideguideMsg();
}

MapViewController::~MapViewController()
{

}

void MapViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;

    int hight = _px(120);
    // header
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, hight));
    this->getView()->addSubview(sView);
    
    // head title
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Map");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

    // header back btn
    CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(ID_BACK);
    this->getView()->addSubview(button);
    
    int btnHight = _px(60);
    
    // left btn
    if (m_leftBtn == NULL)
    {
        m_leftBtn = CAButton::createWithFrame(DRect(_px(0), hight, m_winSize.width / 2, btnHight), CAButtonTypeCustom);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_leftBtn->setBackGroundViewForState(CAControlStateAll, imageView);
        imageView = CAImageView::createWithImage(CAImage::create("common/dsky_bg.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_leftBtn->setBackGroundViewForState(CAControlStateSelected, imageView);
        m_leftBtn->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_leftBtn->setTag(ID_OUTSIDE);
        m_leftBtn->setTitleForState(CAControlStateAll, "Outside");
        m_leftBtn->setTitleFontName("fonts/arial.ttf");
        m_leftBtn->setTitleFontSize(_px(30));
        m_leftBtn->setTitleColorForState(CAControlStateAll, CAColor_white);
        this->getView()->addSubview(m_leftBtn);
        
        m_leftBtn->setControlState(CAControlStateSelected);
        
    }
    
    // right btn
    if (m_rightBtn == NULL)
    {
        m_rightBtn = CAButton::createWithFrame(DRect(m_winSize.width / 2, hight, m_winSize.width / 2, btnHight), CAButtonTypeCustom);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_rightBtn->setBackGroundViewForState(CAControlStateAll, imageView);
        imageView = CAImageView::createWithImage(CAImage::create("common/dsky_bg.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_rightBtn->setBackGroundViewForState(CAControlStateSelected, imageView);
        m_rightBtn->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_rightBtn->setTag(ID_INSIDE);
        m_rightBtn->setTitleForState(CAControlStateAll, "Inside");
        m_rightBtn->setTitleFontName("fonts/arial.ttf");
        m_rightBtn->setTitleFontSize(_px(30));
        m_rightBtn->setTitleColorForState(CAControlStateAll, CAColor_white);
        this->getView()->addSubview(m_rightBtn);
    }
    
    hight += btnHight;
    
    
    
    /*
    m_segView = FSegmentView::createWithFrame(DRect(m_winSize.width / 2, _px(120), m_winSize.width, 60), 2);
    if (m_segView)
    {
        m_segView->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_segView->setItemTile("Outside", 0);
        m_segView->setItemTile("Inside", 1);
        m_segView->setTag(200, 0);
        m_segView->setTag(201, 1);
        this->getView()->addSubview(m_segView);
    }
     */

    /*
    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(140), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_forward.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(30);
    this->getView()->addSubview(button);
    */
    
    // page one
    m_pageOne = CAScrollView::createWithFrame(DRect(0, hight, m_winSize.width, m_winSize.height - hight));
    if (m_pageOne) {
        //scrollView->setViewSize(DSize(m_winSize.width - _px(40), (m_imageHeight));
        m_pageOne->setHorizontalScrollEnabled(false);
        m_pageOne->setVerticalScrollEnabled(true);
        m_pageOne->setBounceHorizontal(false);
        m_pageOne->setBounds(DRect(0, _px(240), m_winSize.width, m_winSize.height - hight));
        m_pageOne->setAnchorPoint(DPoint(0.f, 0.f));
        m_pageOne->setShowsHorizontalScrollIndicator(false);
        m_pageOne->setShowsVerticalScrollIndicator(false);
        m_pageOne->setBackGroundColor(CAColor_clear);
        m_pageOne->setVisible(true);
        this->getView()->addSubview(m_pageOne);
        
        m_pageOneAddressLabel = CALabel::createWithFrame(DRect(_px(40), _px(40), m_winSize.width - _px(80), _px(100)));
        if (m_pageOneAddressLabel) {
            m_pageOneAddressLabel->setText("address content");//surveyDetail[0]
            m_pageOneAddressLabel->setVerticalScrollEnabled(true);
            m_pageOneAddressLabel->setHorizontalScrollEnabled(true);
            m_pageOneAddressLabel->setColor(CAColor_black);
            m_pageOneAddressLabel->setFontSize(_px(30));
            m_pageOneAddressLabel->setBold(true);
            //label->setItalics(true);
            m_pageOneAddressLabel->setFontName("fonts/arial.ttf");
            m_pageOne->addSubview(m_pageOneAddressLabel);
        }
        
        m_pageOneImage = CommonUrlImageView::createWithFrame(DRect(0, _px(140), m_winSize.width - _px(80), _px(400)));
        if (m_pageOneImage) {
            // test code
            m_pageOneImage->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            m_pageOneImage->setImage(CAImage::create("login/login_bg.png"));
            m_pageOne->addSubview(m_pageOneImage);
        }
        //test code
        CAImageView* temp = CAImageView::createWithImage(CAImage::create("login/login_bg.png"));
        temp->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        temp->setFrame(DRect(0, _px(140), m_winSize.width, m_winSize.height));
        m_pageOne->addSubview(temp);
    }

    // page two
    m_pageTwo = CAScrollView::createWithFrame(DRect(0, hight, m_winSize.width, m_winSize.height - hight));
    if (m_pageTwo) {
        //scrollView->setViewSize(DSize(m_winSize.width - _px(40), (m_imageHeight));
        m_pageTwo->setHorizontalScrollEnabled(false);
        m_pageTwo->setVerticalScrollEnabled(true);
        m_pageTwo->setBounceHorizontal(false);
        m_pageTwo->setBounds(DRect(0, 0, m_winSize.width, m_winSize.height - hight));
        m_pageTwo->setAnchorPoint(DPoint(0.f, 0.f));
        m_pageTwo->setShowsHorizontalScrollIndicator(false);
        m_pageTwo->setShowsVerticalScrollIndicator(false);
        m_pageTwo->setBackGroundColor(CAColor_clear);
        m_pageTwo->setVisible(false);
        this->getView()->addSubview(m_pageTwo);
        
        m_pageTwoImage = CommonUrlImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - hight));
        if (m_pageTwoImage) {
            m_pageTwoImage->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            m_pageTwoImage->setImage(CAImage::create("login/login_bg.png"));
            m_pageTwo->addSubview(m_pageTwoImage);
        }
        
        CAImageView* temp = CAImageView::createWithImage(CAImage::create("login/login_bg.png"));
        temp->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        temp->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - hight));
        m_pageTwo->addSubview(temp);
    }
    
    //requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void MapViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

/*
void MapViewController::initMsgTableView()
{
    if (m_msg.empty())
    {
        showAlert();
        return;
    }

}
 */

void MapViewController::requestInsideguideMsg()
{
    /*
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
     */
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
    key_value["page"] = "1";
    key_value["limit"] = "20";
    key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MapViewController::onRequestFinished));
    
    if (m_pLoading) {
        m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(m_pLoading, CAWindowZOderTop);
    }
    if (m_pLoading) {
        m_pLoading->setLoadingMinTime(0.5f);
//        m_pLoading->setTargetOnCancel(this, callfunc_selector(MapViewController::initMsgTableView));
    }
}

void MapViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    int tag = btn->getTag();
    switch (tag) {
        case ID_BACK:
            RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
            break;
        case ID_OUTSIDE:
            m_pageOne->setVisible(true);
            m_pageTwo->setVisible(false);
            m_leftBtn->setControlState(CAControlStateSelected);
            m_rightBtn->setControlState(CAControlStateNormal);
            break;
        case ID_INSIDE:
            m_pageTwo->setVisible(true);
            m_pageOne->setVisible(false);
            m_rightBtn->setControlState(CAControlStateSelected);
            m_leftBtn->setControlState(CAControlStateNormal);
            break;
        default:
            break;
    }
}

void MapViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg.clear();
    }
    /*
    else
    {
        showAlert();
    }
     */

#ifdef LOCALTEST
	{

	}
#endif

    if (m_pLoading)
    {
        m_pLoading->stopAnimating();
    }
}


/*
void MapViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(120)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}*/

