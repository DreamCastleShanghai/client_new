
#include "SessionsViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "FDataManager.h"
#include "FServerTime.h"

#define REFRESH_STEP 5

SessionsViewController::SessionsViewController()
: m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_sessionNoticeImageView(NULL)
, m_listView(NULL)
, m_filterView(NULL)
, m_navType(0)
, m_navTrackType(0)
, m_navFormatType(0)
, m_timeTblId(-1)
{
    m_downView[0] = NULL;
    m_downView[1] = NULL;
    m_msgFilter.clear();
    m_filterViewVec.clear();
    m_trackButtonVec.clear();
    m_formatButtonVec.clear();
    m_msg = FDataManager::getInstance()->getSessionMsgs();
}

SessionsViewController::~SessionsViewController()
{

}

void SessionsViewController::viewDidAppear()
{
    if (!m_msg->empty())
    {
		if (m_msgTableView)
		{
			refreshTableData();
		}
		else
		{
			initMsgTableView();
		}
    }
}

void SessionsViewController::viewDidDisappear()
{
    
}

void SessionsViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(120)));
    this->getView()->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_search.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    FSegmentView* seg = FSegmentView::createWithFrame(DRect(m_winSize.width - 240, 45, 200, 50), 2);
    seg->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_time.png"));
    seg->setItemBackgroundImage(imageView, 0);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_type.png"));
    seg->setItemBackgroundImage(imageView, 1);
    seg->setTag(200, 0);
    seg->setTag(201, 1);
    this->getView()->addSubview(seg);

    m_navType = 0;
    m_navTimeType = 0;
    m_navFormatType = 0;
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(75), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Agenda");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);
    
    if (m_msg->empty())
    {
        requestMsg();
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            p_pLoading->setTargetOnCancel(this, callfunc_selector(SessionsViewController::initMsgTableView));
        }
    }
    else
    {
        initMsgTableView();
    }
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionsViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SessionsViewController::initMsgTableView()
{
    if (m_msg->empty())
    {
        showAlert();
        return;
    }
    if (m_msgTableView == NULL)
    {
        m_listView = CAListView::createWithFrame(DRect(0,_px(120),m_winSize.width,_px(60)));
        m_listView->setListViewDelegate(this);
        m_listView->setListViewDataSource(this);
        m_listView->setAllowsSelection(true);
        m_listView->setAllowsMultipleSelection(false);
        m_listView->setListViewOrientation(CAListViewOrientationHorizontal);
        m_listView->setShowsScrollIndicators(false);
        m_listView->setSeparatorColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
        m_listView->setBackgroundImage(CAImage::create("common/gray_bg.png"));
        m_listView->setTag(1);
        //m_listView->setc
        this->getView()->addSubview(m_listView);
        
        m_msgTableView = CATableView::createWithFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
        m_msgTableView->setTableViewDataSource(this);
        m_msgTableView->setTableViewDelegate(this);
        m_msgTableView->setScrollViewDelegate(this);
        m_msgTableView->setAllowsSelection(true);
        m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
        //m_msgTableView->setSeparatorViewHeight(_px(2));
        this->getView()->addSubview(m_msgTableView);
        
        CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
        refreshDiscount->setLabelColor(CAColor_black);
        m_msgTableView->setHeaderRefreshView(refreshDiscount);


		m_filterView = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, _px(60)));
		CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
		imageView->setFrame(DRect(0, 0, m_winSize.width, _px(60)));
		m_filterView->addSubview(imageView);
		this->getView()->addSubview(m_filterView);
		m_filterViewVec.push_back(m_filterView);

		for (int i = 0; i < 2; i++)
		{
			CAButton* button = CAButton::createWithFrame(DRect(i * m_winSize.width / 2, 0, m_winSize.width / 2, _px(60)), CAButtonTypeCustom);
			button->setTitleForState(CAControlStateAll, filterItem[i]);
			button->setTitleFontName(SAP_FONT_ARIAL);
            button->setTitleFontSize(_px(30));
			button->setTitleColorForState(CAControlStateAll, CAColor_gray);
			button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
			button->setTag(300 + i);
			button->setAllowsSelected(true);
			m_filterView->addSubview(button);

			m_downView[i] = CAView::createWithFrame(DRect(i * m_winSize.width / 2, _px(180), m_winSize.width / 2, _px(50) * TrackNum + _px(20)));
			CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
			imageView->setFrame(DRect(0, 0, m_winSize.width / 2, _px(50) * TrackNum + _px(20)));
			m_downView[i]->addSubview(imageView);
			this->getView()->addSubview(m_downView[i]);
			m_filterViewVec.push_back(m_downView[i]);
		}

		for (int i = 0; i < m_filterViewVec.size(); i++)
		{
			m_filterViewVec[i]->setVisible(false);
		}

		for (int i = 0; i < TrackNum; i++)
		{
			CAButton* button = CAButton::createWithFrame(DRect(_px(20), _px(50) * i, m_winSize.width / 2 - _px(40), _px(50)), CAButtonTypeCustom);
			button->setTitleForState(CAControlStateAll, trackFilterItem[i]);
			button->setTitleFontName(SAP_FONT_ARIAL);
			button->setTitleColorForState(CAControlStateAll, CAColor_gray);
            button->setTitleColorForState(CAControlStateSelected, CAColor_white);
			button->setTitleFontSize(_px(27));
			button->setAllowsSelected(true);
			//CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/white_bg.png"));
			//button->setBackgroundViewForState(CAControlStateAll, imageView);
			CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
			button->setBackgroundViewForState(CAControlStateSelected, imageView);
			button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
			button->setTag(400 + i);
			m_downView[0]->addSubview(button);
			m_trackButtonVec.push_back(button);
		}

		for (int i = 0; i < FormatNum; i++)
		{
			CAButton* button = CAButton::createWithFrame(DRect(_px(20), _px(50) * i, m_winSize.width / 2 - _px(40), _px(50)), CAButtonTypeCustom);
			button->setTitleForState(CAControlStateAll, formatFilterItem[i]);
			button->setTitleFontName(SAP_FONT_ARIAL);
			button->setTitleColorForState(CAControlStateAll, CAColor_gray);
            button->setTitleColorForState(CAControlStateSelected, CAColor_white);
			button->setTitleFontSize(_px(27));
			button->setAllowsSelected(true);
			//CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/white_bg.png"));
			//button->setBackgroundViewForState(CAControlStateAll, imageView);
			CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
			button->setBackgroundViewForState(CAControlStateSelected, imageView);
			button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
			button->setTag(500 + i);
			m_downView[1]->addSubview(button);
			m_formatButtonVec.push_back(button);
		}
    }
}

void SessionsViewController::requestMsg()
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished));
}

void SessionsViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 300)
	{
		if (btn->getControlState() == CAControlStateSelected)
		{
			CAButton* button = (CAButton*)m_filterView->getSubviewByTag(301);
			button->setControlState(CAControlStateNormal);
			m_downView[0]->setVisible(true);
		}
		else
		{
			m_downView[0]->setVisible(false);
		}
		m_downView[1]->setVisible(false);
	}
	else if (btn->getTag() == 301)
	{
		if (btn->getControlState() == CAControlStateSelected)
		{
			CAButton* button = (CAButton*)m_filterView->getSubviewByTag(300);
			button->setControlState(CAControlStateNormal);
			m_downView[1]->setVisible(true);
		}
		else
		{
			m_downView[1]->setVisible(false);
		}
		m_downView[0]->setVisible(false);
	}
	else
	{
		CAButton* button = (CAButton*)m_filterView->getSubviewByTag(300);
		button->setControlState(CAControlStateNormal);
		button = (CAButton*)m_filterView->getSubviewByTag(301);
		button->setControlState(CAControlStateNormal);
		m_downView[0]->setVisible(false);
		m_downView[1]->setVisible(false);
	}

    if(btn->getTag() == 20)
    {
        SessionsSearchViewController* vc = new SessionsSearchViewController(0);
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
        requestMsg();
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            //p_pLoading->setTargetOnCancel(this, callfunc_selector(SessionsViewController::initMsgTableView));
        }
        initMsgTableView();
    }
    else if (btn->getTag() == 200)
    {
        if(m_navType == 0) return;
        m_navType = 0;
		for (int i = 0; i < m_filterViewVec.size(); i++)
		{
			m_filterViewVec[i]->setVisible(false);
		}
        m_listView->setVisible(true);
        if (m_msgTableView) 
		{
            refreshTableByTime(m_navTimeType);
        }
        
    }
    else if (btn->getTag() == 201)
    {
        if(m_navType == 1) return;
        m_navType = 1;
        m_listView->setVisible(false);
		m_filterView->setVisible(true);
		m_downView[0]->setVisible(false);
		m_downView[0]->setVisible(false);
        if (m_msgTableView) 
		{
			refreshTableByFormat(m_navTrackType, m_navFormatType);
        }
    }
    else if (btn->getTag() >= 400 && btn->getTag() < 500)
    {
        if (btn->getControlState() == CAControlStateNormal)
        {
            m_navTrackType = 0;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
        }
        else if (btn->getControlState() == CAControlStateSelected)
        {
            for (int i = 0; i < TrackNum; i++)
            {
				if (m_trackButtonVec[i] == btn) 
					continue;
				m_trackButtonVec[i]->setControlState(CAControlStateNormal);
            }
			m_navTrackType = btn->getTag() - 400;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
        }
        
    }
	else if (btn->getTag() >= 500 && btn->getTag() < 600)
	{
		if (btn->getControlState() == CAControlStateNormal)
		{
			m_navFormatType = 0;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
		else if (btn->getControlState() == CAControlStateSelected)
		{
			for (int i = 0; i < TrackNum; i++)
			{
				if (m_formatButtonVec[i] == btn)
					continue;
				m_formatButtonVec[i]->setControlState(CAControlStateNormal);
			}
			m_navFormatType = btn->getTag() - 500;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
	}
}

void SessionsViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed && !json.empty())
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        //CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
		FDataManager::getInstance()->setDiffServerTime(value["stime"].asInt64());

        m_msg->clear();
        m_msgFilter.clear();

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
    }
	else
	{
		//showAlert();
	}
#ifdef LOCALTEST
    {
		m_msg->clear();

		for (int i = 0; i < 17; i++)
		{
			sessionMsg temp_msg;
			temp_msg.m_sessionId = 200 + i;
			temp_msg.m_title = "Customer Success";

			temp_msg.m_location = "Lisa Chen";
			temp_msg.m_track = "Customer";
			temp_msg.m_format = "Dev Faire";
			temp_msg.m_startTime = getTimeSecond();
			temp_msg.m_endTime = temp_msg.m_startTime + 3900;
			temp_msg.m_likeNum = 20;
			temp_msg.m_stored = (bool)(rand() % 2);
			temp_msg.m_imageUrl =
				"http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
			//"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
			temp_msg.m_liked = (bool)(rand() % 2);
			temp_msg.m_done = (bool)(rand() % 2);
			temp_msg.m_point = 22;
			m_msg->push_back(temp_msg);

			userInfo uInfo;
			uInfo.m_userId = 101;
			uInfo.m_userName = "Alex Chen";
			uInfo.m_point = 100;
			uInfo.m_pointRank = 20;
			uInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
			FDataManager::getInstance()->setUserInfo(uInfo);
		}
		quickSort(m_msg, 0, (int)m_msg->size() - 1);
    }
#endif

	refreshTableData();

    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    
}

void SessionsViewController::refreshTableData()
{
	if (m_navType == 0)
	{
		if (m_msgTableView)
		{
			m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
			refreshTableByTime(m_navTimeType);
		}

	}
	else if (m_navType == 1)
	{
		if (m_msgTableView)
		{
			m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
	}
}


void SessionsViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(220)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(220)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    bg->setTouchEnabled(false);
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(220)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithFrame(DRect(0, m_winSize.height - _px(300), m_winSize.width, _px(40)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(30));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}

void SessionsViewController::refreshTableByTime(int index)
{
    
    m_msgFilter.clear();
    if (index == 0)
    {
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            m_msgFilter.push_back(&(*it));
        }
    }
    else
    {
        time_t nowTimeCount = getTimeSecond();
//        struct tm* nowTime = localtime(&nowTimeCount);
//        int nowYear = nowTime->tm_year;
//        int nowDay = nowTime->tm_yday;
//        int nowHour = nowTime->tm_hour;
//        int nowMin = nowTime->tm_min;
//        CCLog("now : %d %d %d %d", nowYear, nowDay, nowHour, nowMin);
        
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            struct tm* startTime = localtime(&(it->m_startTime));
//            int startYear = startTime->tm_year;
//            int startDay = startTime->tm_yday;
            int startHour = startTime->tm_hour;
//            int startMin = startTime->tm_min;

            struct tm* endTime = localtime(&(it->m_endTime));
//            int endYear = endTime->tm_year;
//            int endDay = endTime->tm_yday;
            int endHour = endTime->tm_hour;
//            int endMin = endTime->tm_min;
            
//            CCLog("start index %d: %d %d %d %d", index, startYear, startDay, startHour, startMin);
//            CCLog("end   index %d: %d %d %d %d", index, endYear, endDay, endHour, endMin);

            if ((startHour == index + 8) || (endHour >= index + 8 && startHour <= index + 8))
                m_msgFilter.push_back(&(*it));
        }
    }
    if (m_msgTableView)
    {
		m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
        m_msgTableView->reloadData();
    }
}

void SessionsViewController::refreshTableByFormat(int track, int format)
{
    m_msgFilter.clear();
	if (track == 0 && format == 0)
    {
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            m_msgFilter.push_back(&(*it));
        }
    }
	else if (track == 0)
	{
		for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
		{
			if (!strcmp(it->m_format.c_str(), formatFilterItem[format]))
			{
				m_msgFilter.push_back(&(*it));
			}
		}
	}
	else if (format == 0)
	{
		for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
		{
			if (!strcmp(it->m_track.c_str(), trackFilterItem[track]))
			{
				m_msgFilter.push_back(&(*it));
			}
		}
	}
    else
    {
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
			if (!strcmp(it->m_format.c_str(), formatFilterItem[format]) && !strcmp(it->m_track.c_str(), trackFilterItem[track]))
            {
                m_msgFilter.push_back(&(*it));
            }
        }
    }

    if (m_msgTableView)
    {
		m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
        m_msgTableView->reloadData();
    }
}

void SessionsViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    requestMsg();
}

void SessionsViewController::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{
	m_navTimeType = index;
    m_timeTblId = index;
	refreshTableByTime(m_navTimeType);
    CAListViewCell* cell = listView->cellForRowAtIndex(index);
    if (cell) {
        CALabel* text = (CALabel*)cell->getSubviewByTag(100);
        if (text) {
            text->setColor(CAColor_white);
        }
    }
    /*
    if (listView->getTag() == 100) {
        CAView * lable = listView->getSubviewByTag(100);
        //->setColor(ccc4(0x96, 0x96, 0x96, 255));
    }*/
}

void SessionsViewController::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{
    CAListViewCell* cell = listView->cellForRowAtIndex(index);
    if (cell) {
        CALabel* text = (CALabel*)cell->getSubviewByTag(100);
        if (text) {
            text->setColor(SAP_GRAY);
        }
    }
}

unsigned int SessionsViewController::numberOfIndex(CAListView *listView)
{
	return m_timeFilterNum;
}

unsigned int SessionsViewController::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
	return _px(120);
}

CAListViewCell* SessionsViewController::listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index)
{
    CAListViewCell* cell = NULL;
    if (listView->getTag() == 1)
    {
        DSize _size = cellSize;
        cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
        if (!cell)
        {
            cell = CAListViewCell::create("ListViewCell");
            cell->setColor(ccc4(0xEB, 0xEB, 0xEB, 0xff));
            CALabel* test = CALabel::createWithCenter(DRect(_size.width/2,
                                                            _size.height/2,
                                                            _size.width,
                                                            _size.height));
            test->setTextAlignment(CATextAlignmentCenter);
            test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            test->setFontSize(_px(28));
            test->setTag(100);
            CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
            sView->setFrame(DRect(_px(0), _px(0), _size.width, _size.height));
            cell->setBackgroundView(sView);
            cell->addSubview(test);
        }
        CALabel* test = (CALabel*)cell->getSubviewByTag(100);
        if (m_timeTblId == index)
            test->setColor(CAColor_white); // ccc4(0x96, 0x96, 0x96, 255)
        else
            test->setColor(SAP_GRAY);
        if (index == 0)
        {
            test->setText("All");
        }
        else
        {
            test->setText(crossapp_format_string("%2d:00", index + 8));
        }
    }
    else if(listView->getTag() == 2)
    {
        DSize _size = cellSize;
        cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
        if (!cell)
        {
            cell = CAListViewCell::create("ListViewCell");
            CAButton* button = CAButton::createWithFrame(DRect(_px(20), _px(20), _px(120), _px(40)), CAButtonTypeCustom);

            CAScale9ImageView* sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/seggreen_bg.png"));
            sView->setTouchEnabled(false);
            button->addSubview(sView);
            //button->setBackgroundViewForState(CAControlStateAll, sView);
            sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/seggreen_bg.png"));
            sView->setColor(ccc4(0x86, 0xBD, 0x45, 0xff));
            button->setBackgroundViewForState(CAControlStateSelected, sView);
            button->setTitleForState(CAControlStateAll, filterItem[index * 2]);
            button->setTitleColorForState(CAControlStateAll, ccc4(0xA0, 0xA0, 0xA0, 0xA0));
            button->setTitleFontSize(_px(20));
            button->setAllowsSelected(true);
            button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTag(310 + index * 2);
            //m_listButton[index * 2] = button;
            cell->addSubview(button);
            
            button = CAButton::createWithFrame(DRect(_px(20), _px(70), _px(120), _px(40)), CAButtonTypeCustom);
            sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/seggreen_bg.png"));
            sView->setTouchEnabled(false);
            button->addSubview(sView);
            //button->setBackgroundViewForState(CAControlStateAll, sView);
            //button->addSubview(sView);
            //button->setBackgroundViewForState(CAControlStateAll, sView);
            sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/btn_round.png"));
            sView->setColor(ccc4(0x86, 0xBD, 0x45, 0xff));
            button->setBackgroundViewForState(CAControlStateSelected, sView);
            button->setTitleForState(CAControlStateAll, filterItem[index * 2 + 1]);
            button->setTitleColorForState(CAControlStateAll, CAColor_gray);
            button->setTitleFontSize(_px(20));
            button->setAllowsSelected(true);
            button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTag(310 + index * 2 + 1);
            //m_listButton[index * 2 + 1] = button;
            cell->addSubview(button);
            
            //sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
            //sView->setFrame(DRect(_px(0), _px(0), _size.width, _size.height));
            //sView->setColor(CAColor_clear);
            //cell->setAlpha(0);
            //cell->setBackgroundView(sView);
                            
        }
        
    }
    return cell;
}

CATableViewCell* SessionsViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    MainViewTableCell* cell = NULL;
    if (cell == NULL)
    {
        table->dequeueReusableCellWithIdentifier(crossapp_format_string("%d", m_msgFilter[row]->m_sessionId).c_str());
		cell = MainViewTableCell::create(crossapp_format_string("%d", m_msgFilter[row]->m_sessionId).c_str(), DRect(0, 0, _size.width, _size.height)); //
		cell->initWithCell(*m_msgFilter[row]);
    }
  
    return cell;
    
}

unsigned int SessionsViewController::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int SessionsViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return (int)m_msgFilter.size();
}

unsigned int SessionsViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return _px(240);
}

void SessionsViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    SessionDetailViewController* vc = new SessionDetailViewController(*m_msgFilter.at(row));
    vc->init();
    vc->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void SessionsViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}


