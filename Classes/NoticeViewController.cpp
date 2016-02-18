
#include "NoticeViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FServerTime.h"
#include "FNoticeManager.h"
#include "NoticeDetailViewController.h"

NoticeViewController::NoticeViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_msgTableView(NULL)
{
    
}

NoticeViewController::~NoticeViewController()
{
//    for (int i = 0; i < m_msgList.size(); i++)
//    {
//        FNoticeManager::sharedFNoticeManager()->readNotice(atoi(m_msgList[i].nid.c_str()));
//    }
}

void NoticeViewController::viewDidAppear()
{
    std::vector<FNotice>& notices = FNoticeManager::sharedFNoticeManager()->getNotices();
    m_msgList.clear();
    for (int i = 0; i < notices.size(); i++)
    {
        if (notices[i].startTime > getTimeSecond() + PRETIME)
        {
            continue;
        }
        LocalNoticeMsg tmp;
        tmp.nid = crossapp_format_string("%d", notices[i].sid);
        tmp.type = notices[i].type;
        tmp.title = notices[i].title;
        tmp.startTime = notices[i].startTime;
        tmp.readed = notices[i].readed;
        m_msgList.push_back(tmp);
    }
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}

void NoticeViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    m_msg = FDataManager::getInstance()->getSessionMsgs();
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect((0), (0), m_winSize.width, (120)));
    this->getView()->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(NoticeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
//    
//    button = CAButton::createWithFrame(DRect(m_winSize.width - (140), (20), (100), (100)), CAButtonTypeCustom);
//    imageView = CAImageView::createWithImage(CAImage::create("common/nav_forward.png"));
//    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
//    imageView->setFrame(DRect((20), (20), (80), (80)));
//    button->setBackgroundViewForState(CAControlStateAll, imageView);
//    button->addTarget(this, CAControl_selector(NoticeViewController::buttonCallBack), CAControlEventTouchUpInSide);
//    button->setTag(30);
//    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Notification");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);
    
    
    m_msgTableView = CATableView::createWithFrame(DRect(0, (120), m_winSize.width, m_winSize.height - (120)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
    //m_msgTableView->setSeparatorViewHeight((2));
    this->getView()->addSubview(m_msgTableView);
    
    //requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void NoticeViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void NoticeViewController::initMsgTableView()
{
    
}

void NoticeViewController::requestMsg()
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
	key_value["tag"] = noticeTag[0];
	key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(NoticeViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(NoticeViewController::initMsgTableView));
    }
}

void NoticeViewController::buttonCallBack(CAControl* btn, DPoint point)
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
}

void NoticeViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
    }
    else
    {
        //showAlert();
    }
#ifdef LOCALTEST
    {
		for (int i = 0; i < 5; i++)
		{
			noticeMsg temp;
			temp.closed = false;
			temp.noticeType = 0;
			temp.title = "GASS";
			temp.startTime = getTimeSecond();
			temp.endTime = getTimeSecond();
			temp.noticeId = i;
			m_msg.push_back(temp);
		}
    }
#endif
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}


void NoticeViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (120)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect((0), (120), m_winSize.width, m_winSize.height - (120)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect((0), (0), m_winSize.width, m_winSize.height - (120)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(NoticeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - (400), m_winSize.width, (30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize((24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}


CATableViewCell* NoticeViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    
    table->dequeueReusableCellWithIdentifier("CrossApp");
    
    CATableViewCell* cell = NULL;
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));

		CAImageView* imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("notice/level_%d.png", row % 2)));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
		imageView->setFrame(DRect((40), (20), (80), (80)));
		cell->addSubview(imageView);
        
        CommonUrlImageView* urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
        if (urlImageView) {
            urlImageView->setFrame(DRect((40), (20), (80), (80)));
            urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            urlImageView->setUrl(crossapp_format_string("%s%s", imgPreUrl.c_str(), noticeIcon[m_msgList[row].type]));
            cell->addSubview(urlImageView);
        }

		if (m_msgList[row].readed == false)
		{
			imageView = CAImageView::createWithImage(CAImage::create("notice/icon_reddot.png"));
			imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
			imageView->setFrame(DRect((100), (20), (10), (10)));
			cell->addSubview(imageView);
		}

		CALabel* label = CALabel::createWithFrame(DRect((160), (20), m_winSize.width - (200), (40)));
		label->setText(m_msgList[row].title);
		label->setFontSize((30));
		label->setColor(CAColor_black);
		cell->addSubview(label);

		label = CALabel::createWithFrame(DRect((160), (70), (200), (30)));
		label->setText(crossapp_format_string("%s", timeToString(m_msgList[row].startTime).c_str()));
		label->setFontSize((25));
		label->setColor(CAColor_gray);
		cell->addSubview(label);

		imageView = CAImageView::createWithImage(CAImage::create("common/btn_rightarrow.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
		imageView->setFrame(DRect(_size.width - (90), (_size.height - (50)) / 2, (50), (50)));
		cell->addSubview(imageView);

        //CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
        //sView->setFrame(DRect((0), (0), _size.width, _size.height));
        //cell->setBackgroundView(sView);
    }
    //cell->setModel(m_msgFilter[row]);
    
    return cell;
    
}

unsigned int NoticeViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int NoticeViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return (int)m_msgList.size();
}

unsigned int NoticeViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return (120);
}

void NoticeViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if (m_msgList[row].type == notice_session)
    {
        for (int i = 0; i < m_msg->size(); i++)
        {
            if (m_msg->at(i).m_sessionId == atoi(m_msgList[row].nid.c_str()))
            {
                SessionDetailViewController* vc = new SessionDetailViewController(m_msg->at(i));
                vc->init();
                vc->autorelease();
                RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
                
                FNoticeManager::sharedFNoticeManager()->readNotice(atoi(m_msgList[row].nid.c_str()));
                break;
            }
        }
        
    }
    else
    {
        NoticeDetailViewController* vc = new NoticeDetailViewController(m_msgList[row].title);
        vc->init();
        vc->autorelease();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
        
        FNoticeManager::sharedFNoticeManager()->readNotice(atoi(m_msgList[row].nid.c_str()));
    }
    
}

void NoticeViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}
