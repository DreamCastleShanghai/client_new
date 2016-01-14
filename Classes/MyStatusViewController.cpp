
#include "MyStatusViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "FServerTime.h"
#include "FDataManager.h"

MyStatusViewController::MyStatusViewController()
: m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, m_pointView(NULL)
, m_navType(0)
, m_pointType(0)
, m_canSwitchSeg(true)
{
    m_msg = FDataManager::getInstance()->getSessionMsgs();
}

MyStatusViewController::~MyStatusViewController()
{

}

void MyStatusViewController::viewDidAppear()
{
    if (!m_msg->empty())
    {
        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
        if(m_navType == 0)
        {
            m_msgTableView->reloadData();
        }
    }
}

void MyStatusViewController::viewDidDisappear()
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
    
    m_navSegmentView =
        FSegmentView::createWithFrame(DRect((m_winSize.width - 400) / 2, 40, 400, 60), 2);
    m_navSegmentView->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_navSegmentView->setItemTile("Favourites", 0);
    m_navSegmentView->setItemTile("Point", 1);
    m_navSegmentView->setTag(200, 0);
    m_navSegmentView->setTag(201, 1);
    this->getView()->addSubview(m_navSegmentView);
    m_navType = 0;
    
    m_pointView = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, _px(300)));
    m_pointView->setColor(ccc4(0, 0xa8, 0xfc, 0xff));
    m_pointView->setVisible(false);
    this->getView()->addSubview(m_pointView);
    
    m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
    //createWithFrame(DRect(_px(30), _px(40), _px(80), _px(80)));
    m_urlImageView->setFrame(DRect((m_winSize.width - _px(120)) / 2, _px(30), _px(120), _px(120)));
    m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    m_urlImageView->setImage(CAImage::create("common/bg.png"));
    m_pointView->addSubview(m_urlImageView);
    m_nameLabel = CALabel::createWithFrame(DRect((m_winSize.width - _px(200)) / 2, _px(170), _px(200), _px(35)));
    m_nameLabel->setFontSize(_px(30));
    m_nameLabel->setColor(CAColor_white);
    m_nameLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_nameLabel->setTextAlignment(CATextAlignmentCenter);
    m_pointView->addSubview(m_nameLabel);
    
    m_pointSegmentView = FSegmentView::createWithFrame(DRect(0, _px(200), m_winSize.width, _px(100)), 2, 1);
    m_pointSegmentView->addTarget(this, CAControl_selector(MyStatusViewController::buttonCallBack), CAControlEventTouchUpInSide);
    m_pointSegmentView->setTag(300, 0);
    m_pointSegmentView->setTag(301, 1);
    m_pointView->addSubview(m_pointSegmentView);
    for (int i = 0; i < 2; i++)
    {
        m_pointLabel[i] = CALabel::createWithFrame(DRect(i * m_winSize.width / 2, _px(200), m_winSize.width / 2, _px(60)));
        m_pointLabel[i]->setTextAlignment(CATextAlignmentCenter);
        m_pointLabel[i]->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_pointLabel[i]->setFontSize(_px(50));
        m_pointLabel[i]->setTouchEnabled(false);
        m_pointLabel[i]->setColor(CAColor_white);
        m_pointView->addSubview(m_pointLabel[i]);
        m_rankLabel[i] = CALabel::createWithFrame(DRect(i * m_winSize.width / 2, _px(260), m_winSize.width / 2, _px(35)));
        m_rankLabel[i]->setTextAlignment(CATextAlignmentCenter);
        m_rankLabel[i]->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_rankLabel[i]->setFontSize(_px(30));
        m_rankLabel[i]->setTouchEnabled(false);
        m_rankLabel[i]->setColor(CAColor_white);
        m_pointView->addSubview(m_rankLabel[i]);
    }
    m_rankLabel[0]->setText("Credit Point");
    m_rankLabel[1]->setText("Ranking");
    m_pointLabel[0]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
    m_rankLabel[0]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
    userInfo* info = FDataManager::getInstance()->getUserInfo();
    if (info->m_userId > 0)
    {
        m_urlImageView->setUrl(info->m_imageUrl);
        m_nameLabel->setText(info->m_userName);
        m_pointLabel[0]->setText(crossapp_format_string("%d", info->m_point));
    }
    
    if (m_msg->empty())
    {
        m_canSwitchSeg = false;
        requestMsg();
        showLoading();
    }
    else
    {
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
    m_canSwitchSeg = true;
    if (m_msgTableView == NULL)
    {
        m_msgTableView = CATableView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
        m_msgTableView->setTableViewDataSource(this);
        m_msgTableView->setTableViewDelegate(this);
        m_msgTableView->setScrollViewDelegate(this);
        m_msgTableView->setAllowsSelection(true);
        m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
        //m_msgTableView->setSeparatorViewHeight(_px(2));
        this->getView()->addSubview(m_msgTableView);
    }

}

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
        switchNavType(m_navType);
    }
    else if (btn->getTag() == 201)
    {
        if (!m_canSwitchSeg)
        {
            m_navSegmentView->resetSegment();
            return;
        }
        m_navType = m_pointType + 1;
        switchNavType(m_navType);
    }
    else if (btn->getTag() == 300)
    {
        m_pointType = 0;
        m_navType = 1;
        switchNavType(m_navType);
    }
    else if (btn->getTag() == 301)
    {
        if (!m_canSwitchPoint)
        {
            m_pointSegmentView->resetSegment();
            return;
        }
        m_pointType = 1;
        m_navType = 2;
        switchNavType(m_navType);
    }
}

void MyStatusViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg->clear();
        m_filterMsg.clear();
        for (int index = 0; index < length; index++)
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
        userInfo uInfo;
        uInfo.m_userId = 101;
        uInfo.m_userName = "Alex Chen";
        uInfo.m_point = 100;
        uInfo.m_pointRank = 20;
        uInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
        FDataManager::getInstance()->setUserInfo(uInfo);
        
        quickSort(m_msg, 0, (int)m_msg->size() - 1);
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
        userInfo* info = FDataManager::getInstance()->getUserInfo();
        if (info->m_userId > 0)
        {
            m_urlImageView->setUrl(info->m_imageUrl);
            m_nameLabel->setText(info->m_userName);
            m_pointLabel[0]->setText(crossapp_format_string("%d", info->m_point));
        }
    }
    else
    {
        //showAlert();
    }
    
    {
        m_msg->clear();
        m_filterMsg.clear();
        srand((int)getTimeSecond());
        for (int i = 0; i < 17; i++)
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
            temp_msg.m_startTime = getTimeSecond() + ((rand() % 10) - 5) * 3600;
            temp_msg.m_endTime = temp_msg.m_startTime + rand() % 3900;
            temp_msg.m_likeNum = 20;
            temp_msg.m_stored = (bool)(rand() % 2);
            temp_msg.m_imageUrl =
            "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
            //"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            temp_msg.m_stored = (bool)(rand() % 2);
            temp_msg.m_done = (bool)(rand() % 2);
            m_msg->push_back(temp_msg);
            
            userInfo uInfo;
            uInfo.m_userId = 101;
            uInfo.m_userName = "Alex Chen";
            uInfo.m_point = 100;
            uInfo.m_pointRank = 20;
            uInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
            FDataManager::getInstance()->setUserInfo(uInfo);
            
            userInfo* info = FDataManager::getInstance()->getUserInfo();
            if (info->m_userId > 0)
            {
                m_urlImageView->setUrl(info->m_imageUrl);
                m_nameLabel->setText(info->m_userName);
                m_pointLabel[0]->setText(crossapp_format_string("%d", info->m_point));
            }
        }
        quickSort(m_msg, 0, (int)m_msg->size() - 1);
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
    }
    initMsgTableView();
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    
}

void MyStatusViewController::onRequestRankFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        m_canSwitchPoint = true;
        const CSJson::Value& v2 = json["result"]["rl"];
        int length = v2.size();
        m_rankMsg.clear();
        for (int i = 0; i < length; i++)
        {
            userInfo tmpInfo;
            tmpInfo.m_userName = crossapp_format_string("%s %s", v2[i]["FirstName"].asString().c_str(), v2[i]["LastName"].asString().c_str());
            tmpInfo.m_pointRank = i + 1;
            tmpInfo.m_point = v2[i]["Rank"].asInt();
            tmpInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
            m_rankMsg.push_back(tmpInfo);
        }
        
        const CSJson::Value& v1 = json["result"]["usr"];
        userInfo uInfo;
        uInfo.m_userId = FDataManager::getInstance()->getUserId();
        uInfo.m_userName = crossapp_format_string("%s %s", v1["FirstName"].asString().c_str(), v1["LastName"].asString().c_str());
        uInfo.m_point = v1["Rank"].asInt();
        uInfo.m_pointRank = json["result"]["urk"].asInt();
        uInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
        FDataManager::getInstance()->setUserInfo(uInfo);
        
        m_pointLabel[1]->setText(crossapp_format_string("%d", uInfo.m_pointRank + 1));
        //sort
    }
    else
    {
        showAlert();
    }
    
//    {
//        m_canSwitchPoint = true;
//        m_rankMsg.clear();
//        for (int i = 0; i < 10; i++)
//        {
//            userInfo tmpInfo;
//            tmpInfo.m_userId = 10;
//            tmpInfo.m_userName = "Alxm Chen";
//            tmpInfo.m_point = 251 - i;
//            tmpInfo.m_pointRank = i + 1;
//            tmpInfo.m_imageUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
//            m_rankMsg.push_back(tmpInfo);
//        }
//        userInfo* info = FDataManager::getInstance()->getUserInfo();
//        if (info->m_userId > 0)
//        {
//            m_pointLabel[1]->setText(crossapp_format_string("%d", m_rankMsg[9].m_point - info->m_point));
//        }
//        
//    }
}

void MyStatusViewController::switchNavType(int index)
{
    if (index == 0)
    {
        m_pointView->setVisible(false);
        m_searchButton->setVisible(true);
        m_pointButton->setVisible(false);

        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_stored)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
        m_msgTableView->setFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
        m_msgTableView->reloadData();

    }
    else if(index == 1)
    {
        if (m_rankMsg.empty())
        {
            requestRankMsg();
        }
        m_pointLabel[0]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
        m_rankLabel[0]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
        m_pointLabel[1]->setColor(CAColor_white);
        m_rankLabel[1]->setColor(CAColor_white);
        m_pointView->setVisible(true);
        m_searchButton->setVisible(false);
        m_pointButton->setVisible(true);
        m_filterMsg.clear();
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(it->m_done)
            {
                m_filterMsg.push_back(&(*it));
            }
        }
        m_msgTableView->setFrame(DRect(0, _px(420), m_winSize.width, m_winSize.height - _px(420)));
        m_msgTableView->reloadData();
        
    }
    else if(index == 2)
    {
        m_pointLabel[0]->setColor(CAColor_white);
        m_rankLabel[0]->setColor(CAColor_white);
        m_pointLabel[1]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
        m_rankLabel[1]->setColor(ccc4(0xce, 0xea, 0xfd, 0xff));
        m_pointView->setVisible(true);
        m_searchButton->setVisible(false);
        m_pointButton->setVisible(true);
        m_msgTableView->setFrame(DRect(0, _px(420), m_winSize.width, m_winSize.height - _px(420)));
        m_msgTableView->reloadData();
        
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
    CATableViewCell* cell = NULL;
    if (m_navType == 0)
    {

        cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp0"));
        if(cell == NULL && !m_filterMsg.empty())
        {
            int count = 0;
            for (int i = 0; i < section; i++) {
                count += m_rowNumOfSection[i].rowNum;
            }
            sessionMsg* msg = m_filterMsg[count + row];
            cell = CATableViewCell::create("CrossApp0");
            CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(0), _px(300), _px(50)));
            label->setText(msg->m_title);
            CCLog("%d  %s", count + row, msg->m_title.c_str());
            label->setFontSize(_px(25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);
        }
    }
    else if(m_navType == 1 && !m_filterMsg.empty())
    {
        cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp1"));
        if(cell == NULL)
        {
            sessionMsg* msg = m_filterMsg[row];
            cell = CATableViewCell::create("CrossApp1");
            CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(0), _px(300), _px(50)));
            label->setText(crossapp_format_string("+%d", msg->m_point));
            label->setFontSize(_px(25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);
            label = CALabel::createWithFrame(DRect(_px(120), _px(0), _px(300), _px(50)));
            label->setText(msg->m_title);
            label->setFontSize(_px(25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);
        }
    }
    else if(m_navType == 2 && !m_rankMsg.empty())
    {
        cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp2"));
        if(cell == NULL)
        {
            cell = CATableViewCell::create("CrossApp2");
            CommonUrlImageView* urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
            //createWithFrame(DRect(_px(30), _px(40), _px(80), _px(80)));
            urlImageView->setFrame(DRect(_px(40), _px(5), _px(40), _px(40)));
            urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            urlImageView->setImage(CAImage::create("common/bg.png"));
            urlImageView->setUrl(m_rankMsg[row].m_imageUrl);
            cell->addSubview(urlImageView);
            
            CALabel* label = CALabel::createWithFrame(DRect(_px(100), _px(0), _px(300), _px(50)));
            label->setText(m_rankMsg[row].m_userName);
            label->setFontSize(_px(25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);
            
            label = CALabel::createWithFrame(DRect(m_winSize.width / 2, _px(0), _px(300), _px(50)));
            label->setText(crossapp_format_string("%d", m_rankMsg[row].m_point));
            label->setFontSize(_px(25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);
            
            label = CALabel::createWithFrame(DRect(m_winSize.width - _px(100), _px(0), _px(300), _px(50)));
            label->setText(crossapp_format_string("%d", m_rankMsg[row].m_pointRank));
            label->setFontSize(_px(25));
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setColor(ccc4(0x3f, 0x3f, 0x3f, 0xff));
            cell->addSubview(label);
        }
    }
    return cell;
    
}

CAView* MyStatusViewController::tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = NULL;
    if (m_navType == 0)
    {
        view = CAView::createWithColor(ccc4(0xf3, 0xf3, 0xf3, 0xf3));
        
        CALabel* label = CALabel::createWithFrame(DRect(_px(40), _px(0), _px(300), _px(50)));
        int hour = m_rowNumOfSection[section].hour;
        label->setText(crossapp_format_string("%02d:00 - %02d:00", hour, hour + 1));
        CCLog("head %d %d", section, hour);
        label->setFontSize(_px(25));
        label->setColor(ccc4(0, 0xa8, 0xfc, 0xff));
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        view->addSubview(label);
    }
    return view;
}

unsigned int MyStatusViewController::numberOfSections(CATableView *table)
{
    int num = 1;
    if (m_navType == 0)
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
    if (m_navType == 0)
    {
        CCLog("s:%d", section);
        num = m_rowNumOfSection[section].rowNum;
    }
    else if(m_navType == 1)
    {
        num = (int)m_filterMsg.size();
    }
    else if(m_navType == 2)
    {
        num = (int)m_rankMsg.size();
        CCLog("num %d", num);
    }
    return num;
}

unsigned int MyStatusViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    int hight = 0;
    if (m_navType == 0)
    {
        hight = _px(50);
    }
    return hight;
}

unsigned int MyStatusViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return _px(50);
}

void MyStatusViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if (m_navType == 0 || m_navType == 1)
    {
        SessionDetailViewController* vc = new SessionDetailViewController(m_msg->at(row));
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    
}

void MyStatusViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	
}


