
#include "IconUploadViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"

IconUploadViewController::IconUploadViewController()
{

}

IconUploadViewController::~IconUploadViewController()
{

}

void IconUploadViewController::viewDidLoad()
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
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(IconUploadViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(140), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_album.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(IconUploadViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(30);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Photo");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

    
    ///////////////////
    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(140), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_album.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(IconUploadViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(30);
    this->getView()->addSubview(button);
    
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void IconUploadViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void IconUploadViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if(btn->getTag()==2) {
        DSize winSize = m_winSize;
        m_clvImage->setClippingEnabled(true);
        CARenderImage* rm = CARenderImage::create(_px(winSize.width-100), _px(winSize.width-100));
        rm->printscreenWithView(m_clvImage);
        
        renderImage = CAView::createWithFrame(DRect(50,winSize.height/4,winSize.width-100,winSize.width-100));
        this->getView()->addSubview(renderImage);
        
        m_clvImage->setClippingEnabled(false);
        
        if (m_clv!=NULL)
        {
            this->getView()->removeSubview(m_clv);
            m_clv = NULL;
            this->getView()->removeSubview(m_clvImage);
            m_clvImage = NULL;
            this->getView()->removeSubview(render_btn);
            render_btn = NULL;
        }
        
        CAImageView* imageView = CAImageView::createWithFrame(DRect(0,0,winSize.width-100,winSize.width-100));
        imageView->setImage(rm->getImageView()->getImage());
        renderImage->addSubview(imageView);
        
        CAScheduler::schedule(schedule_selector(IconUploadViewController::scheduleFuck), this, 3);
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
}

void IconUploadViewController::scheduleFuck(float dt)
{
    
    if (dle_ren_index>=1) {
        CAScheduler::unschedule(schedule_selector(IconUploadViewController::scheduleFuck), this);
        if (renderImage!=NULL)
        {
            this->getView()->removeSubview(renderImage);
            renderImage = NULL;
        }
        dle_ren_index = 0;
    }else{
        dle_ren_index++;
    }
    
    
}

CADrawView* IconUploadViewController::getStencil(const DSize& size, int index)
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

void IconUploadViewController::getSelectedImage(CAImage *image)
{
    DSize winSize = DSize(m_winSize.width, m_winSize.height - _px(120));
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
    this->getView()->addSubview(m_clvImage);
    
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
    scrollView->setBackgroundColor(CAColor_clear);
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
    this->getView()->addSubview(m_clv);
    
    DRect ivRect;
	ivRect.size = DSize(m_winSize.width, winSize.height - _px(120));
    ivRect.origin = ccpMult(scrollRect.origin, -1);
    CAView* iv = CAView::createWithColor(ccc4(0,0,0,128));
    iv->setFrame(ivRect);
    m_clv->addSubview(iv);
    
    render_btn = CAButton::create(CAButtonTypeSquareRect);
    render_btn->setCenter(DRect(winSize.width/2, winSize.height-100, 100, 50));
    render_btn->setTitleForState(CAControlStateNormal, "Click");
    render_btn->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    render_btn->addTarget(this, CAControl_selector(IconUploadViewController::buttonCallBack), CAControlEventTouchUpInSide);
    render_btn->setTag(2);
    this->getView()->addSubview(render_btn);
}


