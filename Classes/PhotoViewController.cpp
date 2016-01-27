
#include "PhotoViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "PhotoViewController.h"

PhotoViewController::PhotoViewController(int type)
: m_type(type)
{
dle_ren_index = 0;
}

PhotoViewController::~PhotoViewController()
{

}

void PhotoViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    
    m_photoView = CAScale9ImageView::createWithImage(CAImage::create("common/black_bg.png"));
    m_photoView->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height));
    this->getView()->addSubview(m_photoView);
    m_photoView->setVisible(false);
    
    m_basicView = CAScale9ImageView::createWithImage(CAImage::create("common/white_bg.png"));
    m_basicView->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height));
    this->getView()->addSubview(m_basicView);
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(120)));
    m_basicView->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    sView->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Photo");
    label->setFontName("fonts/arial.ttf");
    label->setTouchEnabled(false);
    sView->addSubview(label);
    
    ///////////////////
    button = CAButton::createWithFrame(DRect(_px(40), _px(160), _px(200), _px(200)), CAButtonTypeCustom);
    sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), _px(200), _px(200)));
    button->addSubview(sView);
    imageView = CAImageView::createWithImage(CAImage::create("moments/upload_icon.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    imageView->setFrame(DRect(_px(50), _px(50), _px(100), _px(100)));
    button->addSubview(imageView);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(200);
    m_basicView->addSubview(button);

    button = CAButton::createWithFrame(DRect(_px(280), _px(160), _px(200), _px(200)), CAButtonTypeCustom);
    sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), _px(200), _px(200)));
    button->addSubview(sView);
    imageView = CAImageView::createWithImage(CAImage::create("moments/album.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    imageView->setFrame(DRect(_px(50), _px(50), _px(100), _px(100)));
    button->addSubview(imageView);
    button->setColor(CAColor_magenta);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(300);
    m_basicView->addSubview(button);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void PhotoViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void PhotoViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if(btn->getTag()==2) {

    }
    else if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 30)
    {
        CADevice::openAlbum(this);
        //CADevice::openCamera(this);
    }
    else if (btn->getTag() == 200)
    {
        CADevice::openCamera(this);
    }
    else if (btn->getTag() == 300)
    {
        CADevice::openAlbum(this);
    }
    else if (btn->getTag() == 400) // select
    {
        DSize winSize = m_winSize;
        m_clvImage->setClippingEnabled(true);
        CARenderImage* rm = CARenderImage::create(_px(winSize.width-100), _px(winSize.width-100));
        rm->printscreenWithView(m_clvImage);

        renderImage = CAView::createWithFrame(DRect(50,winSize.height/4,winSize.width-100,winSize.width-100));
        m_photoView->addSubview(renderImage);
        
        //m_clvImage->setClippingEnabled(false);
        
        if (m_clv!=NULL)
        {
            m_photoView->removeSubview(m_clv);
            m_clv = NULL;
            m_photoView->removeSubview(m_clvImage);
            m_clvImage = NULL;
        }
        
        CAImageView* imageView = CAImageView::createWithFrame(DRect(0,0,winSize.width-100,winSize.width-100));
        imageView->setImage(rm->getImageView()->getImage());
        renderImage->addSubview(imageView);
        
        std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/" + "2.png";
        imageView->getImage()->saveToFile(imagePath);
        CCLog("path : %s", imagePath.c_str());
        requestPhotoSubmit(imagePath);
    }
    else if (btn->getTag() == 500) // cancle
    {
        m_basicView->setVisible(true);
        m_photoView->setVisible(false);
        m_photoView->removeAllSubviews();
    }
    
}

void PhotoViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if(value["r"].asString() == "1")
        {
            m_basicView->setVisible(true);
            m_photoView->setVisible(false);
            m_photoView->removeAllSubviews();
        }
        else
        {
            
        }
        
    }
    else
    {
        
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

void PhotoViewController::requestPhotoSubmit(std::string fullPath)
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = iconUploadTag[m_type];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    key_value["cat"] = "developers";
    key_value["ptype"] = "png";
    CommonHttpManager::getInstance()->send_postFile(httpUrl, key_value, fullPath, this, CommonHttpJson_selector(PhotoViewController::onRequestFinished));
    {
        DRect r(m_winSize.width / 2, (m_winSize.height - _px(120)) / 2 + _px(120),
                m_winSize.width, m_winSize.height - _px(120));
        p_pLoading = CAActivityIndicatorView::createWithCenter(r);
        p_pLoading->setLoadingMinTime(0.5f);
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
    }
}

CADrawView* PhotoViewController::getStencil(const DSize& size, int index)
{
    if (index == 0)
    {
        DPoint ver[4];
        ver[0] = DPoint(0, 0);
        ver[1] = DPoint(0, size.height);
        ver[2] = DPoint(size.width, size.height);
        ver[3] = DPoint(size.width, 0);
        CADrawView* stencil = CADrawView::create();
        stencil->drawPolygon(ver, 4, ccc4f(255, 0, 0, 0), 2, ccc4f(255, 0, 0, 0));
        //stencil->drawDot(DPoint(size.width / 2, size.height / 2), size.width / 2, ccc4f(255, 255, 0, 0));
        stencil->setFrameOrigin(DPoint(0, size.height));
        return stencil;
    }
    else if (index == 1)
    {
        DPoint cir[720];
        for (int i=0; i<720; i++)
        {
            float x = cosf(i * M_PI/180.f) * size.width/2;
            float y = sinf(i * M_PI/180.f) * size.width/2;
            cir[i] = DPoint(x, y);
        }
        CADrawView* stencil = CADrawView::create();
        stencil->drawPolygon(cir, 720, ccc4f(1, 1, 1, 0.5), 0, ccc4f(1, 1, 1, 0));
        stencil->setCenterOrigin(DPoint(size.width/2, size.height/2));
        return stencil;
    }
    return NULL;
}

void PhotoViewController::getSelectedImage(CAImage *image)
{
    m_basicView->setVisible(false);
    m_photoView->setVisible(true);
    
    DSize winSize = DSize(m_winSize.width, m_winSize.height);
    DRect scrollRect;
    scrollRect.origin.x = 50;
    scrollRect.origin.y = winSize.height/4;
    scrollRect.size.width = winSize.width-100;
    scrollRect.size.height = scrollRect.size.width;
    
    m_clvImage = CAClippingView::create();
    
    m_clvImage->setStencil(getStencil(scrollRect.size, 0));
    m_clvImage->setFrame(scrollRect);
    m_clvImage->setInverted(false);
    m_clvImage->setClippingEnabled(false);
    m_photoView->addSubview(m_clvImage);
    
    float temp_mini = 0;
    if (image->getContentSize().width>image->getContentSize().height) {
        temp_mini = scrollRect.size.height/image->getContentSize().height;
    }else{
        temp_mini = scrollRect.size.width/image->getContentSize().width;
    }
    CAScrollView* scrollView = CAScrollView::createWithFrame(m_clvImage->getBounds());
    scrollView->setViewSize(DSize(image->getContentSize()));
    scrollView->setContentOffset(DPoint(0,winSize.height/4), false);
    scrollView->setMinimumZoomScale(temp_mini);
    scrollView->setMaximumZoomScale(2.5f);
    scrollView->setBackGroundColor(CAColor_clear);
    scrollView->setShowsScrollIndicators(false);
    scrollView->setBounces(false);
    scrollView->setScrollViewDelegate(this);
    scrollView->setDisplayRange(true);
    m_clvImage->addSubview(scrollView);
    
    DRect rect;
    rect.origin = DPointZero;
    rect.size = scrollView->getViewSize();
    CAImageView* imv = CAImageView::createWithFrame(rect);
    imv->setImage(image);
    imv->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    scrollView->addSubview(imv);

    m_clv = CAClippingView::create();
    m_clv->setStencil(getStencil(scrollRect.size, 0));
    m_clv->setFrame(scrollRect);
    m_clv->setInverted(true);
    m_clv->setTouchEnabled(false);
    m_photoView->addSubview(m_clv);
    
    DRect ivRect;
	ivRect.size = DSize(m_winSize.width, winSize.height);
    ivRect.origin = ccpMult(scrollRect.origin, -1);
    CAView* iv = CAView::createWithColor(ccc4(0,0,0,128));
    iv->setFrame(ivRect);
    m_clv->addSubview(iv);
    
    CAButton* button = CAButton::createWithFrame(DRect(_px(80),  m_winSize.height -  _px(200), _px(200), _px(60)), CAButtonTypeCustom);
    button->setTitleForState(CAControlStateAll, "Select");
    button->setTitleFontName("fonts/arial.ttf");
    button->setTitleFontSize(_px(40));
    button->setTitleColorForState(CAControlStateAll, CAColor_white);
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), _px(200), _px(60)));
    //button->addSubview(sView);
    //button->setBackGroundViewForState(CAControlStateAll, CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png")));
    button->setTag(400);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_photoView->addSubview(button);
    
    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(280),  m_winSize.height -  _px(200), _px(200), _px(60)), CAButtonTypeCustom);
    button->setTitleForState(CAControlStateAll, "Cancel");
    button->setTitleFontName("fonts/arial.ttf");
    button->setTitleFontSize(_px(40));
    button->setTitleColorForState(CAControlStateAll, CAColor_white);
    sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), _px(200), _px(60)));
    //button->addSubview(sView);
    //button->setBackGroundViewForState(CAControlStateAll, CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png")));
    button->setTag(500);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_photoView->addSubview(button);
  
}


