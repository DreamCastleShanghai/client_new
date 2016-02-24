
#include "MyStatusViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "FServerTime.h"
#include "FDataManager.h"
#include "PhotoViewController.h"
#include "MeInfoViewController.h"

MyStatusViewController::MyStatusViewController()
: m_msgTableView(NULL)
, m_myCalanderView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_pointView(NULL)
, m_navType(0)
, m_pointType(0)
, m_navSegmentView(NULL)
, m_pointSegmentView(NULL)
, m_searchButton(NULL)
, m_pointButton(NULL)
, m_urlImageView(NULL)
, m_greenAmbIcon(NULL)
, m_nameLabel(NULL)
, m_msg(FDataManager::getInstance()->getSessionMsgs())
{
    m_filterMsg.clear();
    m_rankMsg.clear();
    m_shMsg.clear();
    m_rowNumOfSection.clear();
    m_pointLabel[0] = NULL;
    m_pointLabel[1] = NULL;
    m_rankLabel[0] = NULL;
    m_rankLabel[1] = NULL;
}

MyStatusViewController::~MyStatusViewController()
{

}

void MyStatusViewController::viewDidAppear()
{
    if (FDataManager::getInstance()->isUserDirty()) {
        FDataManager::getInstance()->setUserDirty(false);
        requestUserMsg();
    }
    if (FDataManager::getInstance()->isRankDirty()) {
        FDataManager::getInstance()->setRankDirty(false);
        switchNavType();
    }
}

void MyStatusViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    // header bg
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    if (sView) {
        sView->setFrame(DRect((0), (0), m_winSize.width, (120)));
        this->getView()->addSubview(sView);
    }
    
    // search btn in header
//    m_searchButton = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
//    if (m_searchButton) {
//        m_searchButton->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
//        m_searchButton->setTag(20);
//        this->getView()->addSubview(m_searchButton);
//        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_search.png"));
//        if (imageView) {
//            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
//            m_searchButton->setBackgroundViewForState(CAControlStateAll, imageView);
//        }
//    }
    
    // information btn
    m_pointButton = CAButton::createWithFrame(DRect(m_winSize.width - (120), (20), (100), (100)), CAButtonTypeCustom);
    if (m_pointButton) {
        m_pointButton->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_pointButton->setTag(30);
        m_pointButton->setVisible(false);
        this->getView()->addSubview(m_pointButton);
        
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_info.png"));
        if (imageView) {
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            m_pointButton->setBackgroundViewForState(CAControlStateAll, imageView);
        }
    }
    
    // my calander view
    m_navSegmentView =
        FSegmentView::createWithFrame(DRect((m_winSize.width - 400) / 2, 40, 400, 60), 2);
    if (m_navSegmentView) {
        m_navSegmentView->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_navSegmentView->setItemTile("My Agenda", 0);
        m_navSegmentView->setItemTile("Credits", 1);
        m_navSegmentView->setTag(200, 0);
        m_navSegmentView->setTag(201, 1);
        this->getView()->addSubview(m_navSegmentView);
    }
    
    // my calander table view
    if (m_myCalanderView == NULL)
    {
        m_myCalanderView = CATableView::createWithFrame(DRect(0, (120), m_winSize.width, m_winSize.height - (120)));
        m_myCalanderView->setTableViewDataSource(this);
        m_myCalanderView->setTableViewDelegate(this);
        m_myCalanderView->setScrollViewDelegate(this);
        m_myCalanderView->setAllowsSelection(true);
        m_myCalanderView->setSeparatorColor(ccc4(200, 200, 200, 80));
        //m_msgTableView->setSeparatorViewHeight((2));
        this->getView()->addSubview(m_myCalanderView);
    }
    
    // user information table
    if (m_msgTableView == NULL)
    {
        //m_msgTableView = CATableView::createWithFrame(DRect(0, (120), m_winSize.width, m_winSize.height - (120)));
        m_msgTableView = CATableView::createWithFrame(DRect(0, (420), m_winSize.width, m_winSize.height - (420)));
        m_msgTableView->setTableViewDataSource(this);
        m_msgTableView->setTableViewDelegate(this);
        m_msgTableView->setScrollViewDelegate(this);
        m_msgTableView->setAllowsSelection(true);
        m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
        //m_msgTableView->setSeparatorViewHeight((2));
        this->getView()->addSubview(m_msgTableView);
    }

    m_pointView = CAView::createWithFrame(DRect(0, (120), m_winSize.width, (300)));
    if (m_pointView) {
        m_pointView->setColor(ccc4(0, 0xa8, 0xfc, 0xff));
        m_pointView->setVisible(false);
        this->getView()->addSubview(m_pointView);
        
        // user logo
        CAButton* button = CAButton::createWithFrame(DRect((m_winSize.width - (120)) / 2, (30), (120), (120)), CAButtonTypeCustom);
        if (button) {
            button->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTag(400);
            m_pointView->addSubview(button);
            
            m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
            if (m_urlImageView) {
                m_urlImageView->setFrame(DRect(0, 0, (120), (120)));
                m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
                button->addSubview(m_urlImageView);
                
                // user green amb icon
                m_greenAmbIcon = CAImageView::createWithImage(CAImage::create("common/green_amb.png"));
                if (m_greenAmbIcon) {
                    m_greenAmbIcon->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
                    m_greenAmbIcon->setFrame(DRect(80, 80, (40), (40)));
                    m_greenAmbIcon->setVisible(false);
                    m_urlImageView->addSubview(m_greenAmbIcon);
                }
            }
        }
        
        // user name
        m_nameLabel = CALabel::createWithCenter(DRect(m_winSize.width / 2, (185), (400), (40)));
        //m_nameLabel = CALabel::createWithFrame(DRect((m_winSize.width - (200)) / 2, (170), (200), (35)));
        if (m_nameLabel) {
            m_nameLabel->setFontSize((30));
            m_nameLabel->setColor(CAColor_white);
            m_nameLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            m_nameLabel->setTextAlignment(CATextAlignmentCenter);
            m_pointView->addSubview(m_nameLabel);
        }
        
        // score history & rank list
        m_pointSegmentView = FSegmentView::createWithFrame(DRect(0, (200), m_winSize.width, (100)), 2, 1);
        m_pointSegmentView->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_pointSegmentView->setTag(300, 0);
        m_pointSegmentView->setTag(301, 1);
        m_pointView->addSubview(m_pointSegmentView);
        

        for (int i = 0; i < 2; i++)
        {
            m_pointLabel[i] = CALabel::createWithFrame(DRect(i * m_winSize.width / 2, (200), m_winSize.width / 2, (60)));
            if (m_pointLabel[i]) {
                m_pointLabel[i]->setTextAlignment(CATextAlignmentCenter);
                m_pointLabel[i]->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                m_pointLabel[i]->setFontSize((50));
                m_pointLabel[i]->setTouchEnabled(false);
                m_pointLabel[i]->setColor(CAColor_white);
                m_pointLabel[i]->setText("");
                m_pointView->addSubview(m_pointLabel[i]);
            }
            
            m_rankLabel[i] = CALabel::createWithFrame(DRect(i * m_winSize.width / 2, (260), m_winSize.width / 2, (35)));
            if (m_rankLabel[i]) {
                m_rankLabel[i]->setTextAlignment(CATextAlignmentCenter);
                m_rankLabel[i]->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                m_rankLabel[i]->setFontSize((30));
                m_rankLabel[i]->setTouchEnabled(false);
                m_rankLabel[i]->setColor(CAColor_white);
                m_rankLabel[i]->setText("");
                m_pointView->addSubview(m_rankLabel[i]);
            }
        }
        
        if (m_rankLabel[0])     m_rankLabel[0]->setText("Credits");
        if (m_rankLabel[1])     m_rankLabel[1]->setText("Ranking");
        if (m_pointLabel[0])    m_pointLabel[0]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
        if (m_rankLabel[0])     m_rankLabel[0]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
    }
    
    m_navSegmentView->setItemFocus(1);
    m_pointSegmentView->setItemFocus(1);
    m_navType = MY_INFO_VIEW;
    m_pointType = MY_INFO_RANK;
    switchNavType();
    refreshUserInfo();
    refreshTable();
    
    /*
    if (m_msg->empty())
    {
        m_canSwitchSeg = false;
//        requestMsg();
        showLoading();
    }
    else
    {
        refreshTable();
        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
        this->initMsgTableView();
    }
    */
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void MyStatusViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

/*
void MyStatusViewController::initMsgTableView()
{
    if (m_msg->empty())
    {
        showAlert();
        return;
    }
    m_canSwitchSeg = true;
 
    if (m_msgTableView == NULL)
    {
        m_msgTableView = CATableView::createWithFrame(DRect(0, (120), m_winSize.width, m_winSize.height - (120)));
        m_msgTableView->setTableViewDataSource(this);
        m_msgTableView->setTableViewDelegate(this);
        m_msgTableView->setScrollViewDelegate(this);
        m_msgTableView->setAllowsSelection(true);
        m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
        //m_msgTableView->setSeparatorViewHeight((2));
        this->getView()->addSubview(m_msgTableView);
    }
}
*/

/*
void MyStatusViewController::requestMsg()
{
    if (p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MyStatusViewController::onRequestFinished));
}
*/

void MyStatusViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if(btn->getTag() == 20)
    {
        SessionsSearchViewController* vc = new SessionsSearchViewController(1);
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if(btn->getTag() == 30)
    {
        MeInfoViewController* vc = new MeInfoViewController();
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
//        requestMsg();
    }
    else if (btn->getTag() == 200)
    {
        m_navType = MY_CALANDER_VIEW;
        switchNavType();
    }
    else if (btn->getTag() == 201)
    {
//        if (!m_canSwitchSeg)
        {
//            m_navSegmentView->resetSegment();
//            return;
        }
//        m_navType = m_pointType + 1;
        m_navType = MY_INFO_VIEW;
        switchNavType();
    }
    else if (btn->getTag() == 300)
    {
        m_pointType = MY_INFO_SCORE_HISTORY;
        //m_navType = 1;
        switchNavType();
    }
    else if (btn->getTag() == 301)
    {
//        if (!m_canSwitchPoint)
        {
//            m_pointSegmentView->resetSegment();
//            return;
        }
        m_pointType = MY_INFO_RANK;
        //m_navType = 2;
        switchNavType();
    }
	else if (btn->getTag() == 400)
	{
        PhotoViewController* vc = new PhotoViewController(0);
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
	}
}

/*
void MyStatusViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed && !json.empty())
	{
		CSJson::FastWriter writer;
		string tempjson = writer.write(json);
		//CCLog("receive json == %s",tempjson.c_str());

		const CSJson::Value& value = json["result"];
		FDataManager::getInstance()->setDiffServerTime(value["stime"].asInt64());

		m_msg->clear();
		m_filterMsg.clear();

		const CSJson::Value& v1 = json["result"]["sel"];
		int length = v1.size();
		for (int index = 0; index < length; index++)
		{
			sessionMsg temp_msg;
			temp_msg.m_sessionId = v1[index]["SessionId"].asInt();
			temp_msg.m_title = v1[index]["Title"].asString();
			temp_msg.m_location = v1[index]["Location"].asString();
			temp_msg.m_track = v1[index]["Track"].asString();
			temp_msg.m_format = v1[index]["Format"].asString();
			temp_msg.m_startTime = v1[index]["StartTime"].asInt64();
			temp_msg.m_endTime = v1[index]["EndTime"].asInt();
			temp_msg.m_likeNum = v1[index]["LikeCnt"].asInt();
			temp_msg.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v1[index]["Logo"].asCString());
			temp_msg.m_stored = v1[index]["CollectionFlag"].asBool();
			temp_msg.m_liked = v1[index]["LikeFlag"].asBool();
			temp_msg.m_done = v1[index]["Done"].asBool();
			temp_msg.m_point = v1[index]["Point"].asBool();
			m_msg->push_back(temp_msg);
		}
		const CSJson::Value& v2 = json["result"]["usr"];
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
        uInfo->m_loginName = v2["LoginName"].asString();
        uInfo->m_userId = FDataManager::getInstance()->getUserId();
        uInfo->m_userName = crossapp_format_string("%s %s", v2["LastName"].asString().c_str(), v2["FirstName"].asString().c_str());
        uInfo->m_point = v2["Score"].asInt();
        uInfo->m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v2["Icon"].asCString());
        uInfo->m_eggVoted = v2["EggVoted"].asBool();
        uInfo->m_greenAmb = v2["GreenAmb"].asBool();
        uInfo->m_demoVoteIdVec.clear();
        uInfo->m_voiceVoteIdVec.clear();
        int voteId = v2["DemoJamId1"].asInt();
        if (voteId != -1)
        {
            uInfo->m_demoVoteIdVec.push_back(voteId);
        }
        voteId = v2["DemoJamId2"].asInt();
        if (voteId != -1)
        {
            uInfo->m_demoVoteIdVec.push_back(voteId);
        }
        voteId = v2["VoiceVoteId1"].asInt();
        if (voteId != -1)
        {
            uInfo->m_voiceVoteIdVec.push_back(voteId);
        }
        voteId = v2["VoiceVoteId2"].asInt();
        if (voteId != -1)
        {
            uInfo->m_voiceVoteIdVec.push_back(voteId);
        }

		quickSort(m_msg, 0, (int)m_msg->size() - 1);

		m_filterMsg.clear();
		for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
		{
			if (it->m_stored)
			{
				m_filterMsg.push_back(&(*it));
			}
		}
	}
	else
	{
		//showAlert();
	}

//    initMsgTableView();
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    
}
 */


void MyStatusViewController::requestScoreHistoryMsg()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = scoreHistoryTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MyStatusViewController::onRequestScoreHistoryFinished));
}

void MyStatusViewController::onRequestScoreHistoryFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());

//        m_canSwitchPoint = true;
        const CSJson::Value& v2 = json["result"];
        string tag = v2["i"].asString();
        int isServerSucceed = v2["r"].asInt();
        const CSJson::Value& history = v2["h"];
        
        if (!strcmp(tag.c_str(), scoreHistoryTag[0]) && isServerSucceed == 1) {
            int length = history.size();
//            CCLog("list :%d", length);
            m_shMsg.clear();
            for (int i = 0; i < length; i++)
            {
                scoreHistory tmpInfo;
                tmpInfo.m_scoreType = history[i]["ScoreType"].asInt();
                tmpInfo.m_score = history[i]["Score"].asInt();
                tmpInfo.m_scoreDetail = history[i]["ScoreDetail"].asString();
                m_shMsg.push_back(tmpInfo);
            }
            refreshUserInfo();
        }
    }
}

void MyStatusViewController::requestRankMsg()
{
    if (p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = rankViewTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MyStatusViewController::onRequestRankFinished));
}

void MyStatusViewController::onRequestRankFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
//        m_canSwitchPoint = true;
        const CSJson::Value& v2 = json["result"]["rl"];
        int length = v2.size();
//        int points = 0;
        m_rankMsg.clear();
        for (int i = 0; i < length; i++)
        {
            userInfo tmpInfo;
            tmpInfo.m_userName = crossapp_format_string("%s %s", v2[i]["FirstName"].asString().c_str(), v2[i]["LastName"].asString().c_str());
            tmpInfo.m_pointRank = i + 1;
            tmpInfo.m_point = v2[i]["Score"].asInt();
            tmpInfo.m_greenAmb = v2[i]["GreenAmb"].asBool();
            tmpInfo.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v2[i]["Icon"].asString().c_str());
            m_rankMsg.push_back(tmpInfo);
        }
        int myRank = json["result"]["urk"].asInt() + 1;
        m_pointLabel[1]->setText(crossapp_format_string("%d", myRank));
        refreshUserInfo();
    }
    else
    {
        //showAlert();0faaff
    }
}

void MyStatusViewController::requestUserMsg()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = userInfoTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MyStatusViewController::onRequestUserMsgFinished));
    /*
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        //p_pLoading->setTargetOnCancel(this, callfunc_selector(MyStatusViewController::initMsgTableView));
    }*/
}

void MyStatusViewController::onRequestUserMsgFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& v2 = json["result"];
        std::string isSucceed = v2["r"].asString();
        if (isSucceed == "1") {
            userInfo* tmpUser = FDataManager::getInstance()->getUserInfo();
            tmpUser->m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v2["usr"][0]["Icon"].asString().c_str());;
            tmpUser->m_point = v2["usr"][0]["Score"].asInt();
//            if (m_urlImageView != NULL)
//                m_urlImageView->setUrl(tmpUser.m_imageUrl);
//            tmpUser.m_greenAmb = v2["r"]["GreenAmb"].asBool();
        }
        refreshUserInfo();
    }
    
}

void MyStatusViewController::switchNavType()
{
    if (m_navType == MY_CALANDER_VIEW)
    {
        m_myCalanderView->setVisible(true);
        //m_searchButton->setVisible(true);

        m_pointView->setVisible(false);
        m_msgTableView->setVisible(false);
        m_pointButton->setVisible(false);
        

        //refreshTable();
        /*
        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
        m_msgTableView->setFrame(DRect(0, (120), m_winSize.width, m_winSize.height - (120)));
        m_msgTableView->reloadData();
        */
    }
    else if(m_navType == MY_INFO_VIEW)
    {
 //       if (m_rankMsg.empty())
        m_pointView->setVisible(true);
        m_pointButton->setVisible(true);
        m_msgTableView->setVisible(true);
        
        //m_searchButton->setVisible(false);
        m_myCalanderView->setVisible(false);
        
        if (m_pointType == MY_INFO_SCORE_HISTORY) {
            requestScoreHistoryMsg();
            m_pointLabel[1]->setColor(SAP_WHITE_LOW);
            m_rankLabel[1]->setColor(SAP_WHITE_LOW);
            m_pointLabel[0]->setColor(CAColor_white);
            m_rankLabel[0]->setColor(CAColor_white);
            
            /*
             m_filterMsg.clear();
             for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
             {
             if(it->m_done)
             {
             m_filterMsg.push_back(&(*it));
             }
             }
             m_msgTableView->setFrame(DRect(0, (420), m_winSize.width, m_winSize.height - (420)));
             m_msgTableView->reloadData();
             */
        } else if (m_pointType == MY_INFO_RANK) {
            requestRankMsg();
            m_pointLabel[1]->setColor(CAColor_white);
            m_rankLabel[1]->setColor(CAColor_white);
            m_pointLabel[0]->setColor(SAP_WHITE_LOW);
            m_rankLabel[0]->setColor(SAP_WHITE_LOW);

            /*
            m_pointView->setVisible(true);
            m_searchButton->setVisible(false);
            m_pointButton->setVisible(true);
            m_msgTableView->setFrame(DRect(0, (420), m_winSize.width, m_winSize.height - (420)));
            m_msgTableView->reloadData();
             */
        }
    }
//    else if(index == 2)
    {
        
    }
}

void MyStatusViewController::showLoading()
{
    p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
    this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
    p_pLoading->setLoadingMinTime(0.5f);
    //p_pLoading->setTargetOnCancel(this, callfunc_selector(MyStatusViewController::initMsgTableView));
}

void MyStatusViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (120)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (120)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - (120)) / 2, m_winSize.width, m_winSize.height - (120)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize((24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}

CATableViewCell* MyStatusViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    CATableViewCell* cell = NULL;
    if (table == m_myCalanderView)
    {
        //cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp0"));
        if(!m_filterMsg.empty())
        {
            int count = 0;
            for (int i = 0; i < section; i++)
            {
                count += m_rowNumOfSection[i].rowNum;
            }
            if (m_filterMsg.size() < count + row)
            {
                return NULL;
             }
            sessionMsg* msg = m_filterMsg[count + row];
            //cell = CATableViewCell::create("CrossApp0");
            table->dequeueReusableCellWithIdentifier(crossapp_format_string("0%d", msg->m_sessionId).c_str());
            cell = MainViewTableCell::create(crossapp_format_string("0%d", msg->m_sessionId), DRect(0, 0, _size.width, _size.height));
            ((MainViewTableCell*)cell)->initWithCell(*msg);
            /*
            CALabel* label = CALabel::createWithFrame(DRect((40), (10), m_winSize.width - (40) * 2, (30)));
            label->setText(msg->m_title);
            label->setFontSize((25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);*/
        }
    }
    else if(table == m_msgTableView)
    {
        if (m_pointType == MY_INFO_SCORE_HISTORY)
        {
            if(cell == NULL)
            {
                /*
                 sessionMsg* msg = m_filterMsg[row];
                 cell = CATableViewCell::create("CrossApp1");
                 CALabel* label = CALabel::createWithFrame(DRect((40), (10), (300), (30)));
                 label->setText(crossapp_format_string("+%d", msg->m_point));
                 label->setFontSize((25));
                 label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                 label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                 cell->addSubview(label);
                 label = CALabel::createWithFrame(DRect((120), (10), m_winSize.width - (120) - (40), (30)));
                 label->setText(msg->m_title);
                 label->setFontSize((25));
                 label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                 label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                 cell->addSubview(label);*/
                //for (vector<scoreHistory>::iterator it = m_shMsg.begin(); it != m_shMsg.end(); it++) {
                if (m_shMsg.size() <= row)
                {
                    return NULL;
                }
                scoreHistory it = m_shMsg[row];
                int sessionId = atoi(it.m_scoreDetail.c_str());
                table->dequeueReusableCellWithIdentifier(crossapp_format_string("1%d", sessionId).c_str());
                cell = CATableViewCell::create(crossapp_format_string("1%d", sessionId));
                    //cell = CATableViewCell::create("history");
                    //CCLog("%d", it.m_score);
                    //CCLog("%d", it.m_scoreType);
                    //CCLog("%s", it.m_scoreDetail.c_str());
                CALabel* scorelabel = CALabel::createWithFrame(DRect((40), (20), (300), (60)));
                if (scorelabel) {
                    scorelabel->setText(crossapp_format_string("+%d", it.m_score));
                    scorelabel->setFontSize((30));
                    scorelabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                    scorelabel->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                    cell->addSubview(scorelabel);
                }
                if (it.m_scoreType == 0 || it.m_scoreType == 6)
                {
                    // show score reason
                    CALabel* typelabel = CALabel::createWithFrame(DRect((120), (10), m_winSize.width - (120) - (40), (30)));
                    if (typelabel) {
                        typelabel->setText(scoreType[it.m_scoreType]);
                        typelabel->setFontSize((25));
                        typelabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                        typelabel->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                        cell->addSubview(typelabel);
                    }
                    // show session title
                    CALabel* detaillabel = CALabel::createWithFrame(DRect((120), (60), m_winSize.width - (120) - (40), (30)));
                    if (detaillabel) {
                        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++) {
                            if (it->m_sessionId == sessionId) {
                                detaillabel->setText(it->m_title.c_str());
                                break;
                            }
                        }
                        detaillabel->setFontSize((25));
                        detaillabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                        detaillabel->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                        cell->addSubview(detaillabel);
                    }
                }
                else
                {
                    // show score reason
                    CALabel* typelabel = CALabel::createWithFrame(DRect((120), (35), m_winSize.width - (120) - (40), (30)));
                    if (typelabel) {
                        typelabel->setText(scoreType[it.m_scoreType]);
                        typelabel->setFontSize((25));
                        typelabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                        typelabel->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                        cell->addSubview(typelabel);
                    }
                }
                //}
            }
        }
        else if (m_pointType == MY_INFO_RANK)
        {
            if(cell == NULL)
            {
                table->dequeueReusableCellWithIdentifier(crossapp_format_string("2%d", m_rankMsg[row].m_userId).c_str());
                cell = CATableViewCell::create(crossapp_format_string("2%d", m_rankMsg[row].m_userId));
                //cell = CATableViewCell::create("rank");
                CommonUrlImageView* urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
                //createWithFrame(DRect((30), (40), (80), (80)));
                urlImageView->setFrame(DRect((140), (5), (40), (40)));
                urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
                //urlImageView->setImage(CAImage::create("common/bg.png"));
                urlImageView->setUrl(m_rankMsg[row].m_imageUrl);
                
                cell->addSubview(urlImageView);
                cell->setTouchEnabled(false);
                
                // green amb icon
                if (m_rankMsg[row].m_greenAmb) {
                    CAImageView* greenAmbIcon = CAImageView::createWithImage(CAImage::create("common/green_amb.png"));
                    greenAmbIcon->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
                    greenAmbIcon->setFrame(DRect(25, 25, (20), (20)));
                    greenAmbIcon->setVisible(true);
                    urlImageView->addSubview(greenAmbIcon);
                }
                
                CALabel* label = CALabel::createWithFrame(DRect((200), (10), (300), (30)));
                label->setText(m_rankMsg[row].m_userName);
                label->setFontSize((25));
                label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                cell->addSubview(label);
                
                label = CALabel::createWithFrame(DRect(m_winSize.width * 0.75, (10), (300), (30)));
                label->setText(crossapp_format_string("%d", m_rankMsg[row].m_point));
                label->setFontSize((25));
                label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                cell->addSubview(label);
                
                label = CALabel::createWithFrame(DRect((60), (10), (300), (30)));
                label->setText(crossapp_format_string("%d", m_rankMsg[row].m_pointRank));
                label->setFontSize((25));
                label->setBold(true);
                label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
                label->setColor(ccc4(0x0f, 0xaa, 0xff, 0xff));//CAColor_blue);//(ccc4(0x3f, 0x3f, 0x3f, 0xff));
                cell->addSubview(label);
            }
        }
        //cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp1"));
    }
//    else if(m_navType == 2 && m_pointType == 2)
    {
        //cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp2"));
    }
    return cell;
}

CAView* MyStatusViewController::tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = NULL;
    if (table == m_myCalanderView)
    {
        view = CAView::createWithColor(ccc4(0xf3, 0xf3, 0xf3, 0xf3));
        
        CALabel* label = CALabel::createWithFrame(DRect((40), (0), (300), (50)));
        int hour = m_rowNumOfSection[section].hour;
        label->setText(crossapp_format_string("%02d:00 - %02d:00", hour, hour + 1));
        CCLog("head %d %d", section, hour);
        label->setFontSize((25));
        label->setColor(ccc4(0, 0xa8, 0xfc, 0xff));
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        view->addSubview(label);
    }
    return view;
}

unsigned int MyStatusViewController::numberOfSections(CATableView *table)
{
    int num = 1;
    if (table == m_myCalanderView)
    {
        m_rowNumOfSection.clear();
        if (!m_filterMsg.empty())
        {
            tm* time = localtime(&m_filterMsg[0]->m_startTime);
            int tmp = time->tm_hour;
            int rowNum = 0;
            secVec sv;
            for (int i = 0; i < m_filterMsg.size(); i++)
            {
                time = localtime(&m_filterMsg[i]->m_startTime);
                if (tmp != time->tm_hour)
                {
                    sv.rowNum = rowNum;
                    sv.hour = tmp;
                    CCLog("row %d %d", rowNum, tmp);
                    m_rowNumOfSection.push_back(sv);
                    rowNum = 1;
                    num++;
                    tmp = time->tm_hour;
                }
                else
                {
                    rowNum++;
                }
                
            }
            sv.rowNum = rowNum;
            sv.hour = tmp;
            m_rowNumOfSection.push_back(sv);
        }
        else
        {
            secVec sv;
            sv.rowNum = 1;
            sv.hour = 8;
            m_rowNumOfSection.push_back(sv);
        }
    }

	return num;
}

unsigned int MyStatusViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    int num = 0;
    if (table == m_myCalanderView)
    {
        CCLog("s:%d", section);
        num = m_rowNumOfSection[section].rowNum;
    }
    else if(table == m_msgTableView)
    {
        if (m_pointType == MY_INFO_SCORE_HISTORY)
        {
            num = (int)m_shMsg.size();
        }
        else if (m_pointType == MY_INFO_RANK)
        {
            num = (int)m_rankMsg.size();
        }
        //num = (int)m_filterMsg.size();
    }
//    else if(m_navType == 2)
//    {
        //CCLog("num %d", num);
//    }
    return num;
}

unsigned int MyStatusViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    int hight = 0;
    if (table == m_myCalanderView)
    {
        hight = (50);
    }
    return hight;
}

unsigned int MyStatusViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if (table == m_myCalanderView) {
        return (240);
    }
    else if (table == m_msgTableView)
    {
        if (m_pointType == MY_INFO_SCORE_HISTORY)
        {
            return (100);
        }
        else if (m_pointType == MY_INFO_RANK)
        {
            return (50);
        }
    }
	return (50);
}

void MyStatusViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if (table == m_myCalanderView)
    {
        int count = 0;
        for (int i = 0; i < section; i++)
        {
            count += m_rowNumOfSection[i].rowNum;
        }
        SessionDetailViewController* vc = new SessionDetailViewController(*m_filterMsg[count + row]);
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (table == m_msgTableView)
    {
        if (m_pointType == MY_INFO_SCORE_HISTORY)
        {
            scoreHistory it = m_shMsg[row];
            int sessionId = atoi(it.m_scoreDetail.c_str());
            for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++) {
                if (it->m_sessionId == sessionId) {
                    SessionDetailViewController* vc = new SessionDetailViewController(*it);
                    vc->init();
                    vc->autorelease();
                    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
                    break;
                }
            }
        }
        else if (m_pointType == MY_INFO_RANK)
        {
        }
    }
}

void MyStatusViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}

void MyStatusViewController::refreshTable()
{
    m_filterMsg.clear();
    for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
    {
        if (it->m_stored)
        {
            m_filterMsg.push_back(&(*it));
        }
    }
    if (m_myCalanderView) {
        m_myCalanderView->reloadData();
    }
}

void MyStatusViewController::refreshUserInfo()
{
    userInfo* info = FDataManager::getInstance()->getUserInfo();
    if (info->m_userId > 0)
    {
        CCLog("%s", info->m_imageUrl.c_str());
        
        m_urlImageView->setUrl(info->m_imageUrl);
        m_nameLabel->setText(info->m_userName);
        m_pointLabel[0]->setText(crossapp_format_string("%d", info->m_point));
        if (info->m_greenAmb)
            m_greenAmbIcon->setVisible(true);
    }
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}
