
#include "SessionDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FServerTime.h"
#include "SurveyViewController.h"

SessionDetailViewController::SessionDetailViewController(sessionMsg &msg)
: m_msg(&msg),
  m_canStore(true),
  m_canLike(true)
{
    m_isStore = msg.m_stored;
	m_isLiked = msg.m_liked;
	m_detailMsg.m_sessionId = -1;
}

SessionDetailViewController::~SessionDetailViewController()
{

}

void SessionDetailViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Session");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);
	
	if (m_detailMsg.m_sessionId == -1)
	{
		requestMsg();
	}
	else
	{
		initView();
	}

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionDetailViewController::initView()
{
	if (m_detailMsg.m_sessionId == -1)
	{
		showAlert();
		return;
	}
	//////////////////////
	//CommonUrlImageView* m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
	////createWithFrame(DRect(_px(30), _px(40), _px(80), _px(80)));
	//m_urlImageView->setFrame(DRect(_px(30), _px(40) + _px(120), _px(80), _px(80)));
	////m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	//m_urlImageView->setImage(CAImage::create("common/bg.png"));
	//m_urlImageView->setUrl(m_detailMsg.m_speaker[0].iconUrl);
	//this->getView()->addSubview(m_urlImageView);

	CALabel* m_titleLabel = CALabel::createWithFrame(DRect(_px(40), _px(40) + _px(120), m_winSize.width - _px(80), _px(120)));
	m_titleLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	m_titleLabel->setTextAlignment(CATextAlignmentLeft);
	m_titleLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentBottom);
	m_titleLabel->setFontSize(_px(32));
	m_titleLabel->setText(m_msg->m_title);
	this->getView()->addSubview(m_titleLabel);

	CALabel* m_timeLabel = CALabel::createWithFrame(DRect(_px(40), _px(190) + _px(120), m_winSize.width - _px(80), _px(30)));
	m_timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	m_timeLabel->setTextAlignment(CATextAlignmentLeft);
	//m_timeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	m_timeLabel->setFontSize(_px(28));
	m_timeLabel->setText(crossapp_format_string("%s - %s", timeToString(m_msg->m_startTime).c_str(), timeToString(m_msg->m_endTime).c_str()));
	this->getView()->addSubview(m_timeLabel);

	CALabel* fromatLabel = CALabel::createWithFrame(DRect(_px(40), _px(270) + _px(120), m_winSize.width - _px(80), _px(25)));
	fromatLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	fromatLabel->setTextAlignment(CATextAlignmentLeft);
	fromatLabel->setFontSize(_px(24));
	fromatLabel->setText(crossapp_format_string("Track: %s    Format: %s", m_msg->m_track.c_str(), m_msg->m_format.c_str()));
	this->getView()->addSubview(fromatLabel);

	CALabel* timeLabel = CALabel::createWithFrame(DRect(_px(40), _px(310) + _px(120), m_winSize.width - _px(80), _px(25)));
	timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	timeLabel->setTextAlignment(CATextAlignmentLeft);
	timeLabel->setFontSize(_px(24));
	timeLabel->setText(crossapp_format_string("Location: %s", m_msg->m_location.c_str()));
	this->getView()->addSubview(timeLabel);


	CALabel* m_lectureDetailLabel = CALabel::createWithFrame(DRect(_px(40), _px(350) + _px(120), m_winSize.width - _px(80), _px(300)));
	m_lectureDetailLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
	m_lectureDetailLabel->setTextAlignment(CATextAlignmentLeft);
	m_lectureDetailLabel->setFontSize(_px(24));
	m_lectureDetailLabel->setText(m_detailMsg.m_detail);
	this->getView()->addSubview(m_lectureDetailLabel);

	CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(770), m_winSize.width - _px(80), _px(35)));
	label->setColor(CAColor_gray);
	label->setTextAlignment(CATextAlignmentLeft);
	label->setFontSize(_px(30));
	label->setText("Speaker");
	this->getView()->addSubview(label);

	for (int i = 0; i < m_detailMsg.m_speaker.size(); i++)
	{
		CommonUrlImageView* urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
		urlImageView->setFrame(DRect(_px(100) + i * _px(200), _px(820), _px(80), _px(80)));
		urlImageView->setImage(CAImage::create("common/bg.png"));
		urlImageView->setUrl(m_detailMsg.m_speaker[i].iconUrl);
		this->getView()->addSubview(urlImageView);

		label = CALabel::createWithFrame(DRect(_px(40) + i * _px(200), _px(900), _px(200), _px(30)));
		label->setColor(CAColor_gray);
		label->setTextAlignment(CATextAlignmentCenter);
		label->setFontSize(_px(25));
		label->setText(m_detailMsg.m_speaker[i].name);
		this->getView()->addSubview(label);
	}


	// tail
	CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
	sView->setFrame(DRect(_px(0), m_winSize.height - _px(100), m_winSize.width, _px(100)));
	this->getView()->addSubview(sView);

	m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
	m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	m_storeBtnImage->setFrame(DRect(_px(35), _px(0), _px(80), _px(80)));
	CAButton* storeBtn = CAButton::createWithFrame(DRect(_px(0), _px(0), _px(150), _px(100)), CAButtonTypeCustom);
	storeBtn->setAllowsSelected(true);
	//storeBtn->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
	storeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	storeBtn->addSubview(m_storeBtnImage);
	storeBtn->setTag(300);

	m_isStore = m_msg->m_stored;
	m_canStore = true;
	if (m_msg->m_stored)
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
	}
	else
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
	}
	sView->addSubview(storeBtn);

	m_likeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
	m_likeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	m_likeBtnImage->setFrame(DRect(_px(35), _px(0), _px(80), _px(80)));
	CAButton* likeBtn = CAButton::createWithFrame(DRect(_px(150), _px(0), _px(150), _px(100)), CAButtonTypeCustom);
	likeBtn->setAllowsSelected(true);
	likeBtn->setTag(400);
	//likeBtn->setBackGroundViewForState(CAControlStateAll, m_likeBtnImage);
	likeBtn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	likeBtn->addSubview(m_likeBtnImage);

	m_canLike = !(m_msg->m_liked);
	if (!m_canLike)
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}
	else
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
	}
	sView->addSubview(likeBtn);

	m_likeNumLabel = CALabel::createWithFrame(DRect(_px(255), _px(30), _px(50), _px(35)));
	m_likeNumLabel->setColor(CAColor_white);
	m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
	m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	m_likeNumLabel->setFontSize(_px(20));
	m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
	m_likeNumLabel->setTouchEnabled(false);
	sView->addSubview(m_likeNumLabel);

	m_surveyButtonView[0] = CAView::createWithFrame(DRect(_px(0), _px(0), _px(400), _px(100)));
	m_surveyButtonView[0]->setColor(CAColor_clear);
	label = CALabel::createWithFrame(DRect(_px(0), _px(0), _px(400), _px(100)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(35));
	label->setText("Click to win point");
	m_surveyButtonView[0]->addSubview(label);
	m_surveyButtonView[0]->setVisible(false);

	m_surveyButtonView[1] = CAView::createWithFrame(DRect(_px(0), _px(0), _px(300), _px(100)));
	m_surveyButtonView[1]->setColor(CAColor_clear);
	label = CALabel::createWithFrame(DRect(_px(0), _px(10), _px(300), _px(40)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(30));
	label->setText("Click to win point");
	m_surveyButtonView[1]->addSubview(label);

	m_surveyTimeLabel = CALabel::createWithFrame(DRect(_px(0), _px(50), _px(300), _px(30)));
	m_surveyTimeLabel->setTextAlignment(CATextAlignmentCenter);
	m_surveyTimeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	m_surveyTimeLabel->setColor(CAColor_white);
	m_surveyTimeLabel->setFontSize(_px(20));
	m_surveyTimeLabel->setText("(Avalible in 30s)");
	m_surveyButtonView[1]->addSubview(m_surveyTimeLabel);

	CAButton* button = CAButton::createWithFrame(DRect(m_winSize.width - _px(300), 0, _px(300), _px(100)), CAButtonTypeCustom);
	CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/dsky_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	imageView = CAImageView::createWithImage(CAImage::create("common/gray_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateDisabled, imageView);
	button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	//button->setControlState(CAControlStateDisabled);
	button->setTag(200);
	sView->addSubview(button);

	button->addSubview(m_surveyButtonView[0]);
	button->addSubview(m_surveyButtonView[1]);
}

void SessionDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void SessionDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
	else if (btn->getTag() == 100)
	{
		this->getView()->removeSubview(p_alertView);
		p_alertView = NULL;
		requestMsg();
	}
    else if (btn->getTag() == 200)
    {
		SurveyViewController* vc = new SurveyViewController(m_msg->m_sessionId);
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
	else if (btn->getTag() == 300)
	{
		requestStore();
	}
	else if (btn->getTag() == 400)
	{
		requestLike();
	}
}

void SessionDetailViewController::requestMsg()
{
	std::map<std::string, std::string> key_value;
	key_value["tag"] = sessionDetailViewTag[0];
	key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
	//key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onRequestFinished));
	{
		DRect r(m_winSize.width / 2, (m_winSize.height - _px(120)) / 2 + _px(120),
			m_winSize.width, m_winSize.height - _px(120));
		p_pLoading = CAActivityIndicatorView::createWithCenter(r);
		p_pLoading->setLoadingMinTime(0.5f);
		this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
	}
}

void SessionDetailViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		CSJson::FastWriter writer;
		string tempjson = writer.write(json);
		CCLog("receive json == %s",tempjson.c_str());

		const CSJson::Value& value = json["result"];

		if (value["r"].asString() == "1")
		{
			const CSJson::Value& v1 = value["s"];
			m_detailMsg.m_sessionId = v1[0]["SessionId"].asInt();
			m_detailMsg.m_detail = v1[0]["Description"].asString();
			const CSJson::Value& v2 = value["sp"];
			for (int i = 0; i < v2.size(); i++)
			{
				speakerMsg speaker;
				speaker.name = crossapp_format_string("%s %s", v2[i]["FirstName"].asCString(), v2[i]["LastName"].asCString());
				speaker.title = v2[i]["Title"].asString();
				speaker.iconUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v2[i]["Icon"].asCString());
				speaker.role = v2[i]["Role"].asString();
				m_detailMsg.m_speaker.push_back(speaker);
			}
		}
		initView();
	}

#ifdef LOCALTEST
	{
		m_detailMsg.m_sessionId = 12;
		m_detailMsg.m_detail = "asdfasdfasd";
		for (int i = 0; i < 2; i++)
		{
			speakerMsg speaker;
			speaker.name = "Lisa Chen";
			speaker.title = "Custome Success";
			speaker.iconUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
			speaker.role = "Presenter";
			m_detailMsg.m_speaker.push_back(speaker);
		}
		initView();
	}

#endif
	if (p_pLoading)
	{
		p_pLoading->stopAnimating();
	}
}


void SessionDetailViewController::requestStore()
{
	if (m_canStore)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[1];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", m_isStore ? 1 : 0);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onStoreRequestFinished));

		m_canStore = false;
	}
}

void SessionDetailViewController::onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        m_isStore = !m_isStore;
        m_msg->m_stored = m_isStore;
        if (m_isStore)
        {
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
        }
        else
        {
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
        }
    }
    m_canStore = true;
}

void SessionDetailViewController::requestLike()
{
	if (m_canLike)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[2];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", 1);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onLikeRequestFinished));
	}
}

void SessionDetailViewController::onLikeRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		m_msg->m_likeNum += 1;
		m_msg->m_liked = true;
		m_canLike = false;
		m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}
	else
	{
		m_canLike = !(m_msg->m_liked);
	}
}

void SessionDetailViewController::showAlert()
{
	if (p_alertView) {
		this->getView()->removeSubview(p_alertView);
		p_alertView = NULL;
	}

	p_alertView = CAView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
	p_alertView->setColor(CAColor_clear);
	this->getView()->addSubview(p_alertView);

	CAButton* button = CAButton::create(CAButtonTypeCustom);
	button->setTag(100);
	button->setColor(CAColor_clear);
	button->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
	button->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	p_alertView->addSubview(button);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(300), m_winSize.width, _px(30)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	test->setFontSize(_px(24));
	test->setText("Network cannot connect!");
	test->setTouchEnabled(false);
	p_alertView->addSubview(test);

}