
#include "MainViewController.h"
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"
#include "time.h"
#include "SessionsViewController.h"
#include "LoginViewController.h"
#include "SessionDetailViewController.h"
#include "MyStatusViewController.h"
//#include "PrizeViewController.h"
#include "MapViewController.h"
#include "VoteViewController.h"
#include "PhotoViewController.h"
#include "NoticeViewController.h"
#include "FDataManager.h"
#include "FServerTime.h"
#include "MomentViewController.h"
#include "FirstSurveyViewController.h"
#include "MeInfoViewController.h"

#define REFRESH_STEP 5

//#define LOCALTEST

MainViewController::MainViewController()
: m_msg(FDataManager::getInstance()->getSessionMsgs())
, m_timeNoticeImageView(NULL)
, m_sessionNoticeImageView(NULL)
//, m_totalView(NULL)
//, m_totalViewList(NULL)
, m_msgTableView(NULL)
, m_pageView(NULL)
, m_pageControl(NULL)
, m_pageControlBG(NULL)
//, m_pageViewTitle(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
//, m_pastSection(0)
//, m_nextSection(1)
, m_timeForPageView(getTimeSecond())
, m_sustainabilitySurvey(NULL)
, m_monent(NULL)
, m_vote(NULL)
, m_map(NULL)
, m_session(NULL)
, m_my(NULL)
, m_pageStatus(MAP_ALL_MAP)
{
    m_filterMsg.clear();
    m_page.clear();
    m_pageAllMapVec.clear();
//    m_msg(FDataManager::getInstance()->getSessionMsgs())
}

MainViewController::~MainViewController()
{

}

void MainViewController::update(float dt)
{
    CAViewController::update(dt);
}

void MainViewController::updatePageView(float dt)
{
    if (m_pageView) {
        if (checkPageStatusIsChanged()) {
            adjustPageViewContent();
        }
        int pageCnt = m_pageView->getPageCount();
        if (pageCnt > 1) {
            int nowPage = m_pageView->getCurrPage();
            nowPage = (nowPage + 1) % pageCnt;
            if (nowPage == 0) {
                m_pageView->setCurrPage(nowPage, false);
            }
            m_pageView->setCurrPage(nowPage, true);
            if  (m_pageControl)
            {
                m_pageControl->setCurrentPage(nowPage);
                m_pageControl->updateCurrentPageDisplay();
            }
        }
    }
}

void MainViewController::refreshTable()
{
    m_filterMsg.clear();
    for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
    {
        if(it->m_stored && it->m_endTime > getTimeSecond())
        {
            m_filterMsg.push_back(&(*it));
        }
        if (m_filterMsg.size() == REFRESH_STEP)
        {
            break;
        }
    }
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
    else
    {
//        initMsgTableView();
    }
}

void MainViewController::viewDidDisappear()
{

}

void MainViewController::viewDidAppear()
{
    
    /*
    if (!m_msg->empty())
    {
        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored && it->m_endTime > getTimeSecond())
            {
                m_filterMsg.push_back(&(*it));
            }
            if (m_filterMsg.size() == REFRESH_STEP)
            {
                break;
            }
        }
        if (m_msgTableView)
        {
            m_msgTableView->reloadData();
        }
        else
        {
            initMsgTableView();
        }
    }*/
}

void MainViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    // header
    CAScale9ImageView* header = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    if (header) {
        header->setFrame(DRect((0), (0), m_winSize.width, (120)));
        header->setTouchEnabled(false);
        this->getView()->addSubview(header);
        
        // title lable
        CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (80), m_winSize.width, (50)));
        if (label) {
            label->setTextAlignment(CATextAlignmentCenter);
            label->setColor(CAColor_white);
            label->setFontSize(SAP_TITLE_FONT_SIZE);
            label->setText("SAP d-kom");
            label->setFontName(SAP_FONT_ARIAL);
            label->setTouchEnabled(false);
            header->addSubview(label);
        }
    }

    // left notification button
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    if (button) {
        button->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
        button->setTag(20);
        this->getView()->addSubview(button);
        
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("main/nav_notification.png"));
        if (imageView) {
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            button->setBackGroundViewForState(CAControlStateAll, imageView);
        }
        
        m_timeNoticeImageView = CAImageView::createWithFrame(DRect((60), (30), (10), (10)));
        if (m_timeNoticeImageView) {
            m_timeNoticeImageView->setImage(CAImage::create("common/reddot.png"));
            button->addSubview(m_timeNoticeImageView);
        }
    }
    
    // notification alert point

    // right survey button
	button = CAButton::createWithFrame(DRect(m_winSize.width - (100), (20), (100), (100)), CAButtonTypeCustom);
    if (button) {
        button->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
        button->setTag(30);
        this->getView()->addSubview(button);
        
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("main/nav_survey.png"));
        if (imageView) {
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            button->setBackgroundViewForState(CAControlStateAll, imageView);
        }
    }
	
    int pageViewHeight = m_winSize.height * 0.3;
    int butViewHeight = m_winSize.height * 0.15;
    int headerHeight = (120);
    int buttitleHeight = (40);
    
    /*
    // Page view
    if (!m_pageView)
    {
        m_pageView = CAPageView::createWithFrame(DRect(0, headerHeight, m_winSize.width, pageViewHeight), CAPageViewDirectionHorizontal);
        if (m_pageView) {
            m_pageView->setPageViewDelegate(this);
            m_pageView->setTouchEnabled(true);
            
             CAVector<CAView* > viewList;
             for (int i = 0; i<m_page.size(); i++)
             {
             CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(0, (0), m_winSize.width, pageViewHeight - headerHeight));
             temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
             temImage->setImage(CAImage::create("common/bg.png"));
             temImage->setUrl(m_page[i].m_imageUrl);
             temImage->setTouchEnabled(false);
             viewList.pushBack(temImage);
             }
             m_pageView->setViews(viewList);
             m_pageView->setCurrPage(0, false);
             this->getView()->addSubview(m_pageView);
             //headView->addSubview(m_pageView);
        }
    }
    
    // picture dots bg in page view
    m_pageControlBG = CAView::createWithColor(ccc4(0, 0, 0, 20));
    m_pageControlBG->setFrame(DRect(0, pageViewHeight + headerHeight - (50), m_winSize.width, (50)));
    m_pageControlBG->setTouchEnabled(false);
    this->getView()->addSubview(m_pageControlBG);
    //headView->addSubview(bg);
    
    // picture dots
    m_pageControl = CAPageControl::createWithCenter(DRect(m_winSize.width / 2, pageViewHeight + headerHeight - (25), (120), (50)));
    if (m_pageControl) {
        m_pageControl->setNumberOfPages((int)m_page.size());
        m_pageControl->setTag(200);
        m_pageControl->addTarget(this, CAControl_selector(MainViewController::buttonCallBack));
        this->getView()->addSubview(m_pageControl);
    }
    //headView->addSubview(m_pageControl);
     */
    
    int buttonHight = (150);//m_winSize.height * 0.15;//(116);
    // three center button
    for (int i = 0; i < 3; i++)
    {
        DRect r(i * (m_winSize.width / 3), headerHeight + pageViewHeight, m_winSize.width / 3, buttonHight);
        CAButton* btn = CAButton::createWithFrame(r, CAButtonTypeCustom);
        btn->setTag(300 + i);
        CAImageView* imageView = CAImageView::createWithCenter(DRect(m_winSize.width / 3 / 2, (60), (60), (60)));//createWithImage(CAImage::create(crossapp_format_string("main/short_%d.png", i)));
        imageView->setImage(CAImage::create(crossapp_format_string("main/short_%d.png", i)));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        //imageView->setFrame(DRect(m_winSize.width / 3 / 2, (30), (60), (60)));
        imageView->setTouchEnabled(false);
        btn->addSubview(imageView);
        CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 3 / 2, (140), m_winSize.width / 3, (60)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_gray);
        label->setFontSize((28));
        label->setText(unicode_to_utf8(mainShort[i]));
        label->setFontName(SAP_FONT_ARIAL);
        label->setTouchEnabled(false);
        btn->addSubview(label);
        btn->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
        this->getView()->addSubview(btn);
        //headView->addSubview(btn);
    }
    
    /*
     if (m_page.size() > 0)
     {
     m_pageViewTitle = CALabel::createWithFrame(DRect(20, m_winSize.width / 2 - 40, m_winSize.width - 50, 50));
     m_pageViewTitle->setText(m_page[0].m_title);
     m_pageViewTitle->setColor(CAColor_white);
     m_pageViewTitle->setFontSize((28));
     m_pageViewTitle->setScrollEnabled(false);
     m_pageViewTitle->setTouchEnabled(false);
     this->getView()->addSubview(m_pageViewTitle);
     //headView->addSubview(m_pageViewTitle);
     }
     */
    
    // sub title bar left picture
    CAView *subViewleft = CAView::createWithCenter(DRect((100), headerHeight + pageViewHeight + buttonHight + (20), m_winSize.width / 4, (20)));
    //CAView *subViewleft = CAView::createWithFrame(DRect(0, 0, m_winSize.width, (20)));
    CAImageView* subbarView1 = CAImageView::createWithImage(CAImage::create("main/home_bar.png"));
    subbarView1->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    subbarView1->setFrame(DRect(0, 0, m_winSize.width / 4, (20)));
    subbarView1->setTouchEnabled(false);
    subViewleft->addSubview(subbarView1);
    this->getView()->addSubview(subViewleft);

    // sub title
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, headerHeight + pageViewHeight + buttonHight + (30), m_winSize.width / 2, (45)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_gray);//ccc4(0x0f, 0xaa, 0xff, 0xff));//CAColor_gray);//CAColor_blue);
    label->setFontSize((27));
    label->setText("My Agenda");
    label->setFontName(SAP_FONT_ARIAL);//markerfelt.ttf");
    label->setTouchEnabled(false);
    this->getView()->addSubview(label);
    
    // sub title bar right picture
    CAView *subViewright = CAView::createWithCenter(DRect(m_winSize.width - (100), headerHeight + pageViewHeight +buttonHight + (20), m_winSize.width / 4, (20)));
    CAImageView* subbarView2 = CAImageView::createWithImage(CAImage::create("main/home_bar.png"));
    subbarView2->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    subbarView2->setFrame(DRect(0, 0, m_winSize.width / 4, (20)));
    subbarView2->setTouchEnabled(false);
    subViewright->addSubview(subbarView2);
    this->getView()->addSubview(subViewright);

    //CAView* headView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.width * 0.6 - (120)));
    //m_msgTableView->setTableHeaderView(headView);
    //m_msgTableView->setTableHeaderHeight(m_winSize.width / 2 + (130));
    
    int tableStartY = pageViewHeight + butViewHeight + headerHeight + buttitleHeight + (15);
    m_msgTableView = CATableView::createWithFrame(DRect(0, tableStartY, m_winSize.width, m_winSize.height - tableStartY));
    if (m_msgTableView) {
        m_msgTableView->setTableViewDataSource(this);
        m_msgTableView->setTableViewDelegate(this);
        m_msgTableView->setScrollViewDelegate(this);
        m_msgTableView->setAllowsSelection(true);
        m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
        //m_msgTableView->setSeparatorViewHeight((2));
        this->getView()->addSubview(m_msgTableView);
        
        CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
        if (refreshDiscount) {
            refreshDiscount->setLabelColor(CAColor_black);
            m_msgTableView->setFooterRefreshView(refreshDiscount);
        }
//        CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
//        if (refreshDiscount1) {
//            refreshDiscount1->setLabelColor(CAColor_black);
//            m_msgTableView->setHeaderRefreshView(refreshDiscount1);
//        }
    }
    
    if (m_msg->empty())
    {
        requestSessionMsg();
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
//        p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
    }
    else
    {
    //    this->initMsgTableView();
    }
}

void MainViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void MainViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    /*
    if (m_msg->size() - m_filterMsg.size() > 0)
    {
        int count = 0;
        for (int i = (int)m_msg->size() - 1; i >= 0; i--)
        {
            if(!(m_msg->at(i).m_stored))
                continue;
            if (m_filterMsg.size() == 0)
            {
                m_filterMsg.insert(m_filterMsg.begin(), &(m_msg->at(i)));
                count++;
                continue;
            }
            bool flag = false;
            for (int j = 0; j < m_filterMsg.size(); j++)
            {
                if (m_msg->at(i).m_sessionId == m_filterMsg[j]->m_sessionId)
                    flag = true;
            }
            if (flag)
                continue;
            if (m_msg->at(i).m_startTime <= m_filterMsg[0]->m_startTime)
            {
                m_filterMsg.insert(m_filterMsg.begin(), &(m_msg->at(i)));
                count++;
            }
            if (count == REFRESH_STEP)
            {
                break;
            }
        }
        
    }
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
     */
    refreshTable();
}

void MainViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    if (m_msg->size() - m_filterMsg.size() > 0 && m_filterMsg.size() > 0)
    {
        int count = 0;
        for (int i = 0; i < (int)m_msg->size() - 1; i++)
        {
            if(!(m_msg->at(i).m_stored))
                continue;
            if (m_filterMsg.size() == 0)
            {
                m_filterMsg.push_back(&(m_msg->at(i)));
                count++;
                continue;
            }
            
            bool flag = false;
            for (int j = 0; j < m_filterMsg.size(); j++)
            {
                if (m_msg->at(i).m_sessionId == m_filterMsg[j]->m_sessionId)
                    flag = true;
            }
            if (flag)
                continue;
            if (m_msg->at(i).m_startTime >= m_filterMsg[m_filterMsg.size() - 1]->m_startTime)
            {
                m_filterMsg.push_back(&(m_msg->at(i)));
                count++;
            }
            if (count == REFRESH_STEP)
            {
                break;
            }
        }
    }
    CAScheduler::schedule(schedule_selector(MainViewController::reloadTableData), this, 0.1, 0, 0.5f, false);
}

void MainViewController::reloadTableData()
{
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}
/*
void MainViewController::initMsgTableView()
{
    if (m_msg->empty())
    {
        showAlert();
        return;
    }
    if (m_msgTableView != NULL)
    {
        this->getView()->removeSubview(m_msgTableView);
        m_msgTableView = NULL;
    }
//    m_pastSection = 0;
//    m_nextSection = 1;
    
    
//    initPageView();
}
 */

/*
void MainViewController::initPageView()
{
}
*/
void MainViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (220)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect((0), (0), m_winSize.width, m_winSize.height - (220)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    bg->setTouchEnabled(false);
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect((0), (0), m_winSize.width, m_winSize.height - (220)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithFrame(DRect(0, m_winSize.height - (300), m_winSize.width, (40)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize((30));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);
    
}

void MainViewController::requestSessionMsg()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
	key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //CCLog("requestSessionMsg %d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
}

/*
void MainViewController::requestIconMsg()
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = iconUploadTag[0];
	key_value["uid"] = crossapp_format_string("%d", 25595);
    key_value["ptype"] = "png";
    CCLog("requestSessionMsg %d", 25595); //FDataManager::getInstance()->getUserId()
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_postFile(httpUrl, key_value, "/Users/csh/Documents/developments/CrossApp-master/samples/SapSession/Resources/common/my_status_top.png", this, CommonHttpJson_selector(MainViewController::onRequestFinished));
}
 */

void MainViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        //CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        int length = value["bar"].size();
        m_msg->clear();
        m_filterMsg.clear();
        m_page.clear();
        m_pageAllMapVec.clear();
//        FDataManager::getInstance()->setDiffServerTime(value["stime"].asInt64());
        for (int i = 0; i < length; i++) {
            newsPage temp_page;
			//temp_page.m_type = value["bar"][i]["ResType"].asString();
			temp_page.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["bar"][i]["Resource"].asCString());
            temp_page.m_lable = value["bar"][i]["ResLable"].asString();

            CCLog("m_imageUrl== %s",temp_page.m_imageUrl .c_str());
            //temp_page.m_titleId = value["bar"][i]["tid"].asInt();
            //temp_page.m_title = value["bar"][i]["til"].asString();
            m_pageAllMapVec.push_back(temp_page);
            m_page.push_back(temp_page);
        }
        
        initPageView();
        /*
//        if (m_pageView)
//        {
            int pageViewHeight = m_winSize.height * 0.3;
            int headerHeight = (120);
//            m_pageView = CAPageView::createWithFrame(DRect(0, headerHeight, m_winSize.width, pageViewHeight), CAPageViewDirectionHorizontal);
            if (m_pageView) {
                CAVector<CAView* > viewList;
                for (int i = 0; i<m_page.size(); i++)
                {
                    CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(0, (0), m_winSize.width, pageViewHeight - headerHeight));
                    temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
                    temImage->setImage(CAImage::create("common/bg.png"));
                    temImage->setUrl(m_page[i].m_imageUrl);
                    temImage->setTouchEnabled(false);
                    viewList.pushBack(temImage);
                }
                m_pageView->setViews(viewList);
                m_pageView->setCurrPage(0, false);
                if (m_pageControl) {
                    m_pageControl->setNumberOfPages((int)m_page.size());
                }
                
                // to : animate the page view
                CAScheduler::schedule(schedule_selector(MainViewController::updatePageView), this, 5);//, true, 3000);
            }
//        }
         */
        
        const CSJson::Value& v1 = json["result"]["sel"];
        length = v1.size();
        for (int index = 0; index < length; index++)
        {
            sessionMsg temp_msg;
            temp_msg.m_sessionId = v1[index]["SessionId"].asInt();
            temp_msg.m_title = v1[index]["Title"].asString();
            temp_msg.m_location = v1[index]["Location"].asString();
            temp_msg.m_track = v1[index]["Track"].asString();
            temp_msg.m_format = v1[index]["Format"].asString();
            temp_msg.m_startTime = v1[index]["StartTime"].asInt64();
			temp_msg.m_endTime = v1[index]["EndTime"].asInt64();
            temp_msg.m_likeNum = v1[index]["LikeCnt"].asInt();
            temp_msg.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), v1[index]["Logo"].asCString());
            temp_msg.m_stored = v1[index]["CollectionFlag"].asBool();
			temp_msg.m_liked = v1[index]["LikeFlag"].asBool();
            temp_msg.m_done = v1[index]["Done"].asBool();
            temp_msg.m_point = v1[index]["Point"].asInt();
            m_msg->push_back(temp_msg);
        }
        const CSJson::Value& v2 = json["result"]["usr"];
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
		uInfo->m_loginName = v2["LoginName"].asString();
        uInfo->m_userId = FDataManager::getInstance()->getUserId();
        uInfo->m_userName = crossapp_format_string("%s %s", v2["FirstName"].asString().c_str(), v2["LastName"].asString().c_str());
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
        /*
        quickSort(m_msg, 0, (int)m_msg->size() - 1);
        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored && it->m_endTime > getTimeSecond())
            {
                m_filterMsg.push_back(&(*it));
            }
            if (m_filterMsg.size() == REFRESH_STEP)
            {
                break;
            }
        }
         */
        refreshTable();
    }
    else
    {
        //showAlert();
    }
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

void MainViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 20) // note
	{
        NoticeViewController* vc = new NoticeViewController();
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
	}
	else if (btn->getTag() == 30) // prize
	{
        goDKomSurvey();
        /*
        m_sustainbilitySurvey = new FirstSurveyViewController();
        m_sustainbilitySurvey->init();
        m_sustainbilitySurvey->autorelease();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_sustainbilitySurvey, true);
         */
	}
	else if (btn->getTag() == 100)
    {
        requestSessionMsg();
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
//        p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
    }
    /*
    else if (btn->getTag() == 200)
    {
        CAPageControl* button = (CAPageControl*)btn;
        m_pageView->setCurrPage(button->getCurrentPage(), true);
        if (m_page.size()>0) {
            m_pageViewTitle->setText(m_page[button->getCurrentPage()].m_title);
        }
    }
     */
    else if (btn->getTag() == 300)
    {
        goUploadPhoto();
        /*
        m_monent = (MomentViewController*)new MomentViewController();
        m_monent->init();
        m_monent->autorelease();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_monent, true);
         */
    }
    else if (btn->getTag() == 301)
    {
        goDemoJam();
        /*
        m_vote = (VoteViewController*)new VoteViewController();
        m_vote->init();
        m_vote->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_vote, true);
        */
    }
    else if (btn->getTag() == 302)
    {
        m_map = (MapViewController*)new MapViewController();
        m_map->init();
        m_map->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_map, true);
    }
    else if (btn->getTag() == 400)
    {

    }
    /*
    else if (btn->getTag() == 401)
    {
        m_session = (SessionsViewController*)new SessionsViewController();
        m_session->init();
        m_session->autorelease();
//		SessionsViewController* vc = new SessionsViewController();
//		vc->init();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_session, true);
    }
    */
    else if (btn->getTag() == 402)
    {
        m_my = (MyStatusViewController*)new MyStatusViewController();
        m_my->init();
        m_my->autorelease();
//        MyStatusViewController* vc = new MyStatusViewController();
//        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_my, true);
    }
    
}

CATableViewCell* MainViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    MainViewTableCell* cell = NULL;//dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("home", DRect(0, 0, _size.width, _size.height));
		cell->initWithCell(*m_filterMsg[row]);
        cell->autorelease();
    }
    
    return cell;
}

unsigned int MainViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int MainViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return (int)m_filterMsg.size();
}

unsigned int MainViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return (240);
}

void MainViewController::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void MainViewController::pageViewDidEndTurning(CAPageView* pageView)
{
    m_pageControl->setCurrentPage(pageView->getCurrPage());
    m_pageControl->updateCurrentPageDisplay();
//    if (m_page.size() > 0) {
//        m_pageViewTitle->setText(m_page[pageView->getCurrPage()].m_title);
//    }
}

void MainViewController::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    CCLog("click %d", index);
    if (index < m_page.size()) {
        std::string tag = m_page.at(index).m_lable;
        if (strcmp(tag.c_str(), "ds") == 0)
        {
            goDKomSurvey();
        }
        else if (strcmp(tag.c_str(), "sc") == 0)
        {
            goSustainabilityCampaign();
        }
        else if (strcmp(tag.c_str(), "dj") == 0)
        {
            goDemoJam();
        }
        else if (strcmp(tag.c_str(), "up") == 0)
        {
            goUploadPhoto();
        }
        else if (strcmp(tag.c_str(), "cpr") == 0)
        {
            goCreditPointRules();
        }
    }
}

void MainViewController::pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}

void MainViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	SessionDetailViewController* vc = new SessionDetailViewController(*m_filterMsg[row]);
    vc->init();
    vc->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MainViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

bool MainViewController::checkPageStatusIsChanged()
{
    time_t nowTime = getTimeSecond();
    struct tm * timeinfo;
    timeinfo = localtime(&nowTime);
    string timeStr = "";
    int statusShouldbe = MAP_ALL_MAP;
    if (timeinfo->tm_hour >= 6 && timeinfo->tm_hour < 9) {
        statusShouldbe = MAP_PRE_EVENT;
    }
    else if (timeinfo->tm_hour >= 9 && timeinfo->tm_hour < 13) {
        statusShouldbe = MAP_MORNING;
    }
    else if (timeinfo->tm_hour >= 13 && timeinfo->tm_hour < 18) {
        statusShouldbe = MAP_AFTERNOON;
    }
    else if (timeinfo->tm_hour >= 18 && timeinfo->tm_hour < 22) {
        statusShouldbe = MAP_EVENING;
    }
    if (m_pageStatus != statusShouldbe) {
        m_pageStatus = statusShouldbe;
        return true;
    }
    return false;
}

void MainViewController::adjustPageViewContent()
{
    m_page.clear();
    switch (m_pageStatus) {
        case MAP_PRE_EVENT:
            m_page.push_back(m_pageAllMapVec.at(0));
            m_page.push_back(m_pageAllMapVec.at(2));
            m_page.push_back(m_pageAllMapVec.at(3));
            m_page.push_back(m_pageAllMapVec.at(4));
            m_page.push_back(m_pageAllMapVec.at(7));
            break;
        case MAP_MORNING:
            m_page.push_back(m_pageAllMapVec.at(0));
            m_page.push_back(m_pageAllMapVec.at(4));
            m_page.push_back(m_pageAllMapVec.at(5));
            m_page.push_back(m_pageAllMapVec.at(6));
            m_page.push_back(m_pageAllMapVec.at(7));
            break;
        case MAP_AFTERNOON:
            m_page.push_back(m_pageAllMapVec.at(1));
            m_page.push_back(m_pageAllMapVec.at(3));
            m_page.push_back(m_pageAllMapVec.at(5));
            m_page.push_back(m_pageAllMapVec.at(2));
            m_page.push_back(m_pageAllMapVec.at(7));
            break;
        case MAP_EVENING:
            m_page.push_back(m_pageAllMapVec.at(1));
            m_page.push_back(m_pageAllMapVec.at(3));
            m_page.push_back(m_pageAllMapVec.at(2));
            m_page.push_back(m_pageAllMapVec.at(6));
            m_page.push_back(m_pageAllMapVec.at(7));
            break;
        default:
            m_page.push_back(m_pageAllMapVec.at(0));
            m_page.push_back(m_pageAllMapVec.at(1));
            m_page.push_back(m_pageAllMapVec.at(2));
            m_page.push_back(m_pageAllMapVec.at(3));
            m_page.push_back(m_pageAllMapVec.at(4));
            m_page.push_back(m_pageAllMapVec.at(5));
            m_page.push_back(m_pageAllMapVec.at(6));
            m_page.push_back(m_pageAllMapVec.at(7));
            break;
    }
    initPageView();
}

void MainViewController::initPageView()
{
    int pageViewHeight = m_winSize.height * 0.3;
    int headerHeight = (120);
    // Page view
    if (m_pageView == NULL)
    {
        m_pageView = CAPageView::createWithFrame(DRect(0, headerHeight, m_winSize.width, pageViewHeight), CAPageViewDirectionHorizontal);
        m_pageView->setPageViewDelegate(this);
        m_pageView->setTouchEnabled(true);
        this->getView()->addSubview(m_pageView);
    }
    
    if (m_pageView) {
        CAVector<CAView* > viewList;
        for (int i = 0; i<m_page.size(); i++)
        {
            CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(0, (0), m_winSize.width, pageViewHeight - headerHeight));
            temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            temImage->setImage(CAImage::create("common/bg.png"));
            temImage->setUrl(m_page[i].m_imageUrl);
            temImage->setTouchEnabled(true);
            temImage->setTag(500 + i);
            viewList.pushBack(temImage);
        }
        m_pageView->setViews(viewList);
        m_pageView->setCurrPage(0, false);
    }
    
    // picture dots bg in page view
    if (m_pageControlBG == NULL) {
        m_pageControlBG = CAView::createWithColor(ccc4(0, 0, 0, 20));
        m_pageControlBG->setFrame(DRect(0, pageViewHeight + headerHeight - (50), m_winSize.width, (50)));
        m_pageControlBG->setTouchEnabled(false);
        this->getView()->addSubview(m_pageControlBG);
    }
    
    // picture dots
    if (m_pageControl != NULL) {
        this->getView()->removeSubview(m_pageControl);
        m_pageControl = NULL;
    }
    m_pageControl = CAPageControl::createWithCenter(DRect(m_winSize.width / 2, pageViewHeight + headerHeight - (25), (200), (50)));
    m_pageControl->setTag(200);
    m_pageControl->setTouchEnabled(false);
    m_pageControl->addTarget(this, CAControl_selector(MainViewController::buttonCallBack));
    m_pageControl->setNumberOfPages((int)m_page.size());
    this->getView()->addSubview(m_pageControl);
    
    // to : animate the page view
    CAScheduler::schedule(schedule_selector(MainViewController::updatePageView), this, 5);//, true, 3000);
}

void MainViewController::goSustainabilityCampaign()
{
    m_sustainabilitySurvey = new FirstSurveyViewController();
    m_sustainabilitySurvey->init();
    m_sustainabilitySurvey->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_sustainabilitySurvey, true);
}

void MainViewController::goDKomSurvey()
{
    CADevice::OpenURL(DKOM_SURVEY_LINK);
}

void MainViewController::goDemoJam()
{
    m_vote = (VoteViewController*)new VoteViewController();
    m_vote->init();
    m_vote->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_vote, true);
}

void MainViewController::goUploadPhoto()
{
    m_monent = (MomentViewController*)new MomentViewController();
    m_monent->init();
    m_monent->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(m_monent, true);
}

void MainViewController::goCreditPointRules()
{
    MeInfoViewController* vc = new MeInfoViewController();
    vc->init();
    vc->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}
