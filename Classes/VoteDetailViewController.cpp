
#include "VoteDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"

VoteDetailViewController::VoteDetailViewController()
{
}

VoteDetailViewController::~VoteDetailViewController()
{

}

void VoteDetailViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(VoteDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Vote Notice");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);

	CAView* view = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
	this->getView()->addSubview(view);

//    CommonUrlImageView* urlImageVIew = CommonUrlImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
//    urlImageVIew->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
//    urlImageVIew->setImage(CAImage::create("common/bg.png"));
//    urlImageVIew->setTouchEnabled(false);
//    view->addSubview(urlImageVIew);
//    if(m_demoMsg)
//    {
//        urlImageVIew->setUrl(m_demoMsg->m_imageUrl);
//    }
//    else if(m_voiceMsg)
//    {
//        urlImageVIew->setUrl(m_voiceMsg->m_imageUrl);
//    }
    
    imageView = CAImageView::createWithImage(CAImage::create("vote/note_bg.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    imageView->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
    view->addSubview(imageView);
    
	label = CALabel::createWithFrame(DRect(_px(40), _px(120), m_winSize.width - _px(80), _px(90)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_gray);
	label->setFontSize(_px(35));
	label->setText("Vote event will be start after the show!");
	label->setFontName(SAP_FONT_ARIAL);
	view->addSubview(label);
//
//
//	button = CAButton::createWithFrame(DRect(_px(40), m_winSize.height - _px(120), m_winSize.width - _px(80), _px(100)), CAButtonTypeCustom);
//	imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
//	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
//	button->setBackGroundViewForState(CAControlStateAll, imageView);
//	button->setTitleForState(CAControlStateAll, "Submit and Get Session Points");
//	button->setTitleFontSize(_px(35));
//	button->setTitleColorForState(CAControlStateAll, CAColor_white);
//	button->addTarget(this, CAControl_selector(VoteDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
//	button->setTag(200);
//	this->getView()->addSubview(button);
//
//	
//
//    requestMsg();
//    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void VoteDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void VoteDetailViewController::requestMsg()
{
    std::map<std::string, std::string> key_value;
	key_value["tag"] = surveySubmitTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(VoteDetailViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
    }
}

void VoteDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 100)
    {
		requestMsg();
    }
	else if (btn->getTag() >= 300 && btn->getTag() < 305)
	{
	}
}

void VoteDetailViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
    }
    else
    {
        //showAlert();
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
