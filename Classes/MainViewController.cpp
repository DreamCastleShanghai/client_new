
#include "MainViewController.h"
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"
#include "time.h"
#include "SessionsViewController.h"
#include "LoginViewController.h"
#include "SessionDetailViewController.h"
#include "MyStatusViewController.h"
#include "PrizeViewController.h"
#include "MapViewController.h"
#include "VoteViewController.h"
#include "PhotoViewController.h"
#include "NoticeViewController.h"
#include "FDataManager.h"
#include "FServerTime.h"

#define REFRESH_STEP 5

MainViewController::MainViewController()
: m_pageView(NULL)
, m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_pastSection(0)
, m_nextSection(1)
{
    m_msg = FDataManager::getInstance()->getSessionMsgs();
}

MainViewController::~MainViewController()
{
    
}

void MainViewController::update(float dt)
{
    CAViewController::update(dt);
}

void MainViewController::viewDidAppear()
{
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
        m_msgTableView->reloadData();
    }
}

void MainViewController::viewDidDisappear()
{

}

void MainViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(120)));
    sView->setTouchEnabled(false);
    this->getView()->addSubview(sView);

	CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
	CAImageView* imageView = CAImageView::createWithImage(CAImage::create("main/nav_notification.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	button->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(20);
	this->getView()->addSubview(button);
    
    m_timeNoticeImageView = CAImageView::createWithFrame(DRect(_px(60), _px(30), _px(10), _px(10)));
    m_timeNoticeImageView->setImage(CAImage::create("common/reddot.png"));
    button->addSubview(m_timeNoticeImageView);

	button = CAButton::createWithFrame(DRect(m_winSize.width - _px(100), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
	imageView = CAImageView::createWithImage(CAImage::create("main/nav_prize.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	button->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(30);
	this->getView()->addSubview(button);

	CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(40));
	label->setText("SAP DKOM");
	label->setFontName("fonts/arial.ttf");
    label->setTouchEnabled(false);
	sView->addSubview(label);
    
    if (m_msg->empty())
    {
        requestSessionMsg();
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
    }
    else
    {
        this->initMsgTableView();
    }
}

void MainViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void MainViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    if (m_msg->size() - m_filterMsg.size() > 0)
    {
        int count = 0;
        for (int i = (int)m_msg->size() - 1; i >= 0; i--)
        {
            if (m_msg->at(i).m_stored &&
                m_msg->at(i).m_startTime <= m_filterMsg[0]->m_startTime &&
                m_msg->at(i).m_sessionId != m_filterMsg[0]->m_sessionId)
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
}

void MainViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    if (m_msg->size() - m_filterMsg.size() > 0)
    {
        int count = 0;
        for (int i = 0; i < (int)m_msg->size() - 1; i++)
        {
            if (m_msg->at(i).m_stored &&
                m_msg->at(i).m_startTime >= m_filterMsg[m_filterMsg.size() - 1]->m_startTime &&
                m_msg->at(i).m_sessionId != m_filterMsg[m_filterMsg.size() - 1]->m_sessionId)
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
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}

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
    m_pastSection = 0;
    m_nextSection = 1;
    
    m_msgTableView = CATableView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setScrollViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
    //m_msgTableView->setSeparatorViewHeight(_px(2));
    this->getView()->addSubview(m_msgTableView);
    CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    refreshDiscount->setLabelColor(CAColor_black);
    CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    refreshDiscount1->setLabelColor(CAColor_black);
    m_msgTableView->setFooterRefreshView(refreshDiscount);
    m_msgTableView->setHeaderRefreshView(refreshDiscount1);
    
    initPageView();
}

void MainViewController::initPageView()
{
	CAView* headView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.width / 2 + _px(130)));
    m_pageView = CAPageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.width / 2), CAPageViewDirectionHorizontal);
    m_pageView->setPageViewDelegate(this);
    m_pageView->setTouchEnabled(true);
    CAVector<CAView* > viewList;
    for (int i = 0; i<m_page.size(); i++)
    {
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(m_winSize.width / 2, 100, m_winSize.width, m_winSize.width / 2));
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setImage(CAImage::create("common/bg.png"));
        temImage->setUrl(m_page[i].m_imageUrl);
        temImage->setTouchEnabled(false);
        viewList.pushBack(temImage);
    }
    m_pageView->setViews(viewList);
    m_pageView->setCurrPage(0, false);
    headView->addSubview(m_pageView);
    
    CAView* bg = CAView::createWithColor(ccc4(0, 0, 0, 100));
    bg->setFrame(DRect(0, m_winSize.width / 2 - 50, m_winSize.width, 50));
    bg->setTouchEnabled(false);
    headView->addSubview(bg);
    
    if (m_page.size() > 0)
    {
        m_pageViewTitle = CALabel::createWithFrame(DRect(20, m_winSize.width / 2 - 40, m_winSize.width - 50, 50));
        m_pageViewTitle->setText(m_page[0].m_title);
        m_pageViewTitle->setColor(CAColor_white);
        m_pageViewTitle->setFontSize(_px(28));
        m_pageViewTitle->setScrollEnabled(false);
        m_pageViewTitle->setTouchEnabled(false);
        headView->addSubview(m_pageViewTitle);
    }
    
    m_pageControl = CAPageControl::createWithCenter(DRect(m_winSize.width / 2, m_winSize.width / 2 - _px(25), _px(100), _px(50)));
    m_pageControl->setNumberOfPages((int)m_page.size());
    m_pageControl->setTag(200);
    m_pageControl->addTarget(this, CAControl_selector(MainViewController::buttonCallBack));
    headView->addSubview(m_pageControl);
    
	for (int i = 0; i < 3; i++)
	{
		DRect r(i * (m_winSize.width / 3), m_winSize.width / 2 + 2, m_winSize.width / 3 - 2, _px(116));
		CAButton* btn = CAButton::createWithFrame(r, CAButtonTypeCustom);
		btn->setTag(300 + i);
		CAImageView* imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("main/short_%d.png", i)));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        imageView->setFrame(DRect((m_winSize.width / 3 - _px(80)) / 2, _px(20), _px(80), _px(80)));
        imageView->setTouchEnabled(false);
        btn->addSubview(imageView);
        CALabel* label = CALabel::createWithFrame(DRect(0, _px(100), m_winSize.width / 3, _px(30)));
        label->setTextAlignment(CATextAlignmentCenter);
        label->setColor(CAColor_gray);
        label->setFontSize(_px(25));
        label->setText(unicode_to_utf8(mainShort[i]));
        label->setFontName("fonts/arial.ttf");
        label->setTouchEnabled(false);
        btn->addSubview(label);
		btn->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
		headView->addSubview(btn);
	}

    m_msgTableView->setTableHeaderView(headView);
    m_msgTableView->setTableHeaderHeight(m_winSize.width / 2 + _px(130));

}

void MainViewController::showAlert()
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
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithFrame(DRect(0, m_winSize.height - _px(300), m_winSize.width, _px(40)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(30));
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
    CCLog("requestSessionMsg %d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
}

void MainViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        int length = value["pag"].size();
        m_msg->clear();
        m_filterMsg.clear();
        m_page.clear();
        FDataManager::getInstance()->setDiffServerTime(value["srt"].asInt64());
        for (int i = 0; i < length; i++) {
            newsPage temp_page;
            temp_page.m_titleId = value["pag"][i]["tid"].asInt();
            temp_page.m_title = value["pag"][i]["til"].asString();
            temp_page.m_imageUrl = value["pag"][i]["url"].asString();
            m_page.push_back(temp_page);
        }
        
        const CSJson::Value& v1 = json["result"]["sel"];
        length = v1.size();
        for (int index = 0; index < length; index++)
        {
            sessionMsg temp_msg;
            temp_msg.m_sessionId = v1[index]["SessionId"].asInt();
            temp_msg.m_title = v1[index]["SessionTitle"].asString();
            temp_msg.m_location = v1[index]["Location"].asString();
            temp_msg.m_detail = v1[index]["SessionDescription"].asString();
            temp_msg.m_lecturer = crossapp_format_string("%s %s", v1[index]["FirstName"].asString().c_str(), v1[index]["LastName"].asString().c_str());
            temp_msg.m_lecturerEmail = v1[index]["Email"].asString();
            temp_msg.m_track = v1[index]["Track"].asString();
            temp_msg.m_format = v1[index]["Format"].asString();
            temp_msg.m_startTime = v1[index]["StarTime"].asInt64();
            temp_msg.m_endTime = v1[index]["EndTime"].asInt();
            temp_msg.m_likeNum = 20;//value[index]["lkn"].asInt();
            temp_msg.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";//value[index]["img"].asString();
            temp_msg.m_stored = v1[index]["Stored"].asBool();
            temp_msg.m_done = v1[index]["Done"].asBool();
            temp_msg.m_point = v1[index]["Point"].asBool();
            m_msg->push_back(temp_msg);
        }
        const CSJson::Value& v2 = json["result"]["usr"];
        userInfo uInfo;
        uInfo.m_userId = FDataManager::getInstance()->getUserId();
        uInfo.m_userName = crossapp_format_string("%s %s", v2["FirstName"].asString().c_str(), v2["LastName"].asString().c_str());
        uInfo.m_point = v2["Rank"].asInt();
        uInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
        FDataManager::getInstance()->setUserInfo(uInfo);
        
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
    }
    else
    {
        //showAlert();
    }
    
//    {
        //m_msg->clear();
        //m_filterMsg.clear();
        m_page.clear();
        for (int i = 0; i < 3; i++) {
            char title[8];
            sprintf(title, "title_%d", i);
            
            newsPage temp_page;
            temp_page.m_titleId = 100 + i;
            temp_page.m_title = title;
            if (i == 0) {
                temp_page.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            }
            else if(i == 1)
            {
                temp_page.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689484_980x1200_0.png";
            }
            else if(i == 2)
            {
                temp_page.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689485_980x1200_0.png";
            }
            
            m_page.push_back(temp_page);
        }
//        srand((int)getTimeSecond());
//        for (int i = 0; i < 17; i++)
//        {
//            sessionMsg temp_msg;
//            temp_msg.m_sessionId = 200 + i;
//            temp_msg.m_title = "Customer Success";
//            
//            temp_msg.m_location = "Lisa Chen";
//            temp_msg.m_detail = "This is Photoshop's version of Lorem Ipsum. \
//                This is Photoshop's version of Lorem Ipsum. \
//                This is Photoshop's version of Lorem Ipsum. ";
//            temp_msg.m_lecturer = "Lisa Chen";
//            temp_msg.m_lecturerEmail = "coostein@hotmail.com";
//            temp_msg.m_track = "Customer";
//            temp_msg.m_format = "Dev Faire";
//            temp_msg.m_startTime = getTimeSecond() + ((rand() % 10) - 5) * 3600;
//            temp_msg.m_endTime = temp_msg.m_startTime + rand() % 3900;
//            temp_msg.m_likeNum = 20;
//            temp_msg.m_stored = (bool)(rand() % 2);
//            temp_msg.m_imageUrl =
//                "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
//            //"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
//            temp_msg.m_stored = (bool)(rand() % 2);
//            temp_msg.m_done = (bool)(rand() % 2);
//            temp_msg.m_point = 22;
//            m_msg->push_back(temp_msg);
//            
//            userInfo uInfo;
//            uInfo.m_userId = 101;
//            uInfo.m_userName = "Alex Chen";
//            uInfo.m_point = 100;
//            uInfo.m_pointRank = 20;
//            uInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
//            FDataManager::getInstance()->setUserInfo(uInfo);
//        }
//        quickSort(m_msg, 0, (int)m_msg->size() - 1);
//        m_filterMsg.clear();
//        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
//        {
//            if(it->m_stored && it->m_endTime > getTimeSecond())
//            {
//                m_filterMsg.push_back(&(*it));
//            }
//            if (m_filterMsg.size() == REFRESH_STEP)
//            {
//                break;
//            }
//        }
//    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

void MainViewController::onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg->clear();
        m_page.clear();
        for (int i = 0; i<3; i++) {
            newsPage temp_page;
            temp_page.m_titleId = value[i]["til"].asInt();
            temp_page.m_title = value[i]["til"].asString();
            temp_page.m_imageUrl = value[i]["url"].asString();
            m_page.push_back(temp_page);
        }
        for (int index = 3; index < length; index++)
        {
            sessionMsg temp_msg;
            temp_msg.m_sessionId = value[index]["sid"].asInt();
            temp_msg.m_title = value[index]["title"].asString();
            temp_msg.m_lecturer = value[index]["lct"].asString();
            temp_msg.m_track = value[index]["trk"].asString();
            temp_msg.m_format = value[index]["fmt"].asString();
            temp_msg.m_startTime = value[index]["trk"].asInt64();
            temp_msg.m_endTime = value[index]["drt"].asInt();
            temp_msg.m_likeNum = value[index]["lkn"].asInt();
            temp_msg.m_imageUrl = value[index]["image"].asString();
            m_msg->push_back(temp_msg);
        }
    }
    
    do
    {
        CC_BREAK_IF(p_pLoading == NULL);
        if (p_pLoading->isAnimating())
        {
            p_pLoading->stopAnimating();
        }
        else
        {
            m_msgTableView->reloadData();
        }
    } while (0);
}

void MainViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 20) // note
	{
        //CADevice::sendLocalNotification("hello", "hello world", 10, "notice id");
        NoticeViewController* vc = new NoticeViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
	}
	else if (btn->getTag() == 30) // prize
	{
		PrizeViewController* vc = new PrizeViewController();
		vc->init();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
	}
	else if (btn->getTag() == 100)
    {
        requestSessionMsg();
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
    }
    else if (btn->getTag() == 200)
    {
        CAPageControl* button = (CAPageControl*)btn;
        m_pageView->setCurrPage(button->getCurrentPage(), true);
        if (m_page.size()>0) {
            m_pageViewTitle->setText(m_page[button->getCurrentPage()].m_title);
        }
    }
    else if (btn->getTag() == 300)
    {
        PhotoViewController* vc = new PhotoViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 301)
    {
        VoteViewController* vc = new VoteViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 302)
    {
        MapViewController* vc = new MapViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 400)
    {

    }
    else if (btn->getTag() == 401)
    {
		SessionsViewController* vc = new SessionsViewController();
		vc->init();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 402)
    {
        MyStatusViewController* vc = new MyStatusViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    
}

CATableViewCell* MainViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    MainViewTableCell* cell = dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    cell->setModel(*m_filterMsg[row]);
    
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
    return _px(240);
}

void MainViewController::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void MainViewController::pageViewDidEndTurning(CAPageView* pageView)
{
    m_pageControl->setCurrentPage(pageView->getCurrPage());
    m_pageControl->updateCurrentPageDisplay();
    if (m_page.size() > 0) {
        m_pageViewTitle->setText(m_page[pageView->getCurrPage()].m_title);
    }
}

void MainViewController::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}

void MainViewController::pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}

void MainViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    SessionDetailViewController* vc = new SessionDetailViewController(m_msg->at(row));
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MainViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}