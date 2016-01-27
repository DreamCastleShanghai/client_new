
#include "MapViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

MapViewController::MapViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
{

}

MapViewController::~MapViewController()
{

}

void MapViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(MapViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
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
    imageView = CAImageView::createWithImage(CAImage::create("login/login_bg.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(0,_px(120),m_winSize.width,m_winSize.height - _px(120)));
    this->getView()->addSubview(imageView);

    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Map");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);
    //requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void MapViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void MapViewController::initMsgTableView()
{
    if (m_msg.empty())
    {
        showAlert();
        return;
    }

}

void MapViewController::requestMsg()
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
    key_value["page"] = "1";
    key_value["limit"] = "20";
    key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MapViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(MapViewController::initMsgTableView));
    }
}

void MapViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 30)
    {
        
    }
    else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
        requestMsg();
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
    else
    {
        showAlert();
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

}

