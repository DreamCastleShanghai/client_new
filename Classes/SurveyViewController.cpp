
#include "SurveyViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

SurveyViewController::SurveyViewController(int sessionId)
: p_alertView(NULL)
, p_pLoading(NULL)
, m_listView(NULL)
, m_sessionId(sessionId)
, m_score(0)
{
    
}

SurveyViewController::~SurveyViewController()
{

}

void SurveyViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Sustainability Survey");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

	CAView* view = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
	this->getView()->addSubview(view);

	label = CALabel::createWithFrame(DRect(_px(40), _px(40), m_winSize.width - _px(80), _px(80)));
	//label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_gray);
	label->setFontSize(_px(35));
	label->setText("How do you like this session?");
	label->setFontName("fonts/arial.ttf");
	view->addSubview(label);

	for (int i = 0; i < 5; i++)
	{
		button = CAButton::createWithFrame(DRect(_px(40) + i * _px(100), _px(100), _px(80), _px(100)), CAButtonTypeSquareRect);
		imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("session/face%d_btn.png", i + 1)));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		//imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
		button->setBackGroundViewForState(CAControlStateAll, imageView);
		imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("session/face%d_btn_pre.png", i + 1)));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		//imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
		button->setBackGroundViewForState(CAControlStateSelected, imageView);
		button->setAllowsSelected(true);
		button->setTag(300 + i);
		button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
		m_scoreButtonVec.push_back(button);
		view->addSubview(button);
	}

	label = CALabel::createWithFrame(DRect(_px(40), _px(240), m_winSize.width - _px(80), _px(80)));
	//label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_gray);
	label->setFontSize(_px(35));
	label->setText("Fadeback to the speaker:");
	label->setFontName("fonts/arial.ttf");
	view->addSubview(label);

	m_feedBackTextView = CATextView::createWithFrame(DRect(_px(40), _px(330), m_winSize.width - _px(80), _px(300)));
	m_feedBackTextView->setFontSize(_px(35));
	m_feedBackTextView->setFontName("fonts/arial.ttf");
	m_feedBackTextView->setColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
	m_feedBackTextView->setBackGroundColor(CAColor_gray);
	view->addSubview(m_feedBackTextView);

	m_alertLabel = CALabel::createWithFrame(DRect(_px(40), _px(640), m_winSize.width - _px(80), _px(40)));
	m_alertLabel->setText("*Feedback failed, please try again!");
	m_alertLabel->setColor(CAColor_red);
	m_alertLabel->setFontSize(_px(35));
	m_alertLabel->setFontName("fonts/arial.ttf");
	m_alertLabel->setVisible(false);
	view->addSubview(m_alertLabel);

	button = CAButton::createWithFrame(DRect(_px(40), m_winSize.height - _px(120), m_winSize.width - _px(80), _px(100)), CAButtonTypeCustom);
	imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	button->setTitleForState(CAControlStateAll, "Submit and Get Session Points");
	button->setTitleFontSize(_px(35));
	button->setTitleColorForState(CAControlStateAll, CAColor_white);
	button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(200);
	this->getView()->addSubview(button);

	

    //requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SurveyViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SurveyViewController::requestMsg()
{
    std::map<std::string, std::string> key_value;
	key_value["tag"] = suveyTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
	key_value["sid"] = crossapp_format_string("%d", m_sessionId);
	key_value["vlu"] = crossapp_format_string("%d", m_score);
	key_value["fdd"] = m_feedBackTextView->getText();
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SurveyViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
    }
}

void SurveyViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 200)
    {
		requestMsg();
    }
	else if (btn->getTag() >= 300 && btn->getTag() < 305)
	{
		int index = btn->getTag() - 300;
		for (int i = 0; i < m_scoreButtonVec.size(); i++)
		{
			if (i <= index)
			{
				m_scoreButtonVec[i]->setControlState(CAControlStateSelected);
			}
			else
			{
				m_scoreButtonVec[i]->setControlState(CAControlStateNormal);
			}
		}
		m_score = index + 1;
	}
}

void SurveyViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];

		m_alertLabel->setText("*Feedback failed, please try again!");
		m_alertLabel->setVisible(true);
    }
    else
    {
		m_alertLabel->setText("Feedback success! ");
		m_alertLabel->setColor(CAColor_gray);
		m_alertLabel->setVisible(true);
        //showAlert();
    }
    
    {
		for (int i = 0; i < 3; i++)
		{
			surveyDetail tmp;
			tmp.m_itemId = i;
			tmp.m_surveyId = 1;
			tmp.m_itemDetail = "title:  This is Photoshop's version of Lorem, This is Photoshop's version of Lorem,";
			tmp.m_itemSelection.push_back("selection0: This is Photoshop's version of Lorem, This is Photoshop's version of Lorem,");
			tmp.m_itemSelection.push_back("selection1: This is Photoshop's version of Lorem, This is Photoshop's version of Lorem,");
			tmp.m_itemSelection.push_back("selection2: This is Photoshop's version of Lorem, This is Photoshop's version of Lorem,");
			m_msg.push_back(tmp);
		}
		
		
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}
