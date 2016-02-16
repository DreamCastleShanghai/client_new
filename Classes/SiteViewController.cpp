
#include "SiteViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"

SiteViewController::SiteViewController()
{
	m_alertView = NULL;
	m_pLoading = NULL;
}

SiteViewController::~SiteViewController()
{

}

void SiteViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;

	CAScale9ImageView* tabBg = CAScale9ImageView::createWithImage(CAImage::create("common/yellow_bg.png"));
	tabBg->setCapInsets(DRect(1, 1, 1, 1));
	tabBg->setFrame(DRect((0), (0), m_winSize.width, (150)));
	this->getView()->addSubview(tabBg);

    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(SiteViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);

	CALabel* mc = CALabel::createWithFrame(DRect((120), (62), (200), (50)));
	mc->setFontSize((40));
	mc->setText("Site");
	mc->setColor(CAColor_white);
	mc->setFontName(SAP_FONT_ARIAL);
	this->getView()->addSubview(mc);

    imageView = CAImageView::createWithFrame(DRect(0, (150), m_winSize.width, m_winSize.height - (150)));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    imageView->setImage(CAImage::create("common/site_map.png"));
    this->getView()->addSubview(imageView);
    
	button = CAButton::createWithFrame(DRect(m_winSize.width - 100, (50), (50), (50)), CAButtonTypeRoundedRect);
	imageView = CAImageView::createWithImage(CAImage::create("common/share_btn.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	button->setTag(200);
	button->setBackgroundViewForState(CAControlStateAll, imageView);
	button->addTarget(this, CAControl_selector(SiteViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(button);

	requestMsg();
	m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
	m_pLoading->setLoadingMinTime(0.5f);
	this->getView()->addSubview(m_pLoading);

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SiteViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SiteViewController::requestMsg() // all(0) track(1) format(2) my_all(3)
{
	if (m_alertView) {
		this->getView()->removeSubview(m_alertView);
		m_alertView = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
	key_value["type"] = "0";
	key_value["section"] = "0";
	key_value["limit"] = "20";
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SiteViewController::onRequestFinished));
}

void SiteViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 100)
	{
		RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else if (btn->getTag() == 200)
	{

	}
	else if (btn->getTag() == 500)
	{
		requestMsg();
		if (m_pLoading == NULL)
		{
			m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - (200)) / 2, 50, 50));
			this->getView()->insertSubview(m_pLoading, CAWindowZOderTop);
			m_pLoading->setLoadingMinTime(0.5f);
		}
	}
}

void SiteViewController::showAlert()
{
	if (m_alertView) {
		this->getView()->removeSubview(m_alertView);
		m_alertView = NULL;
	}

	m_alertView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (150)));
	this->getView()->addSubview(m_alertView);

	CAImageView* bg = CAImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (150)));
	bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	bg->setImage(CAImage::create("common/bg.png"));

	CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
	btn5->setTag(500);
	btn5->setCenter(DRect(m_winSize.width / 2, (m_winSize.height - (150)) / 2, m_winSize.width, m_winSize.height - (150)));
	btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
	btn5->setBackgroundViewForState(CAControlStateNormal, bg);
	btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
	btn5->addTarget(this, CAControl_selector(SiteViewController::buttonCallBack), CAControlEventTouchUpInSide);
	m_alertView->addSubview(btn5);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - (400), m_winSize.width, (60)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	test->setFontSize((40));
	test->setText("Network cannot connect!");
	m_alertView->addSubview(test);

}

void SiteViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];

	}
	else
	{
		//showAlert(id);
	}

	{

	}

	if (m_pLoading)
	{
		m_pLoading->stopAnimating();
		this->getView()->removeSubview(m_pLoading);
		m_pLoading = NULL;
	}
}


