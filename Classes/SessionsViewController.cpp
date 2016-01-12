
#include "SessionsViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "FDataManager.h"
#include "FServerTime.h"

SessionsViewController::SessionsViewController()
: m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_navType(0)
, m_canTouch(false)
{
    
    for (int i = 0; i < m_filterNum; i++)
    {
        m_listButton[i] = NULL;
    }
    m_msg = FDataManager::getInstance()->getSessionMsgs();
}

SessionsViewController::~SessionsViewController()
{

}

void SessionsViewController::viewDidAppear()
{
    
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
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    FSegmentView* seg = FSegmentView::createWithFrame(DRect(m_winSize.width - 240, 50, 200, 50), 2);
    seg->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_time.png"));
    seg->setItemBackGroundImage(imageView, 0);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_type.png"));
    seg->setItemBackGroundImage(imageView, 1);
    seg->setTag(200, 0);
    seg->setTag(201, 1);
    this->getView()->addSubview(seg);

    m_navType = 0;
    m_navTimeType = 0;
    m_navFormatType = 0;
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Agenda");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);
    
    if (m_msg->empty())
    {
        requestMsg();
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            //p_pLoading->setTargetOnCancel(this, callfunc_selector(SessionsViewController::initMsgTableView));
        }
        initMsgTableView();
    }
    else
    {
        m_msgFilter.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            m_msgFilter.push_back(&(*it));
        }
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
    if (m_msgTableView != NULL)
    {
        this->getView()->removeSubview(m_msgTableView);
        m_msgTableView = NULL;
    }
    m_canTouch = true;
    m_listView = CAListView::createWithFrame(DRect(0,_px(120),m_winSize.width,_px(60)));
    m_listView->setListViewDelegate(this);
    m_listView->setListViewDataSource(this);
    m_listView->setAllowsSelection(true);
    m_listView->setAllowsMultipleSelection(false);
    m_listView->setListViewOrientation(CAListViewOrientationHorizontal);
    m_listView->setShowsScrollIndicators(false);
    m_listView->setSeparatorColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
    m_listView->setBackGroundImage(CAImage::create("common/gray_bg.png"));
    m_listView->setTag(1);
    this->getView()->addSubview(m_listView);
    
    m_filterListView = CAListView::createWithFrame(DRect(_px(0),_px(120),m_winSize.width,_px(130)));
    m_filterListView->setListViewDataSource(this);
    m_filterListView->setAllowsSelection(false);
    m_filterListView->setListViewOrientation(CAListViewOrientationHorizontal);
    m_filterListView->setShowsScrollIndicators(false);
    m_filterListView->setSeparatorColor(CAColor_clear);
    m_filterListView->setVisible(false);
    m_filterListView->setTag(2);
    m_filterListView->setBackGroundColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
    //m_filterListView->setScrollEnabled(false);
    
    this->getView()->addSubview(m_filterListView);
    
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
}

void SessionsViewController::requestMsg()
{
    this->getView()->removeSubview(p_alertView);
    p_alertView = NULL;
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished));
}

void SessionsViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if(btn->getTag() == 20)
    {
        SessionsSearchViewController* vc = new SessionsSearchViewController(0);
        vc->init();
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
        if (m_msg->empty()) return;
        if(m_navType == 0) return;
        m_navType = 0;
        m_filterListView->setVisible(false);
        m_listView->setVisible(true);
        if (m_msgTableView) {
            m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
            refreshTableByTime(m_navTimeType);
        }
        
    }
    else if (btn->getTag() == 201)
    {
        if (m_msg->empty()) return;
        if(m_navType == 1) return;
        m_navType = 1;
        m_listView->setVisible(false);
        m_filterListView->setVisible(true);
        if (m_msgTableView) {
            m_msgTableView->setFrame(DRect(0, _px(250), m_winSize.width, m_winSize.height - _px(250)));
            refreshTableByFormat(m_navFormatType);
        }
    }
    else if (btn->getTag() >= 310 && btn->getTag() < 400)
    {
        if (btn->getControlState() == CAControlStateNormal)
        {
            m_navFormatType = -1;
            refreshTableByFormat(m_navFormatType);
        }
        else if (btn->getControlState() == CAControlStateSelected)
        {
            for (int i = 0; i < m_filterNum; i++)
            {
                if (m_listButton[i] == btn) continue;
                
                m_listButton[i]->setControlState(CAControlStateNormal);
            }
            m_navFormatType = btn->getTag() - 310;
            refreshTableByFormat(m_navFormatType);
        }
        
    }
}

void SessionsViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg->clear();
        m_msgFilter.clear();
        for (int index = 3; index < length; index++)
        {
            sessionMsg temp_msg;
            temp_msg.m_sessionId = value[index]["SessionId"].asInt();
            temp_msg.m_title = value[index]["SessionTitle"].asString();
            temp_msg.m_location = value[index]["Location"].asString();
            temp_msg.m_detail = value[index]["SessionDescription"].asString();
            temp_msg.m_lecturer = crossapp_format_string("%s %s", value[index]["FirstName"].asString().c_str(), value[index]["LastName"].asString().c_str());
            temp_msg.m_lecturerEmail = value[index]["Email"].asString();
            temp_msg.m_track = value[index]["Track"].asString();
            temp_msg.m_format = value[index]["Format"].asString();
            temp_msg.m_startTime = value[index]["StarTime"].asInt64();
            temp_msg.m_endTime = value[index]["EndTime"].asInt();
            temp_msg.m_likeNum = 20;//value[index]["lkn"].asInt();
            temp_msg.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";//value[index]["img"].asString();
            temp_msg.m_stored = value[index]["Stored"].asBool();
            temp_msg.m_done = value[index]["Done"].asBool();
            m_msg->push_back(temp_msg);
        }
        quickSort(m_msg, 0, (int)m_msg->size() - 1);
    }
    {
        m_msg->clear();
        m_msgFilter.clear();
        srand((int)getTimeSecond());
        for (int i = 0; i < 12; i++)
        {
            sessionMsg temp_msg;
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
            temp_msg.m_startTime = getTimeSecond() + ((rand() % 10) - 5) * 3600;//ct.tv_sec + 3500;
            temp_msg.m_endTime = temp_msg.m_startTime + rand() % 3600;
            temp_msg.m_likeNum = 20;
            temp_msg.m_stored = (bool)(rand() % 2);
            temp_msg.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
            //"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            temp_msg.m_stored = (bool)(rand() % 2);
            temp_msg.m_done = (bool)(rand() % 2);
            m_msg->push_back(temp_msg);
        }
        quickSort(m_msg, 0, (int)m_msg->size() - 1);
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
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
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
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
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            struct tm* time = localtime(&(it->m_startTime));
            
            if(time->tm_hour >= (index + 8) && time->tm_hour < (index + 9))
                m_msgFilter.push_back(&(*it));
        }
    }
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}

void SessionsViewController::refreshTableByFormat(int format)
{
    m_msgFilter.clear();
    if(format == -1)
    {
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            m_msgFilter.push_back(&(*it));
        }
    }
    else
    {
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if (!strcmp(it->m_track.c_str(), filterItem[format]))
            {
                m_msgFilter.push_back(&(*it));
            }
        }
    }

    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}

void SessionsViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    requestMsg();
    
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
            m_msgTableView->setFrame(DRect(0, _px(250), m_winSize.width, m_winSize.height - _px(250)));
            refreshTableByFormat(m_navFormatType);
        }
    }
}

void SessionsViewController::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{
    if (listView->getTag() == 1)
    {
        m_navTimeType = index;
        refreshTableByTime(m_navTimeType);
    }
}

void SessionsViewController::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{
    
}

unsigned int SessionsViewController::numberOfIndex(CAListView *listView)
{
    int num = 0;
    if (listView->getTag() == 1)
    {
        num = m_filterNum;
    }
    else if(listView->getTag() == 2)
    {
        num = 4;
    }
    return num;
}

unsigned int SessionsViewController::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
    int width = 0;
    if (listView->getTag() == 1)
    {
        width = _px(120);
    }
    else if(listView->getTag() == 2)
    {
        width = _px(160);
    }
    
    return width;
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
            test->setColor(ccc4(0x96, 0x96, 0x96, 255)); // ccc4(0x96, 0x96, 0x96, 255)
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
            button->setBackGroundViewForState(CAControlStateAll, sView);
            sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/seggreen_bg.png"));
            sView->setColor(ccc4(0x86, 0xBD, 0x45, 0xff));
            button->setBackGroundViewForState(CAControlStateSelected, sView);
            button->setTitleForState(CAControlStateAll, filterItem[index * 2]);
            button->setTitleColorForState(CAControlStateAll, ccc4(0xA0, 0xA0, 0xA0, 0xA0));
            button->setTitleFontSize(_px(20));
            button->setAllowsSelected(true);
            button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTag(310 + index * 2);
            m_listButton[index * 2] = button;
            cell->addSubview(button);
            
            button = CAButton::createWithFrame(DRect(_px(20), _px(70), _px(120), _px(40)), CAButtonTypeCustom);
            sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/seggreen_bg.png"));
            sView->setTouchEnabled(false);
            //button->setBackGroundViewForState(CAControlStateAll, sView);
            button->addSubview(sView);
            //button->setBackGroundViewForState(CAControlStateAll, sView);
            sView = CAScale9ImageView::createWithFrame(DRect(_px(0), _px(0), _px(120), _px(40)));
            sView->setImage(CAImage::create("common/btn_round.png"));
            sView->setColor(ccc4(0x86, 0xBD, 0x45, 0xff));
            button->setBackGroundViewForState(CAControlStateSelected, sView);
            button->setTitleForState(CAControlStateAll, filterItem[index * 2 + 1]);
            button->setTitleColorForState(CAControlStateAll, CAColor_gray);
            button->setTitleFontSize(_px(20));
            button->setAllowsSelected(true);
            button->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTag(310 + index * 2 + 1);
            m_listButton[index * 2 + 1] = button;
            cell->addSubview(button);
            
            sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
            sView->setFrame(DRect(_px(0), _px(0), _size.width, _size.height));
            //sView->setColor(CAColor_clear);
            cell->setAlpha(0);
            //cell->setBackgroundView(sView);
                            
        }
        
    }
    return cell;
}

CATableViewCell* SessionsViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    MainViewTableCell* cell = dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    cell->setModel(*m_msgFilter[row]);
    
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
    SessionDetailViewController* vc = new SessionDetailViewController(m_msg->at(row));
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void SessionsViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}


