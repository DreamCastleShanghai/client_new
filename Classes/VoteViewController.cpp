
#include "VoteViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SimpleAudioEngine.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "FServerTime.h"

VoteViewController::VoteViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_canVote(true)
{
    for (int i = 0; i < 2; i++)
    {
        m_currentPage[i] = 0;
        m_pageControl[i] = NULL;
        m_detailView = NULL;
    }
    
    //CAScheduler::schedule(schedule_selector(CDUIShowView::progressValueChange), this, 0.01, false);
}

VoteViewController::~VoteViewController()
{

}

void VoteViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(100), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_forward.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(30);
    this->getView()->addSubview(button);
    
    FSegmentView* seg =
        FSegmentView::createWithFrame(DRect((m_winSize.width - 400) / 2, 40, 400, 60), 2);
    seg->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
    seg->setItemTile("Demo Jam", 0);
    seg->setItemTile("SAP Voice", 1);
    seg->setTag(200, 0);
    seg->setTag(201, 1);
    this->getView()->addSubview(seg);
    
    DRect r(0, _px(120), m_winSize.width, m_winSize.height - _px(120));
    for (int i = 0; i < 2; i++)
    {
        m_segBgView[i] = CAImageView::createWithFrame(r);
        m_segBgView[i]->setImage(CAImage::create(crossapp_format_string("common/vote_%d_bg.jpg", i)));
        m_segBgView[i]->setTouchEnabled(false);
        this->getView()->addSubview(m_segBgView[i]);
    }
    m_segBgView[1]->setVisible(false);
    
    CAApplication::getApplication()->getAccelerometer()->setDelegate(this);
    
    requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void VoteViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void VoteViewController::initMsgTableView()
{
    if (m_msg.empty())
    {
        showAlert();
        return;
    }
    
    CALabel* label = CALabel::createWithFrame(DRect(0, _px(150), m_winSize.width, _px(100)));
    label->setText("03:20:28");
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(80));
    this->getView()->addSubview(label);
    
    CAScrollView* scrollView = CAScrollView::createWithFrame(DRect(_px(20), _px(280), m_winSize.width - _px(40), (m_winSize.width - _px(40)) * 1.19f + _px(500)));
    scrollView->setHorizontalScrollEnabled(false);
    scrollView->setVerticalScrollEnabled(true);
    scrollView->setBounceHorizontal(false);
    scrollView->setBounds(DRect(0, 0, m_winSize.width - _px(40), m_winSize.height - _px(310)));
    scrollView->setAnchorPoint(DPoint(0.f, 0.f));
    scrollView->setShowsHorizontalScrollIndicator(false);
    scrollView->setShowsVerticalScrollIndicator(false);
    CCLog("%f %f", scrollView->getAnchorPoint().x, scrollView->getAnchorPoint().y);
    this->getView()->addSubview(scrollView);
    
    DRect r = DRect(_px(0), _px(0), m_winSize.width - _px(40), (m_winSize.width - _px(40)) * 1.19f);
    m_pageView = CAPageView::createWithFrame(r, CAPageViewDirectionHorizontal);
    m_pageView->setPageViewDelegate(this);
    m_pageView->setCurrPage(0, false);
    m_pageView->setBackGroundColor(CAColor_clear);
    m_pageType = 0;

    scrollView->addSubview(m_pageView);
    scrollView->setBackGroundColor(CAColor_clear);
    //scrollView->setAlpha(0);
    
    r = DRect((m_winSize.width - _px(340)) / 2, (m_winSize.width - _px(40)) * 1.19f - _px(50), _px(300), _px(30));
    for (int i = 0; i < 2; i++)
    {
        m_pageControl[i] = CAPageControl::createWithFrame(r);
        m_pageControl[i]->setTag(300 + i);
        m_pageControl[i]->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack));
        
    }
    m_pageControl[0]->setNumberOfPages(m_pageView->getPageCount());
    m_pageControl[1]->setNumberOfPages((int)m_msg.size() - m_pageView->getPageCount());
    m_pageControl[1]->setVisible(false);
    scrollView->addSubview(m_pageControl[0]);
    scrollView->addSubview(m_pageControl[1]);
    
    m_detailView = CAView::createWithFrame(DRect(0, (m_winSize.width - _px(40)) * 1.19f + _px(20), _px(100), _px(500)));
    m_detailView->setTouchEnabled(false);
    m_detailView->setColor(CAColor_clear);
    scrollView->addSubview(m_detailView);
    
    refreshPageView(0);
}

void VoteViewController::setDetailView(int type, int index)
{
    m_detailView->removeAllSubviews();
    
    if (type == 0 && m_demoMsg.empty())
    {
        return;
    }
    else if (type == 1 && m_voiceMsg.empty())
    {
        return;
    }
    DSize _size = DSize(m_winSize.width - _px(40), _px(500));
    DRect r = DRect(0, 0, _size.width, _px(35));
    CALabel* label = CALabel::createWithFrame(r);
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(_px(30));
    if(m_pageType == 0)
    {
        label->setText(crossapp_format_string("Time: %s - %s",
                                              timeToString(m_demoMsg[index].m_playStartTime).c_str(),
                                              timeToString(m_demoMsg[index].m_playEndTime).c_str()));
    }
    else if(m_pageType == 1)
    {
        label->setText(crossapp_format_string("Time: %s - %s",
                                              timeToString(m_voiceMsg[index].m_playStartTime).c_str(),
                                              timeToString(m_voiceMsg[index].m_playEndTime).c_str()));
    }
    m_detailView->addSubview(label);
    
    r = DRect(0, _px(35), _size.width, _px(35));
    label = CALabel::createWithFrame(r);
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(_px(30));
    if(m_pageType == 0)
    {
        label->setText(crossapp_format_string("Location: %s",m_demoMsg[index].m_projectLoctaion.c_str()));
    }
    else if(m_pageType == 1)
    {
        label->setText(crossapp_format_string("Location: %s",m_voiceMsg[index].m_projectLoctaion.c_str()));
    }
    m_detailView->addSubview(label);
    
    r = DRect(0, _px(100), _size.width, _px(35));
    label = CALabel::createWithFrame(r);
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(_px(30));
    label->setText("How to Participate");
    m_detailView->addSubview(label);

    r = DRect(0, _px(135), _size.width, _px(30));
    label = CALabel::createWithFrame(r);
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(_px(26));
    label->setText("Simple shake your phone in the voting process");
    m_detailView->addSubview(label);
    
    r = DRect(0, _px(195), _size.width, _px(35));
    label = CALabel::createWithFrame(r);
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(_px(30));
    if(m_pageType == 0)
    {
        label->setText(crossapp_format_string("Introduction of %s",m_demoMsg[index].m_playerName.c_str()));
    }
    else if(m_pageType == 1)
    {
        label->setText(crossapp_format_string("Introduction of %s",m_voiceMsg[index].m_playerName.c_str()));
    }
    m_detailView->addSubview(label);
    
    r = DRect(0, _px(230), _size.width, _px(270));
    label = CALabel::createWithFrame(r);
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    label->setFontSize(_px(26));
    
    if(m_pageType == 0)
    {
        label->setText(m_demoMsg[index].m_projectDetail.c_str());
    }
    else if(m_pageType == 1)
    {
        label->setText(m_voiceMsg[index].m_projectDetail.c_str());
    }
    m_detailView->addSubview(label);
}

void VoteViewController::refreshPageView(int index)
{
    m_pageType = index;
    CAVector<CAView* > viewList;
    for (int i = 0; i < m_msg.size(); i++)
    {
        if (m_msg[i].m_projectType != index)
        {
            continue;
        }
        DRect r(0, 0, m_winSize.width - _px(40), (m_winSize.width - _px(40)) * 1.19f);
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(r);
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        temImage->setImage(CAImage::create("tmp/Demo.png"));
        temImage->setUrl(m_msg[i].m_imageUrl);
        temImage->setTouchEnabled(false);
        
        CALabel* label = CALabel::createWithFrame(DRect(_px(30), _px(30), _px(150), _px(30)));
        label->setText(m_msg[i].m_playerName);
        label->setFontSize(_px(25));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        label = CALabel::createWithFrame(DRect(_px(30), _px(60), _px(150), _px(20)));
        label->setText(m_msg[i].m_playerTeam);
        label->setFontSize(_px(18));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        viewList.pushBack(temImage);
    }
    m_pageView->setViews(viewList);
    m_pageView->setCurrPage(m_currentPage[m_pageType], false);
    if (m_pageControl[m_pageType])
    {
        m_pageControl[m_pageType]->setCurrentPage(m_currentPage[m_pageType]);
    }
    m_segBgView[index]->setVisible(true);
    m_segBgView[1 - index]->setVisible(false);
    
    setDetailView(m_pageType, m_currentPage[m_pageType]);
}

void VoteViewController::requestMsg()
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = voteViewTag[0];
    key_value["uid"] = FDataManager::getInstance()->getUserId();
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(VoteViewController::onRequestFinished));
    {
        DRect r(m_winSize.width / 2, (m_winSize.height - _px(120)) / 2 + _px(120),
                m_winSize.width, m_winSize.height - _px(120));
        p_pLoading = CAActivityIndicatorView::createWithCenter(r);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(VoteViewController::initMsgTableView));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
    }
}

void VoteViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 30)
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
        if (m_msg.empty()) {
            return;
        }
        refreshPageView(0);
    }
    else if (btn->getTag() == 201)
    {
        if (m_msg.empty()) {
            return;
        }
        refreshPageView(1);
    }
    else if (btn->getTag() == 300) // page control
    {
        
    }
}

void VoteViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg.clear();
        
        for (int index = 0; index < length; index++)
        {
            voteMsg temp_msg;
            temp_msg.m_projectId = value[index]["pid"].asInt();
            temp_msg.m_projectType = value[index]["pty"].asInt();
            temp_msg.m_playerName = value[index]["pnm"].asString();
            temp_msg.m_playerTeam = value[index]["ptm"].asString();
            temp_msg.m_playStartTime = value[index]["pst"].asInt64();
            temp_msg.m_playEndTime = value[index]["pset"].asInt64();
            temp_msg.m_projectLoctaion = value[index]["lct"].asString();
            temp_msg.m_projectDetail = value[index]["pdl"].asString();
            temp_msg.m_imageUrl = value[index]["img"].asString();
            m_msg.push_back(temp_msg);
            
            if (temp_msg.m_projectType == 0)
            {
                m_demoMsg.push_back(temp_msg);
            }
            else if (temp_msg.m_projectType == 1)
            {
                m_voiceMsg.push_back(temp_msg);
            }
        }
    }
    else
    {
        //showAlert();
    }
#ifdef LOCALTEST
    {
        struct cc_timeval tp;
        getTimeval(&tp, NULL);
        for (int index = 0; index < 3; index++)
        {
            voteMsg temp_msg;
            temp_msg.m_projectId = index;
            temp_msg.m_projectType = 0;
            temp_msg.m_playerName = "TEAM 1";
            temp_msg.m_playerTeam = "CI&SP";
            temp_msg.m_playStartTime = tp.tv_sec;
            temp_msg.m_playEndTime = tp.tv_sec + 3600;
            temp_msg.m_projectLoctaion = "Shanghai Convention Center";
            temp_msg.m_projectDetail = "This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum.";
            temp_msg.m_imageUrl = "a";//"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            m_msg.push_back(temp_msg);
            
            if (temp_msg.m_projectType == 0)
            {
                m_demoMsg.push_back(temp_msg);
            }
            else if (temp_msg.m_projectType == 1)
            {
                m_voiceMsg.push_back(temp_msg);
            }
        }
        
        for (int index = 0; index < 2; index++)
        {
            voteMsg temp_msg;
            temp_msg.m_projectId = index;
            temp_msg.m_projectType = 1;
            temp_msg.m_playerName = "TEAM 1";
            temp_msg.m_playerTeam = "CI&SP";
            temp_msg.m_playStartTime = tp.tv_sec;
            temp_msg.m_playEndTime = tp.tv_sec + 3600;
            temp_msg.m_projectLoctaion = "Shanghai Convention Center";
            temp_msg.m_projectDetail = "This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum.";
            temp_msg.m_imageUrl = "a";//"http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            m_msg.push_back(temp_msg);
            
            if (temp_msg.m_projectType == 0)
            {
                m_demoMsg.push_back(temp_msg);
            }
            else if (temp_msg.m_projectType == 1)
            {
                m_voiceMsg.push_back(temp_msg);
            }
        }
    }
#endif
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}


void VoteViewController::showAlert()
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
    button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
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


void VoteViewController::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void VoteViewController::pageViewDidEndTurning(CAPageView* pageView)
{
    setDetailView(m_pageType, m_currentPage[m_pageType]);
    m_currentPage[m_pageType] = pageView->getCurrPage();
    m_pageControl[m_pageType]->setCurrentPage(pageView->getCurrPage());
    m_pageControl[m_pageType]->updateCurrentPageDisplay();
}

void VoteViewController::didAccelerate(CCAcceleration* pAccelerationValue)
{
    float nowGX = (pAccelerationValue->x)*9.81f;
    float nowGY = (pAccelerationValue->y)*9.81f;
    
    float dt = 30.f;
    if(m_canVote && (nowGX<-dt||nowGX>dt || nowGY<-dt||nowGY>dt))
    {
        m_canVote = false;
        //m_voteLabel->setText("vote success !");
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/vote.wav");
    }
    
}
