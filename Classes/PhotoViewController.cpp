
#include "PhotoViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "PhotoViewController.h"

PhotoViewController::PhotoViewController(int type)
: m_type(type)
, m_currentCategory("")
, m_mask(NULL)
, m_filterView(NULL)
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
    
    m_basicView = CAScale9ImageView::createWithImage(CAImage::create("common/white_bg.png"));
    m_basicView->setFrame(DRect((0), (0), m_winSize.width, m_winSize.height));
    this->getView()->addSubview(m_basicView);
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect((0), (0), m_winSize.width, (120)));
    m_basicView->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    sView->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Photo");
    label->setFontName(SAP_FONT_ARIAL);
    label->setTouchEnabled(false);
    sView->addSubview(label);
    
    ///////////////////
    button = CAButton::createWithFrame(DRect((40), (160), (200), (200)), CAButtonTypeCustom);
    sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect((0), (0), (200), (200)));
    button->addSubview(sView);
    imageView = CAImageView::createWithImage(CAImage::create("moments/upload_icon.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    imageView->setFrame(DRect((50), (50), (100), (100)));
    button->addSubview(imageView);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(200);
    m_basicView->addSubview(button);

    button = CAButton::createWithFrame(DRect((280), (160), (200), (200)), CAButtonTypeCustom);
    sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect((0), (0), (200), (200)));
    button->addSubview(sView);
    imageView = CAImageView::createWithImage(CAImage::create("moments/album.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    imageView->setFrame(DRect((50), (50), (100), (100)));
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
        if (m_type == 1 && m_currentCategory == "")
        {
            CAAlertView *alertView = CAAlertView::createWithText("Warning !", "Select  one photo tag !", "OK", NULL);
            alertView->show();
            return;
        }
        
        if (m_type == 1)
        {
            std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/" + "2.jpg";
            m_getImage->saveToFile(imagePath.c_str());
            CCLog("path : %s", imagePath.c_str());
            requestPhotoSubmit(imagePath);
            
        }
        else
        {
            m_pScrollView->setHorizontalScrollEnabled(false);
            m_pScrollView->setVerticalScrollEnabled(false);
            
            DSize winSize = m_winSize;
            m_clv->setAlphaThreshold(1.f);
            //m_clv->setClippingEnabled(true);
            //m_photoView->removeSubview(m_clv);
            m_clv->setVisible(false);
            
            m_clvImage->setAlphaThreshold(0.5f);
            m_clvImage->setClippingEnabled(true);
            m_clvImage->visit();
            
            int w = (120);
            int h = (120);
            CARenderImage* rm = CARenderImage::create(m_winSize.width - 100, m_winSize.width - 100);
            rm->printscreenWithView(m_clvImage, CAColor_black);
            
            CAImage* mask = CAImage::create("common/head_bg.png");
            CAImage* image = CAImage::scaleToNewImageWithImage(rm->getImageView()->getImage(), DSize(w, h));
            
            CAImage* newImage = new CAImage();
            unsigned char* data = new unsigned char[w * h * 4];
            
            // Resample.  Simple average 2x2 --> 1, in-place.
            int	pitch = 4;
            for (int j = 0; j < h * w; j++)
            {
                int	r, g, b, a, ma;
                unsigned char*	out = ((unsigned char*)data) + j * pitch;
                unsigned char*	in = ((unsigned char*)image->getData()) + j * pitch;
                unsigned char*  msk = ((unsigned char*)mask->getData()) + j * pitch;
                r = *(in + 0);
                g = *(in + 1);
                b = *(in + 2);
                a = *(in + 3);
                ma = *(msk + 3);
                
                *(out + 0) = r;
                *(out + 1) = g;
                *(out + 2) = b;
                
                if (ma == 0) {
                    *(out + 3) = 0;
                } else {
                    *(out + 3) = a;
                }
                /*
                for (int i = 0; i < w; i++)
                {
                    int	r, g, b, a;
                    r = (*(in + 0) + *(in + 4) + *(in + 0 + pitch) + *(in + 4 + pitch));
                    g = (*(in + 1) + *(in + 5) + *(in + 1 + pitch) + *(in + 5 + pitch));
                    b = (*(in + 2) + *(in + 6) + *(in + 2 + pitch) + *(in + 6 + pitch));
                    a = (*(in + 3) + *(in + 7) + *(in + 3 + pitch) + *(in + 7 + pitch));
                    *(out + 0) = r >> 2;
                    *(out + 1) = g >> 2;
                    *(out + 2) = b >> 2;
                    *(out + 3) = a >> 2;
                    out += 4;
                    in += 8;
                }
                 */
            }
            newImage->initWithRawData(data, CAImage::PixelFormat_RGBA8888, w, h);
            newImage->autorelease();
            delete[] data;

            std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/" + "2.png";
            newImage->saveToFile(imagePath.c_str());
            //image->saveToFile(imagePath.c_str());
            CCLog("path : %s", imagePath.c_str());
            requestPhotoSubmit(imagePath);
            
            FDataManager::getInstance()->setUserDirty(true);
        }
        
    }
    else if (btn->getTag() == 500) // cancle
    {
        m_basicView->setVisible(true);
        this->getView()->removeSubview(m_photoView);
        m_photoView = NULL;
    }
    else if (btn->getTag() == 600)
    {
        m_filterView->setVisible(!(m_filterView->isVisible()));
    }
    else if(m_type == 1)
    {
        for (int i = 1; i < MOMENTSFILTERNUM; i++)
        {
            if (!strcmp(filterMoments[i], btn->getTextTag().c_str()))
            {
                m_currentCategory = filterMoments[i];
                m_filterButton->setTitleForState(CAControlStateAll, m_currentCategory);
            }
        }
        m_filterView->setVisible(false);
    }
}

void PhotoViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
//        CSJson::FastWriter writer;
//        string tempjson = writer.write(json);
//        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if(value["r"].asString() == "1")
        {
            m_basicView->setVisible(true);
            this->getView()->removeSubview(m_photoView);
            m_photoView = NULL;
            RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
            if (m_type == 0) {
                FDataManager::getInstance()->setUserDirty(true);
            } else if (m_type == 1) {
                FDataManager::getInstance()->setRankDirty(true);
            }
        }
        else
        {
            if (m_type == 0)
            {
                m_clv->setVisible(true);
                m_clvImage->setClippingEnabled(false);
            }
            
        }
        
    }
    else
    {
        if (m_type == 0)
        {
            m_clv->setVisible(true);
            m_clvImage->setClippingEnabled(false);
        }
        
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
    key_value["cat"] = m_currentCategory;
    key_value["ptype"] = "png";
    CommonHttpManager::getInstance()->send_postFile(httpUrl, key_value, fullPath, this, CommonHttpJson_selector(PhotoViewController::onRequestFinished));
    {
        DRect r(m_winSize.width / 2, (m_winSize.height - (120)) / 2 + (120),
                m_winSize.width, m_winSize.height - (120));
        p_pLoading = CAActivityIndicatorView::createWithCenter(r);
        p_pLoading->setLoadingMinTime(0.5f);
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
    }
}

CADrawView* PhotoViewController::getStencil(const DSize& size, int index)
{
    if (m_mask == NULL) {
        
    }
    if (index == 0)
    {
        DPoint ver[4];
        ver[0] = DPoint(0, 0);
        ver[1] = DPoint(0, size.height);
        ver[2] = DPoint(size.width, size.height);
        ver[3] = DPoint(size.width, 0);
        m_mask = CADrawView::create();
        m_mask->drawPolygon(ver, 4, ccc4f(255, 0, 0, 0), 2, ccc4f(255, 0, 0, 0));
        //stencil->drawDot(DPoint(size.width / 2, size.height / 2), size.width / 2, ccc4f(255, 255, 0, 0));
        m_mask->setFrameOrigin(DPoint(0, size.height));
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
        m_mask = CADrawView::create();
        m_mask->drawPolygon(cir, 720, ccc4f(0, 0, 0, 144), 0, ccc4f(1, 1, 1, 0));
        m_mask->setCenterOrigin(DPoint(size.width/2, size.height/2));
    }
    return m_mask;
}

void PhotoViewController::getSelectedImage(CAImage *image)
{
    m_basicView->setVisible(false);
    
    m_photoView = CAScale9ImageView::createWithImage(CAImage::create("common/black_bg.png"));
    m_photoView->setFrame(DRect((0), (0), m_winSize.width, m_winSize.height));
    this->getView()->addSubview(m_photoView);
    
    if(m_type == 0)
    {
        DSize winSize = DSize(m_winSize.width, m_winSize.height);
        DRect scrollRect;
        scrollRect.origin.x = 50;
        scrollRect.origin.y = winSize.height/4;
        scrollRect.size.width = winSize.width-100;
        scrollRect.size.height = scrollRect.size.width;
        
        m_clvImage = CAClippingView::create();
        
        m_clvImage->setStencil(getStencil(scrollRect.size, 1));
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
        m_pScrollView = CAScrollView::createWithFrame(m_clvImage->getBounds());
        m_pScrollView->setViewSize(DSize(image->getContentSize()));
        m_pScrollView->setContentOffset(DPoint(0,winSize.height/4), false);
        m_pScrollView->setMinimumZoomScale(temp_mini);
        m_pScrollView->setMaximumZoomScale(2.5f);
        m_pScrollView->setBackgroundColor(CAColor_clear);
        m_pScrollView->setShowsScrollIndicators(false);
        m_pScrollView->setBounces(false);
        m_pScrollView->setScrollViewDelegate(this);
        m_pScrollView->setDisplayRange(true);
        m_pScrollView->setMultitouchGesture(CAScrollView::Zoom);
        m_clvImage->addSubview(m_pScrollView);
        
        DRect rect;
        rect.origin = DPointZero;
        rect.size = m_pScrollView->getViewSize();
        CAImageView* imv = CAImageView::createWithFrame(rect);
        imv->setImage(image);
        imv->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        m_pScrollView->addSubview(imv);

        m_clv = CAClippingView::create();
        m_clv->setStencil(getStencil(scrollRect.size, 1));
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
    }
    else
    {
        int w = image->getPixelsWide();
        int h = image->getPixelsHigh();
        if (w * 9 > h * 16)
        {
            h = h * (640) / w;
            w = (640);
        }
        else
        {
            w = w * (360) / h;
            h = (360);
        }
        
        m_getImage = CAImage::scaleToNewImageWithImage(image, DSize(w, h));
        CAImageView* iView = CAImageView::createWithImage(m_getImage);
        iView->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height));
        iView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        m_photoView->addSubview(iView);
    }

    CAButton* button = CAButton::createWithFrame(DRect((80),  m_winSize.height -  (200), (200), (60)), CAButtonTypeCustom);
    button->setTitleForState(CAControlStateAll, "Select");
    button->setTitleFontName(SAP_FONT_ARIAL);
    button->setTitleFontSize((40));
    button->setTitleColorForState(CAControlStateAll, CAColor_white);
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect((0), (0), (200), (60)));
    //button->addSubview(sView);
    //button->setBackgroundViewForState(CAControlStateAll, CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png")));
    button->setTag(400);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_photoView->addSubview(button);
    
    button = CAButton::createWithFrame(DRect(m_winSize.width - (280),  m_winSize.height -  (200), (200), (60)), CAButtonTypeCustom);
    button->setTitleForState(CAControlStateAll, "Cancel");
    button->setTitleFontName(SAP_FONT_ARIAL);
    button->setTitleFontSize((40));
    button->setTitleColorForState(CAControlStateAll, CAColor_white);
    sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect((0), (0), (200), (60)));
    //button->addSubview(sView);
    //button->setBackgroundViewForState(CAControlStateAll, CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png")));
    button->setTag(500);
    button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_photoView->addSubview(button);

    // moments photo tag
    if (m_type == 1)
    {
        m_filterButton = CAButton::createWithFrame(DRect(0, (30), m_winSize.width, (100)), CAButtonTypeCustom);
        m_filterButton->setTitleForState(CAControlStateAll, "Tag");
        m_filterButton->setTitleFontSize((40));
        m_filterButton->setTitleColorForState(CAControlStateAll, CAColor_white);
        m_filterButton->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_filterButton->setTag(600);
        m_photoView->addSubview(m_filterButton);
        
        m_filterView = CAView::createWithFrame(DRect((m_winSize.width - (200)) / 2, (100), (240), (80) * (MOMENTSFILTERNUM - 1)));
        m_filterView->setColor(ccc4(0, 0, 0, 128));
        m_photoView->addSubview(m_filterView);
        m_filterView->setVisible(false);
        
        for (int i = 1; i < MOMENTSFILTERNUM; i++)// filterMoments
        {
            button = CAButton::createWithFrame(DRect((0), (80) * (i - 1), (240), (80)), CAButtonTypeCustom);
            button->addTarget(this, CAControl_selector(PhotoViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTextTag(filterMoments[i]);
            button->setTitleForState(CAControlStateAll, crossapp_format_string("#%s", filterMoments[i]));
            button->setTitleFontSize((27));
            button->setTitleColorForState(CAControlStateAll, CAColor_white);
            m_filterView->addSubview(button);
        }
    }

}


