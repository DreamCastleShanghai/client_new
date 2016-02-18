
#include "SurveyViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "ConstData/ConstRect.h"
#include "ConstData/ConstId.h"
#include "SessionSurveyResultViewController.h"

SurveyViewController::SurveyViewController(int sessionId)
: p_alertView(NULL)
, p_pLoading(NULL)
, m_listView(NULL)
, m_sessionId(sessionId)
, m_question1(NULL)
, m_question2(NULL)
, m_choose1(0)
, m_choose2(0)
, m_chooseStar(0)
{
    for (int i = 0; i < STAR_CNT; i++) {
        m_stars[i] = NULL;
    }
    for (int i = 0; i < OPTN_CNT; i++) {
        m_option1[i] = NULL;
        m_option2[i] = NULL;
        m_optionBtn1[i] = NULL;
        m_optionBtn2[i] = NULL;
    }
    getSurveyInfo();
}

SurveyViewController::~SurveyViewController()
{

}

void SurveyViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    int headerHight = (120);
    int yHight = headerHight;
    // header bg
    CAScale9ImageView* header = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    header->setFrame(DRect((0), (0), m_winSize.width, headerHight));
    this->getView()->addSubview(header);
    
    // header back
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(ConstId::getBackBtnId());
    button->setAllowsSelected(true);
    this->getView()->addSubview(button);
    
    // header title
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Session Survey");
    label->setFontName(SAP_FONT_ARIAL);
    header->addSubview(label);
    
    // scroll view
    CAScrollView* scrollView = CAScrollView::createWithFrame(DRect(0, yHight, m_winSize.width, m_winSize.height));
    scrollView->setHorizontalScrollEnabled(false);
    scrollView->setVerticalScrollEnabled(true);
    scrollView->setBounceHorizontal(false);
    scrollView->setBounds(DRect(0, 0, m_winSize.width, m_winSize.height - yHight * 2));
    scrollView->setAnchorPoint(DPoint(0.f, 0.f));
    scrollView->setShowsHorizontalScrollIndicator(false);
    scrollView->setShowsVerticalScrollIndicator(true);
    this->getView()->addSubview(scrollView);

    yHight = 0;

    // part one : 4 - 1
    CAView* partOne = CAView::createWithFrame(DRect(0, yHight, m_winSize.width, m_winSize.height * 0.3));
    yHight += m_winSize.height * 0.3;
    scrollView->addSubview(partOne);
    
    // question one label
    if (m_question1 == NULL) {
        m_question1 = CALabel::createWithFrame(DRect((50), (30), m_winSize.width - (80), (60)));
        //label->setTextAlignment(CATextAlignmentCenter);
        m_question1->setColor(CAColor_gray);
        m_question1->setFontSize((28));
        m_question1->setText("Question 1");
        m_question1->setFontName(SAP_FONT_ARIAL);
        partOne->addSubview(m_question1);
    }
    
    // options one label
    for (int i = 0; i < OPTN_CNT; i++) {
        if (m_optionBtn1[i] == NULL) {
            m_optionBtn1[i] = CAButton::createWithFrame(DRect((50), (30) + (60) + (50) * i, (50), (50)), CAButtonTypeSquareRect);
            imageView = CAImageView::createWithImage(CAImage::create("common/checkbox_nochecked.png"));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
            imageView->setFrame(DRect((0), (0), (40), (40)));
            m_optionBtn1[i]->setBackgroundViewForState(CAControlStateAll, imageView);
            imageView = CAImageView::createWithImage(CAImage::create("common/checkbox_checked.png"));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
            imageView->setFrame(DRect((0), (0), (40), (40)));
            m_optionBtn1[i]->setBackgroundViewForState(CAControlStateSelected, imageView);
            m_optionBtn1[i]->setAllowsSelected(true);
            m_optionBtn1[i]->setTag(400 + i);
            m_optionBtn1[i]->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
            partOne->addSubview(m_optionBtn1[i]);
        }
        if (m_option1[i] == NULL)
        {
            m_option1[i] = CALabel::createWithFrame(DRect((50) + (60), (30) + (60) + (10) + (50) * i, m_winSize.width - (80) - (60), (60)));
            m_option1[i]->setColor(CAColor_gray);
            m_option1[i]->setFontSize((27));
            m_option1[i]->setText(crossapp_format_string("option 1 test %d", i));
            m_option1[i]->setFontName(SAP_FONT_ARIAL);
            partOne->addSubview(m_option1[i]);
        }
    }
    
    
    // part two : 4 - 1
    CAView* partTwo = CAView::createWithFrame(DRect(0, yHight, m_winSize.width, m_winSize.height * 0.3));
    yHight += m_winSize.height * 0.3;
    scrollView->addSubview(partTwo);
    
    // question two label
    if (m_question2 == NULL) {
        m_question2 = CALabel::createWithFrame(DRect((50), (30), m_winSize.width - (60), (60)));
        //label->setTextAlignment(CATextAlignmentCenter);
        m_question2->setColor(CAColor_gray);
        m_question2->setFontSize((28));
        m_question2->setText("Question 2?");
        m_question2->setFontName(SAP_FONT_ARIAL);
        partTwo->addSubview(m_question2);
    }
    
    // options two label
    for (int i = 0; i < OPTN_CNT; i++) {
        if (m_optionBtn2[i] == NULL) {
            m_optionBtn2[i] = CAButton::createWithFrame(DRect((50), (30) + (60) + (50) * i, (60), (60)), CAButtonTypeSquareRect);
            imageView = CAImageView::createWithImage(CAImage::create("common/checkbox_nochecked.png"));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
            imageView->setFrame(DRect((0), (0), (40), (40)));
            m_optionBtn2[i]->setBackgroundViewForState(CAControlStateAll, imageView);
            imageView = CAImageView::createWithImage(CAImage::create("common/checkbox_checked.png"));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
            imageView->setFrame(DRect((0), (0), (40), (40)));
            m_optionBtn2[i]->setBackgroundViewForState(CAControlStateSelected, imageView);
            m_optionBtn2[i]->setAllowsSelected(true);
            m_optionBtn2[i]->setTag(500 + i);
            m_optionBtn2[i]->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
            partTwo->addSubview(m_optionBtn2[i]);
        }
        if (m_option2[i] == NULL)
        {
            m_option2[i] = CALabel::createWithFrame(DRect((50) + (70), (30) + (60) + (10) + (50) * i, m_winSize.width - (80) - (60), (60)));
            m_option2[i]->setColor(CAColor_gray);
            m_option2[i]->setFontSize((27));
            m_option2[i]->setText(crossapp_format_string("option 2 test %d", i));
            m_option2[i]->setFontName(SAP_FONT_ARIAL);
            partTwo->addSubview(m_option2[i]);
        }
    }
   
    
    
    // part three : 5 stars
    CAView* partThree = CAView::createWithFrame(DRect(0, yHight, m_winSize.width, m_winSize.height * 0.2));
    yHight += m_winSize.height * 0.2;
    scrollView->addSubview(partThree);

//    CAView* view = CAView::createWithFrame(DRect(0, yHight, m_winSize.width, m_winSize.height - (120)));
//	this->getView()->addSubview(view);

	label = CALabel::createWithFrame(DRect((30), (30), m_winSize.width - (60), (60)));
	label->setColor(CAColor_gray);
	label->setFontSize((28));
	label->setText("How do you like this session?");
	label->setFontName(SAP_FONT_ARIAL);
	partThree->addSubview(label);

	for (int i = 0; i < STAR_CNT; i++)
	{
		button = CAButton::createWithFrame(DRect((30) + i * (100), (100), (80), (80)), CAButtonTypeSquareRect);
		imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("session/face%d_btn.png", i + 1)));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		imageView->setFrame(DRect((20), (20), (80), (80)));
		button->setBackgroundViewForState(CAControlStateAll, imageView);
        //button->setImageForState(CAControlStateAll, CAImage::create(crossapp_format_string("session/face%d_btn.png", i + 1)));
        if (m_stars[i] == NULL) {
            m_stars[i] = CAImage::create(crossapp_format_string("session/face%d_btn_pre.png", i + 1));//CAImageView::createWithImage(CAImage::create(crossapp_format_string("session/face%d_btn_pre.png", i + 1)));
            //m_stars[i]->setFrame(DRect((20), (20), (80), (80)));
            //m_stars[i]->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
       }
		//imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("session/face%d_btn_pre.png", i + 1)));
        imageView = CAImageView::createWithImage(m_stars[i]);
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		imageView->setFrame(DRect((20), (20), (80), (80)));
		button->setBackgroundViewForState(CAControlStateSelected, imageView);
		button->setAllowsSelected(true);
		button->setTag(300 + i);
		button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
		m_scoreButtonVec.push_back(button);
		partThree->addSubview(button);
	}

    /*
	label = CALabel::createWithFrame(DRect((40), (240), m_winSize.width - (80), (80)));
	//label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_gray);
	label->setFontSize((35));
	label->setText("Fadeback to the speaker:");
	label->setFontName("fonts/arial.ttf");
	view->addSubview(label);
*/
    /*
	m_feedBackTextView = CATextView::createWithFrame(DRect((40), (330), m_winSize.width - (80), (300)));
	m_feedBackTextView->setFontSize((35));
	m_feedBackTextView->setFontName("fonts/arial.ttf");
	m_feedBackTextView->setColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
	m_feedBackTextView->setBackgroundColor(CAColor_gray);
	view->addSubview(m_feedBackTextView);

	m_alertLabel = CALabel::createWithFrame(DRect((40), (640), m_winSize.width - (80), (40)));
	m_alertLabel->setText("*Feedback failed, please try again!");
	m_alertLabel->setColor(CAColor_red);
	m_alertLabel->setFontSize((35));
	m_alertLabel->setFontName("fonts/arial.ttf");
	m_alertLabel->setVisible(false);
	view->addSubview(m_alertLabel);
*/
	button = CAButton::createWithFrame(DRect((40), m_winSize.height - (120), m_winSize.width - (80), (100)), CAButtonTypeCustom);
	imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackgroundViewForState(CAControlStateAll, imageView);
	button->setTitleForState(CAControlStateAll, "Submit and Get Session Credits");
	button->setTitleFontSize((35));
	button->setTitleColorForState(CAControlStateAll, CAColor_white);
	button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(200);
    button->setAllowsSelected(true);
    this->getView()->addSubview(button);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SurveyViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SurveyViewController::getSurveyInfo()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = surveyInfoTag[0];
    key_value["sid"] = crossapp_format_string("%d", m_sessionId);

    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SurveyViewController::onSurveyInfoFinished));
    
    if (p_pLoading == NULL)
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
    }
    
    if (p_pLoading)
        p_pLoading->setLoadingMinTime(0.5f);

}


void SurveyViewController::requestMsg()
{
    std::map<std::string, std::string> key_value;
	key_value["tag"] = surveySubmitTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
	key_value["sid"] = crossapp_format_string("%d", m_sessionId);
    key_value["a1"] = crossapp_format_string("%d", m_choose1);
    key_value["a2"] = crossapp_format_string("%d", m_choose2);
    key_value["a3"] = crossapp_format_string("%d", m_chooseStar);
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SurveyViewController::onRequestFinished));
    if (p_pLoading == NULL)
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
    }
    if (p_pLoading)
        p_pLoading->setLoadingMinTime(0.5f);
}

void SurveyViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == ConstId::getBackBtnId())
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
                CAImageView* btnView = (CAImageView*)m_scoreButtonVec[i]->getBackgroundViewForState(CAControlStateSelected);
                btnView->setImage(m_stars[index]);
                //m_scoreButtonVec[i]->getBackgroundViewForState(CAControlStateSelected)->setImage(m_stars[index]);
                //m_scoreButtonVec[i]->setImageForState(CAControlStateSelected, m_stars[index]);
				m_scoreButtonVec[i]->setControlState(CAControlStateSelected);
			}
			else
			{
				m_scoreButtonVec[i]->setControlState(CAControlStateNormal);
			}
		}
        m_chooseStar = index + 1;
	}
    else if (btn->getTag() >= 400 && btn->getTag() < 405)
    {
        int index = btn->getTag() - 400;
        for (int i = 0; i < OPTN_CNT; i++)
        {
            if (i == index)
            {
                m_optionBtn1[i]->setControlState(CAControlStateSelected);
                m_choose1 = index + 1;
            }
            else
            {
                m_optionBtn1[i]->setControlState(CAControlStateNormal);
            }
        }
    }
    else if (btn->getTag() >= 500 && btn->getTag() < 505)
    {
        int index = btn->getTag() - 500;
        for (int i = 0; i < OPTN_CNT; i++)
        {
            if (i == index)
            {
                m_optionBtn2[i]->setControlState(CAControlStateSelected);
                m_choose2 = index + 1;
            }
            else
            {
                m_optionBtn2[i]->setControlState(CAControlStateNormal);
            }
        }
    }
}

void SurveyViewController::onSurveyInfoFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    bool isSucceed = false;
    CSJson::FastWriter writer;
    string tempjson = writer.write(json);
    CCLog("survey info receive json == %s", tempjson.c_str());
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& resTag = json["result"]["i"];
        bool resRes = json["result"]["r"].asBool();
        
        if (resTag == surveyInfoTag[0] && resRes)
        {
            const CSJson::Value& value = json["result"]["q"];
            
            m_surveyDetail.m_question1 = value[0]["QContent1"].asString();
            m_surveyDetail.m_question2 = value[0]["QContent2"].asString();
            
            m_surveyDetail.m_question_option1.push_back(value[0]["Q11"].asString());
            m_surveyDetail.m_question_option1.push_back(value[0]["Q12"].asString());
            m_surveyDetail.m_question_option1.push_back(value[0]["Q13"].asString());
            m_surveyDetail.m_question_option1.push_back(value[0]["Q14"].asString());
            
            m_surveyDetail.m_question_option2.push_back(value[0]["Q21"].asString());
            m_surveyDetail.m_question_option2.push_back(value[0]["Q22"].asString());
            m_surveyDetail.m_question_option2.push_back(value[0]["Q23"].asString());
            m_surveyDetail.m_question_option2.push_back(value[0]["Q24"].asString());
            
            isSucceed = true;
            
            if (m_question1)    m_question1->setText(m_surveyDetail.m_question1);
            if (m_question2)    m_question2->setText(m_surveyDetail.m_question2);
            for (int i = 0; i < OPTN_CNT; i++) {
                if (m_option1[i])    m_option1[i]->setText(m_surveyDetail.m_question_option1[i]);
                if (m_option2[i])    m_option2[i]->setText(m_surveyDetail.m_question_option2[i]);
            }
        }
    }
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    if (!isSucceed) {
        CAAlertView *alertView = CAAlertView::createWithText("Error", "Please try again!", "OK", NULL);
        alertView->setTarget(this, CAAlertView_selector(SurveyViewController::onAlert));
        alertView->show();
    }
}

void SurveyViewController::onAlert(int bid)
{
    RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
}

void SurveyViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        const CSJson::Value& value = json["result"];
        SessionSurveyResultViewController* srv = new SessionSurveyResultViewController();
        srv->setAddedPoint(value["add"].asInt());
        srv->setRank(value["rank"].asInt() + 1);
        srv->setPoint(value["points"].asInt());
        if (value["r"] == 1)
        {
            //            CAAlertView *alertView = CAAlertView::createWithText("Succeed !", "Thanks for take this survey !", "OK", NULL);
            //            alertView->show();
            FDataManager::getInstance()->setRankDirty(true);
            //FDataManager::getInstance()->setUserDirty(true);
            //            userInfo* info = FDataManager::getInstance()->getUserInfo();
            //            info->m_greenAmb = true;
            srv->setCorrect(true);
        }
        else
        {
            //            CAAlertView *alertView = CAAlertView::createWithText("Sorry !", "You have taken this survey !", "OK", NULL);
            //            alertView->show();
            srv->setCorrect(false);
        }
        srv->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(srv, true);

        /*
        const CSJson::Value& value = json["result"];
        int r = value["r"].asInt();
        string tag = value["i"].asString();
        
        if (!std::strcmp(tag.c_str(), surveySubmitTag[0])) {
            if (r == 1) {
                FDataManager::getInstance()->setUserDirty(true);

                // succeed
                CAAlertView *alertView = CAAlertView::createWithText("Succeed !", "Thank you take this survey !", "OK", NULL);
                alertView->show();
            } else if (r == 0) {
                // failed
                CAAlertView *alertView = CAAlertView::createWithText("Fail !", "You have taken this survey !", "OK", NULL);
                alertView->show();
            }
        } else {
            CAAlertView *alertView = CAAlertView::createWithText("Message error !", "Please try again !", "OK", NULL);
            alertView->show();
        }
*/
    }
    else
    {
		m_alertLabel->setText("Feedback success! ");
		m_alertLabel->setColor(CAColor_gray);
		m_alertLabel->setVisible(true);
        //showAlert();
    }
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}
