//
//  PhotoTagViewController.cpp
//  SapSession
//
//  Created by bbknightzm on 16/2/4.
//
//

#include "PhotoTagViewController.h"

PhotoTagViewController::PhotoTagViewController()
: m_submitImage(NULL)
, p_pLoading(NULL)
{
}

PhotoTagViewController::~PhotoTagViewController()
{
    
}

void PhotoTagViewController::viewDidLoad()
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
    label->setText("Select Tag");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);
    
    // header cancel btn
    CAButton* cancelbutton = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    if (cancelbutton) {
 //       cancelbutton->setBackGroundViewForState(CAControlStateAll, imageView);
        cancelbutton->addTarget(this, CAControl_selector(PhotoTagViewController::buttonCallBack), CAControlEventTouchUpInSide);
        cancelbutton->setTag(ID_CANCEL);
        cancelbutton->setTitleForState(CAControlStateAll, "Cancel");
        cancelbutton->setTitleFontSize(30);
        cancelbutton->setTitleColorForState(CAControlStateAll, CAColor_white);
        this->getView()->addSubview(cancelbutton);
    }
    
    // header finish btn
    CAButton* okbutton = CAButton::createWithFrame(DRect(_px(0), m_winSize.width - _px(120), _px(100), _px(100)), CAButtonTypeCustom);
    if (okbutton) {
 //       okbutton->setBackGroundViewForState(CAControlStateAll, imageView);
        okbutton->addTarget(this, CAControl_selector(PhotoTagViewController::buttonCallBack), CAControlEventTouchUpInSide);
        okbutton->setTag(ID_CANCEL);
        okbutton->setTitleForState(CAControlStateAll, "Finished");
        okbutton->setTitleFontSize(30);
        okbutton->setTitleColorForState(CAControlStateAll, CAColor_white);
        this->getView()->addSubview(okbutton);
    }
    
    // picture preview
    CAImage* preImage = CAImage::create(m_file);
    if (preImage)
    {
        int preWidth = preImage->getPixelsWide();
        int preHeight = preImage->getPixelsHigh();
        int nowHeight = (m_winSize.height - _px(120)) / 2;
        int nowWidth = preWidth * nowHeight / preHeight;
        
        m_submitImage = CAImageView::createWithFrame(DRect((m_winSize.width - nowWidth) / 2, _px(120) + _px(40), nowWidth, nowHeight));
        m_submitImage->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        m_submitImage->setImage(preImage);
        this->getView()->addSubview(m_submitImage);
    }

}

void PhotoTagViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void PhotoTagViewController::requestSubmitPictureMsg()
{
    if (!m_file.empty())
    {
        std::map<std::string, std::string> key_value;
        key_value["tag"] = iconUploadTag[m_tagId];
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        key_value["cat"] = filterMoments[m_tagId];
        key_value["ptype"] = "png";
        CommonHttpManager::getInstance()->send_postFile(httpUrl, key_value, m_file, this, CommonHttpJson_selector(PhotoTagViewController::onRequestFinished));
        {
            DRect r(m_winSize.width / 2, (m_winSize.height - _px(120)) / 2 + _px(120),
                    m_winSize.width, m_winSize.height - _px(120));
            p_pLoading = CAActivityIndicatorView::createWithCenter(r);
            p_pLoading->setLoadingMinTime(0.5f);
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        }
    }
}

void PhotoTagViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    int tag = btn->getTag();
    switch (tag) {
        case ID_CANCEL:
            RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
            break;
        case ID_FINISH:
            break;
        default:
            m_tagId = tag - ID_TAG;
            break;
    }
}

void PhotoTagViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
    }

    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

