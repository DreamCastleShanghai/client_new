
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
#include "FDataManager.h"


MainViewController::MainViewController()
: m_pageView(NULL)
, m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, p_section(1)
{
    
}

MainViewController::~MainViewController()
{
    
}

void MainViewController::update(float dt)
{
    CAViewController::update(dt);
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
    
    if (m_msg.empty())
    {
        std::map<std::string, std::string> key_value;
        key_value["tag"] = mainViewTag[0];
        key_value["page"] = "1";
        key_value["limit"] = "20";
        key_value["appid"] = "10000";
        key_value["sign_method"] = "md5";
        key_value["sign"] = getSign(key_value);
        
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
        
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
    std::map<std::string, std::string> key_value;
    key_value["tag"] = mainViewTag[0];
    key_value["page"] = "1";
    key_value["limit"] = "20";
    key_value["appid"] = "10000";
    key_value["sign_method"] = "md5";
    key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
}

void MainViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    p_section++;
    std::map<std::string, std::string> key_value;
    key_value["tag"] = mainViewTag[0];
    key_value["page"] = crossapp_format_string("%d", p_section);
    key_value["limit"] = "20";
    key_value["appid"] = "10000";
    key_value["sign_method"] = "md5";
    key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRefreshRequestFinished));
}

void MainViewController::initMsgTableView()
{
    if (m_page.empty())
    {
        showAlert();
        return;
    }
    if (m_msgTableView != NULL)
    {
        this->getView()->removeSubview(m_msgTableView);
        m_msgTableView = NULL;
    }
    
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

void MainViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value["pag"].size();
        m_msg.clear();
        m_page.clear();
        FDataManager::getInstance()->setDiffServerTime(value["srt"].asInt64());
        for (int i = 0; i < length; i++) {
            newsPage temp_page;
            temp_page.m_titleId = value["pag"][i]["tid"].asInt();
            temp_page.m_title = value["pag"][i]["til"].asString();
            temp_page.m_imageUrl = value["pag"][i]["url"].asString();
            m_page.push_back(temp_page);
        }
        length = value["nws"].size();
        for (int index = 0; index < length; index++)
        {
            newsMsg temp_msg;
            temp_msg.m_sessionId = value["nws"][index]["sid"].asInt();
            temp_msg.m_title = value["nws"][index]["til"].asString();
            temp_msg.m_location = value["nws"][index]["loc"].asString();
            temp_msg.m_detail = value["nws"][index]["dtl"].asString();
            temp_msg.m_lecturer = value["nws"][index]["lct"].asString();
            temp_msg.m_lecturerEmail = value["nws"][index]["eml"].asString();
            temp_msg.m_track = value["nws"][index]["trk"].asString();
            temp_msg.m_format = value["nws"][index]["fmt"].asString();
            temp_msg.m_startTime = value["nws"][index]["trk"].asInt64();
            temp_msg.m_endTime = value["nws"][index]["drt"].asInt();
            temp_msg.m_likeNum = value["nws"][index]["lkn"].asInt();
            temp_msg.m_imageUrl = value["nws"][index]["img"].asString();
            m_msg.push_back(temp_msg);
        }
        
    }
    
    {
        m_msg.clear();
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
        
        for (int i = 0; i < 7; i++)
        {
            newsMsg temp_msg;
            temp_msg.m_sessionId = 200 + i;
            temp_msg.m_title = "Customer Success";
            
            temp_msg.m_location = "Lisa Chen";
            temp_msg.m_detail = "This is Photoshop's version of Lorem Ipsum. \
                This is Photoshop's version of Lorem Ipsum. \
                This is Photoshop's version of Lorem Ipsum. ";
            temp_msg.m_lecturer = "Lisa Chen";
            temp_msg.m_lecturerEmail = "coostein@hotmail.com";
            temp_msg.m_track = "Customer";
            temp_msg.m_format = "Dev Faire";
            cc_timeval ct;
            CCTime::gettimeofdayCrossApp(&ct, NULL);
            temp_msg.m_startTime = ct.tv_sec + 3500;
            temp_msg.m_likeNum = 20;
            temp_msg.m_imageUrl =
                "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
            //"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            m_msg.push_back(temp_msg);
        }
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}

void MainViewController::onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg.clear();
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
            newsMsg temp_msg;
            temp_msg.m_sessionId = value[index]["sid"].asInt();
            temp_msg.m_title = value[index]["title"].asString();
            temp_msg.m_lecturer = value[index]["lct"].asString();
            temp_msg.m_track = value[index]["trk"].asString();
            temp_msg.m_format = value[index]["fmt"].asString();
            temp_msg.m_startTime = value[index]["trk"].asInt64();
            temp_msg.m_endTime = value[index]["drt"].asInt();
            temp_msg.m_likeNum = value[index]["lkn"].asInt();
            temp_msg.m_imageUrl = value[index]["image"].asString();
            m_msg.push_back(temp_msg);
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
        CADevice::sendLocalNotification("hello", "hello world", 10, "notice id");
	}
	else if (btn->getTag() == 30) // prize
	{
		PrizeViewController* vc = new PrizeViewController();
		vc->init();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
	}
	else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
        std::map<std::string, std::string> key_value;
        key_value["tag"] = mainViewTag[0];
        key_value["page"] = "1";
        key_value["limit"] = "20";
        key_value["appid"] = "10000";
        key_value["sign_method"] = "md5";
        key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
        }
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
    cell->setModel(m_msg[row]);
    
    return cell;
}

unsigned int MainViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int MainViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_msg.size();
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
    SessionDetailViewController* vc = new SessionDetailViewController(m_msg[row]);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MainViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}