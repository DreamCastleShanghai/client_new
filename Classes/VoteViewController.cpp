
#include "VoteViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SimpleAudioEngine.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "FServerTime.h"
#include "VoteShakeViewController.h"

VoteViewController::VoteViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_canVote(true)
{
    
    //CAScheduler::schedule(schedule_selector(CDUIShowView::progressValueChange), this, 0.01, false);
}

VoteViewController::~VoteViewController()
{

}

void VoteViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    // header bg
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(120)));
    this->getView()->addSubview(sView);
    
    // left back button
    CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
//    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(100), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
//    imageView = CAImageView::createWithImage(CAImage::create("common/nav_forward.png"));
//    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
//    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
//    button->setBackGroundViewForState(CAControlStateAll, imageView);
//    button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
//    button->setTag(30);
//    this->getView()->addSubview(button);
    
    // segment view
    FSegmentView* seg =
        FSegmentView::createWithFrame(DRect((m_winSize.width - 400) / 2, 40, 500, 60), SEG_PAGE);
    seg->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
    seg->setItemTile("DemoJam", 0);
    seg->setItemTile("SAP Voice", 1);
    seg->setItemTile("Hiking", 2);
    seg->setTag(200, 0);
    seg->setTag(201, 1);
    seg->setTag(202, 2);
    this->getView()->addSubview(seg);
    
    
    for (int i = 0; i < SEG_PAGE; i++)
    {
        m_segView[i] = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
        this->getView()->addSubview(m_segView[i]);
        
        imageView = CAImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(120)));
        imageView->setImage(CAImage::create(crossapp_format_string("common/vote_%d_bg.jpg", i)));
        imageView->setTouchEnabled(false);
        m_segView[i]->addSubview(imageView);
    }
    m_navType = 0;
    m_segView[0]->setVisible(true);
    m_segView[1]->setVisible(false);
    m_segView[2]->setVisible(false);
    
    
   //CAApplication::getApplication()->getAccelerometer()->setDelegate(this);
    
    requestMsg();
    //initMsgTableView();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void VoteViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void VoteViewController::initMsgTableView()
{
    if (m_demoMsg.empty() && m_voiceMsg.empty() && m_hikMsg.empty())
    {
        showAlert();
        return;
    }
    
//    CALabel* label = CALabel::createWithFrame(DRect(0, _px(150), m_winSize.width, _px(100)));
//    label->setText("03:20:28");
//    label->setTextAlignment(CATextAlignmentCenter);
//    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
//    label->setColor(CAColor_white);
//    label->setFontSize(_px(80));
//    this->getView()->addSubview(label);
    
    float whRatio = 1.33f;
    m_imageHeight = (m_winSize.width - _px(40)) * whRatio;
    for (int i = 0; i < SEG_PAGE; i++)
    {
        CAScrollView* scrollView = CAScrollView::createWithFrame(DRect(_px(20), _px(0), m_winSize.width - _px(40), m_imageHeight + _px(320)));
        //scrollView->setViewSize(DSize(m_winSize.width - _px(40), (m_imageHeight));
        scrollView->setHorizontalScrollEnabled(false);
        scrollView->setVerticalScrollEnabled(true);
        scrollView->setBounceHorizontal(false);
        scrollView->setBounds(DRect(0, 0, m_winSize.width - _px(40), m_winSize.height - _px(120)));
        scrollView->setAnchorPoint(DPoint(0.f, 0.f));
        scrollView->setShowsHorizontalScrollIndicator(false);
        scrollView->setShowsVerticalScrollIndicator(false);
        scrollView->setBackGroundColor(CAColor_clear);
        m_segView[i]->addSubview(scrollView);
        
        DRect r = DRect(_px(0), _px(150), m_winSize.width - _px(40), m_imageHeight - _px(150));
        m_pageView[i] = CAPageView::createWithFrame(r, CAPageViewDirectionHorizontal);
        m_pageView[i]->setPageViewDelegate(this);
        m_pageView[i]->setCurrPage(0, false);
        m_pageView[i]->setBackGroundColor(CAColor_clear);
        scrollView->addSubview(m_pageView[i]);
        
        r = DRect((m_winSize.width - _px(300)) / 2, m_imageHeight + _px(10), _px(300), _px(50));
        m_pageControl[i] = CAPageControl::createWithFrame(r);
        m_pageControl[i]->setTag(300 + i);
        m_pageControl[i]->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack));
        if (i == 0)
        {
            m_pageControl[i]->setNumberOfPages((int)m_demoMsg.size());
        }
        else if (i == 1)
        {
            m_pageControl[i]->setNumberOfPages((int)m_voiceMsg.size());
        }
        else if (i == 2)
        {
            m_pageControl[i]->setNumberOfPages((int)m_hikMsg.size());
        }
        m_pageControl[i]->setCurrentPage(0);
        m_pageControl[i]->setTouchEnabled(false);
        scrollView->addSubview(m_pageControl[i]);
        
        m_detailView[i] = CAView::createWithFrame(DRect(_px(40), m_imageHeight + _px(40), m_winSize.width - _px(80), _px(300)));
        m_detailView[i]->setTouchEnabled(false);
        m_detailView[i]->setColor(CAColor_clear);
        scrollView->addSubview(m_detailView[i]);
    }
    
    CAVector<CAView* > viewList;
    for (int i = 0; i < m_demoMsg.size(); i++)
    {
        DRect r(0, 0, m_winSize.width - _px(40), m_imageHeight);
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(r);
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setImage(CAImage::create("common/bg.png"));
        temImage->setUrl(m_demoMsg[i].m_imageUrl);
        //temImage->setTouchEnabled(false);
        
        CALabel* label = CALabel::createWithFrame(DRect(_px(30), _px(30), _px(250), _px(40)));
        label->setText(m_demoMsg[i].m_teamName);
        label->setFontSize(_px(33));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        label = CALabel::createWithFrame(DRect(_px(30), _px(80), _px(150), _px(30)));
        label->setText(m_demoMsg[i].m_department);
        label->setFontSize(_px(25));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        
        r = DRect((m_winSize.width - _px(200)) / 2,  m_imageHeight - _px(300), _px(200), _px(62));
        CAButton* button = CAButton::createWithFrame(r, CAButtonTypeCustom);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("vote/d_vote_btn.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
        
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
        if (uInfo->m_demoVoteIdVec.size() >= 2)
        {
            button->setControlStateDisabled();
        }
        bool cVote = true;
        for (int j = 0; j < uInfo->m_demoVoteIdVec.size(); j++)
        {
            if (m_demoMsg[i].m_projectId == uInfo->m_demoVoteIdVec[j])
            {
                cVote = false;
                button->setTitleForState(CAControlStateAll, "Voted");
                break;
            }
        }
        if (cVote)
        {
            button->setTitleForState(CAControlStateAll, "Vote");
        }
        
        button->setTitleFontName("fonts/arial.ttf");
        button->setTitleColorForState(CAControlStateAll, CAColor_white);
        button->setBackGroundViewForState(CAControlStateAll, imageView);
        button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
        button->setTag(400 + i);
        temImage->addSubview(button);
        
        viewList.pushBack(temImage);

    }
    m_pageView[0]->setViews(viewList);
    
    viewList.clear();
    for (int i = 0; i < m_voiceMsg.size(); i++)
    {
        DRect r(0, 0, m_winSize.width - _px(40), m_imageHeight);
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(r);
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setImage(CAImage::create("common/bg.png"));
        temImage->setUrl(m_voiceMsg[i].m_imageUrl);
        //temImage->setTouchEnabled(false);
        
        CALabel* label = CALabel::createWithFrame(DRect(_px(30), _px(30), _px(250), _px(40)));
        label->setText(m_voiceMsg[i].m_playerName);
        label->setFontSize(_px(33));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        label = CALabel::createWithFrame(DRect(_px(30), _px(80), _px(150), _px(30)));
        label->setText(m_voiceMsg[i].m_projectName);
        label->setFontSize(_px(25));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        
        r = DRect((m_winSize.width - _px(200)) / 2,  m_imageHeight - _px(300), _px(200), _px(62));
        CAButton* button = CAButton::createWithFrame(r, CAButtonTypeCustom);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("vote/v_vote_btn.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
        
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
        if (uInfo->m_voiceVoteIdVec.size() >= 2)
        {
            button->setControlStateDisabled();
        }
        
        bool cVote = true;
        for (int j = 0; j < uInfo->m_voiceVoteIdVec.size(); j++)
        {
            if (m_voiceMsg[i].m_projectId == uInfo->m_voiceVoteIdVec[j])
            {
                cVote = false;
                button->setTitleForState(CAControlStateAll, "Voted");
                break;
            }
        }
        if (cVote)
        {
            button->setTitleForState(CAControlStateAll, "Vote");
        }
        button->setTitleFontName("fonts/arial.ttf");
        button->setTitleColorForState(CAControlStateAll, CAColor_white);
        button->setBackGroundViewForState(CAControlStateAll, imageView);
        button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
        button->setTag(500 + i);
        temImage->addSubview(button);
        
        viewList.pushBack(temImage);
    }
    m_pageView[1]->setViews(viewList);
    
    viewList.clear();
    for (int i = 0; i < m_hikMsg.size(); i++)
    {
        DRect r(0, 0, m_winSize.width - _px(40), m_imageHeight);
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(r);
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setImage(CAImage::create("common/bg.png"));
        temImage->setUrl(m_hikMsg[i].m_imageUrl);
        //temImage->setTouchEnabled(false);
        
        CALabel* label = CALabel::createWithFrame(DRect(_px(30), _px(30), _px(250), _px(40)));
        label->setText(m_hikMsg[i].m_playerName);
        label->setFontSize(_px(33));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        label = CALabel::createWithFrame(DRect(_px(30), _px(80), _px(150), _px(30)));
        label->setText(m_hikMsg[i].m_projectName);
        label->setFontSize(_px(25));
        label->setTouchEnabled(false);
        label->setColor(CAColor_white);
        temImage->addSubview(label);
        
        r = DRect((m_winSize.width - _px(200)) / 2,  m_imageHeight - _px(300), _px(200), _px(62));
        CAButton* button = CAButton::createWithFrame(r, CAButtonTypeCustom);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("vote/v_vote_btn.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
        
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
        if (uInfo->m_voiceVoteIdVec.size() >= 2)
        {
            button->setControlStateDisabled();
        }
        
        bool cVote = true;
        for (int j = 0; j < uInfo->m_voiceVoteIdVec.size(); j++)
        {
            if (m_hikMsg[i].m_projectId == uInfo->m_voiceVoteIdVec[j])
            {
                cVote = false;
                button->setTitleForState(CAControlStateAll, "Voted");
                break;
            }
        }
        if (cVote)
        {
            button->setTitleForState(CAControlStateAll, "Vote");
        }
        button->setTitleFontName("fonts/arial.ttf");
        button->setTitleColorForState(CAControlStateAll, CAColor_white);
        button->setBackGroundViewForState(CAControlStateAll, imageView);
        button->addTarget(this, CAControl_selector(VoteViewController::buttonCallBack), CAControlEventTouchUpInSide);
        button->setTag(500 + i);
        temImage->addSubview(button);
        
        viewList.pushBack(temImage);
    }
    m_pageView[2]->setViews(viewList);

    setDetailView(0, 0);
    setDetailView(1, 0);
    setDetailView(2, 0);
    //refreshPageView(0);
}

void VoteViewController::setDetailView(int type, int index)
{
    if (type == 0 && m_demoMsg.empty())
    {
        return;
    }
    else if (type == 1 && m_voiceMsg.empty())
    {
        return;
    }
    else if (type == 2 && m_hikMsg.empty())
    {
        return;
    }
    m_detailView[type]->removeAllSubviews();
    
    DSize _size = DSize(m_winSize.width - _px(80), _px(300));
    CALabel* label = CALabel::createWithFrame(DRect(0, _px(0), _size.width, _px(300)));
    label->setColor(CAColor_white);
    label->setTextAlignment(CATextAlignmentLeft);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    label->setFontSize(_px(32));
    
    if(type == 0) {
        label->setText(m_demoMsg[index].m_teamDetail.c_str());
    } else if (type == 1){
        label->setText(m_voiceMsg[index].m_teamDetail.c_str());
    } else if (type == 2) {
        label->setText(m_hikMsg[index].m_teamDetail.c_str());
    }
    m_detailView[type]->addSubview(label);
}

void VoteViewController::refreshPageView(int index)
{
//    m_pageType = index;

//    m_pageView->setCurrPage(m_currentPage[m_pageType], false);
//    if (m_pageControl[m_pageType])
//    {
//        m_pageControl[m_pageType]->setCurrentPage(m_currentPage[m_pageType]);
//    }
//    m_segBgView[index]->setVisible(true);
//    m_segBgView[1 - index]->setVisible(false);
//    
//    setDetailView(m_pageType, m_currentPage[m_pageType]);
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
        m_navType = 0;
        m_segView[0]->setVisible(true);
        m_segView[1]->setVisible(false);
        m_segView[2]->setVisible(false);
    }
    else if (btn->getTag() == 201)
    {
        m_navType = 1;
        m_segView[0]->setVisible(false);
        m_segView[1]->setVisible(true);
        m_segView[2]->setVisible(false);
    }
    else if (btn->getTag() == 202)
    {
        m_navType = 2;
        m_segView[0]->setVisible(false);
        m_segView[1]->setVisible(false);
        m_segView[2]->setVisible(true);
    }
    else if (btn->getTag() == 300) // page control
    {
        CAPageControl* control = (CAPageControl*)btn;
        setDetailView(0, control->getCurrentPage());
        m_pageView[0]->setCurrPage(control->getCurrentPage(), true);
    }
    else if (btn->getTag() == 301) // page control
    {
        CAPageControl* control = (CAPageControl*)btn;
        m_pageView[1]->setCurrPage(control->getCurrentPage(), true);
    }
    else if (btn->getTag() >= 400 && btn->getTag() < 500)
    {
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
        if (uInfo->m_demoVoteIdVec.size() >= 2)
        {
            return;
        }
        VoteShakeViewController* vc = new VoteShakeViewController(&m_demoMsg[btn->getTag() - 400], NULL);
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() >= 500 && btn->getTag() < 600)
    {
        userInfo* uInfo = FDataManager::getInstance()->getUserInfo();
        if (uInfo->m_voiceVoteIdVec.size() >= 2)
        {
            return;
        }
        VoteShakeViewController* vc = new VoteShakeViewController(NULL, &m_voiceMsg[btn->getTag() - 500]);
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    
}

void VoteViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        m_demoMsg.clear();
        m_voiceMsg.clear();
        
        int length = value["dl"].size();
        for (int index = 0; index < length; index++)
        {
            demoJamMsg temp_msg;
            temp_msg.m_projectId = value["dl"][index]["DemoJamItemId"].asInt();
            temp_msg.m_teamName = value["dl"][index]["TeamName"].asString();
            temp_msg.m_department = value["dl"][index]["Department"].asString();
            temp_msg.m_teamDetail = value["dl"][index]["Introduction"].asString();
            temp_msg.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["dl"][index]["Resource"].asCString());
            m_demoMsg.push_back(temp_msg);
        }
        
        length = value["vl"].size();
        for (int index = 0; index < length; index++)
        {
            voiceMsg temp_msg;
            temp_msg.m_projectId = value["vl"][index]["VoiceItemId"].asInt();
            temp_msg.m_playerName = value["vl"][index]["VoicerName"].asString();
            temp_msg.m_projectName = value["vl"][index]["SongName"].asString();
            temp_msg.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["vl"][index]["VoicerPic"].asCString());
            temp_msg.m_teamDetail = value["vl"][index]["VoicerDes"].asString();
            m_voiceMsg.push_back(temp_msg);
            // test code
            if (index == 2) {
                m_hikMsg.push_back(temp_msg);
            }
        }
    }
    else
    {
        showAlert();
    }
#ifdef LOCALTEST
    {
        struct cc_timeval tp;
        getTimeval(&tp, NULL);
        for (int index = 0; index < 3; index++)
        {
            demoJamMsg temp_msg;
            temp_msg.m_projectId = index;
            temp_msg.m_teamName = "TEAM 1";
            temp_msg.m_department = "CI&SP";
            temp_msg.m_teamDetail = "This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum. This is Photoshop's version of Lorem Ipsum.";
            temp_msg.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
            m_demoMsg.push_back(temp_msg);
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
    //if(m_navType == 0)
    setDetailView(m_navType, pageView->getCurrPage());
    m_pageControl[m_navType]->setCurrentPage(pageView->getCurrPage());
    m_pageControl[m_navType]->updateCurrentPageDisplay();
}
//
//void VoteViewController::didAccelerate(CCAcceleration* pAccelerationValue)
//{
//    float nowGX = (pAccelerationValue->x)*9.81f;
//    float nowGY = (pAccelerationValue->y)*9.81f;
//    
//    float dt = 30.f;
//    if(m_canVote && (nowGX<-dt||nowGX>dt || nowGY<-dt||nowGY>dt))
//    {
//        m_canVote = false;
//        //m_voteLabel->setText("vote success !");
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/vote.wav");
//    }
//    
//}
