
#include "MyStatusViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"

MyStatusViewController::MyStatusViewController()
: m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_navType(0)
, m_canSwitchSeg(true)
{
    for (int i = 0; i< 2; i++)
    {
        m_segView[i] = NULL;
    }
    m_msg = FDataManager::getInstance()->getSessionMsgs();
}

MyStatusViewController::~MyStatusViewController()
{

}

void MyStatusViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(120)));
    this->getView()->addSubview(sView);
    
    m_searchButton = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_search.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    m_searchButton->setBackGroundViewForState(CAControlStateAll, imageView);
    m_searchButton->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_searchButton->setTag(20);
    this->getView()->addSubview(m_searchButton);
    
    m_pointButton = CAButton::createWithFrame(DRect(m_winSize.width - _px(140), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_search.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    m_pointButton->setBackGroundViewForState(CAControlStateAll, imageView);
    m_pointButton->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_pointButton->setTag(30);
    m_pointButton->setVisible(false);
    this->getView()->addSubview(m_pointButton);
    
    FSegmentView* seg =
        FSegmentView::createWithFrame(DRect((m_winSize.width - 400) / 2, 40, 400, 60), 2);
    seg->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    seg->setItemTile("Favourites", 0);
    seg->setItemTile("Point", 1);
    seg->setTag(200, 0);
    seg->setTag(201, 1);
    this->getView()->addSubview(seg);
    m_navType = 0;
    
    if (m_msg->empty())
    {
        m_canSwitchSeg = false;
        requestMsg();
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
        }
    }
    else
    {
        this->initMsgTableView();
    }
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void MyStatusViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void MyStatusViewController::initMsgTableView()
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
    
    m_msgTableView = CATableView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setScrollViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
    //m_msgTableView->setSeparatorViewHeight(_px(2));
    this->getView()->addSubview(m_msgTableView);
}

void MyStatusViewController::requestMsg()
{
    this->getView()->removeSubview(p_alertView);
    p_alertView = NULL;
    std::map<std::string, std::string> key_value;
    key_value["tag"] = sessionViewTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MyStatusViewController::onRequestFinished));
}

void MyStatusViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if(btn->getTag() == 20)
    {
        SessionsSearchViewController* vc = new SessionsSearchViewController(1);
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if(btn->getTag() == 30)
    {
        
    }
    else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
        requestMsg();
    }
    else if (btn->getTag() == 200)
    {
        m_navType = 0;
        if (m_msgTableView) {
            m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
        }
        
    }
    else if (btn->getTag() == 201)
    {
        m_navType = 1;
        if (m_msgTableView) {
            m_msgTableView->setFrame(DRect(0, _px(250), m_winSize.width, m_winSize.height - _px(250)));
        }
    }
}

void MyStatusViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg->clear();
        
        for (int index = 3; index < length; index++)
        {
            sessionMsg temp_msg;
            temp_msg.m_sessionId = value[index]["sid"].asInt();
            temp_msg.m_title = value[index]["til"].asString();
            temp_msg.m_location = value[index]["loc"].asString();
            temp_msg.m_detail = value[index]["dtl"].asString();
            temp_msg.m_lecturer = value[index]["lct"].asString();
            temp_msg.m_lecturerEmail = value[index]["eml"].asString();
            temp_msg.m_track = value[index]["trk"].asString();
            temp_msg.m_format = value[index]["fmt"].asString();
            temp_msg.m_startTime = value[index]["trk"].asInt64();
            temp_msg.m_endTime = value[index]["drt"].asInt();
            temp_msg.m_likeNum = value[index]["lkn"].asInt();
            temp_msg.m_imageUrl = value[index]["img"].asString();
            m_msg->push_back(temp_msg);
        }
        m_canSwitchSeg = true;
    }
    
    {
        m_msg->clear();
        for (int i = 0; i < 7; i++)
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
            cc_timeval ct;
            CCTime::gettimeofdayCrossApp(&ct, NULL);
            temp_msg.m_startTime = 46800;//ct.tv_sec + 3500;
            temp_msg.m_likeNum = 20;
            temp_msg.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
            //"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            m_msg->push_back(temp_msg);
        }
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    
    initMsgTableView();

    if (m_navType == 0)
    {
        if (m_msgTableView)
        {
            m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
        }
        
    }
    else if (m_navType == 1)
    {
        if (m_msgTableView)
        {
            m_msgTableView->setFrame(DRect(0, _px(250), m_winSize.width, m_winSize.height - _px(250)));
        }
    }
    
}


void MyStatusViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(120)) / 2, m_winSize.width, m_winSize.height - _px(120)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}

CATableViewCell* MyStatusViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    MainViewTableCell* cell = dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    cell->setModel(m_msg->at(row));
    
    return cell;
    
}

unsigned int MyStatusViewController::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int MyStatusViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return (int)m_msg->size();
}

unsigned int MyStatusViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return _px(240);
}

void MyStatusViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    SessionDetailViewController* vc = new SessionDetailViewController(m_msg->at(row));
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MyStatusViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}


