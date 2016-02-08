
#include "NoticeViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FServerTime.h"

NoticeViewController::NoticeViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_msgTableView(NULL)
{
    m_msg.clear();
    CADevice::getLocalNotificationList(m_msgList);
}

NoticeViewController::~NoticeViewController()
{
    for (int i = 0; i < m_msgList.size(); i++)
    {
        if (m_msgList[i].startTime <= getTimeSecond())
        {
            CADevice::cancelLocalNotification(m_msgList[i].nid.c_str());
        }
    }
}

void NoticeViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(NoticeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
//    
//    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(140), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
//    imageView = CAImageView::createWithImage(CAImage::create("common/nav_forward.png"));
//    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
//    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
//    button->setBackGroundViewForState(CAControlStateAll, imageView);
//    button->addTarget(this, CAControl_selector(NoticeViewController::buttonCallBack), CAControlEventTouchUpInSide);
//    button->setTag(30);
//    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(SAP_TITLE_FONT_SIZE);
    label->setText("Notification");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);
    
    
    m_msgTableView = CATableView::createWithFrame(DRect(0, _px(120), m_winSize.width, m_winSize.height - _px(120)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
    //m_msgTableView->setSeparatorViewHeight(_px(2));
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
	if (m_msg.empty())
    {
        showAlert();
        return;
    }
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
    
    p_alertView = CAView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(120)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(NoticeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}


CATableViewCell* NoticeViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));

		CAImageView* imageView = CAImageView::createWithImage(CAImage::create(crossapp_format_string("notice/level_%d.png", row % 2)));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
		imageView->setFrame(DRect(_px(40), _px(20), _px(80), _px(80)));
		cell->addSubview(imageView);

		//if (m_msg[row].closed == false)
		{
			imageView = CAImageView::createWithImage(CAImage::create("notice/icon_reddot.png"));
			imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
			imageView->setFrame(DRect(_px(100), _px(20), _px(10), _px(10)));
			cell->addSubview(imageView);
		}

		CALabel* label = CALabel::createWithFrame(DRect(_px(160), _px(20), _px(200), _px(40)));
		label->setText(m_msgList[row].title);
		label->setFontSize(_px(35));
		label->setColor(CAColor_black);
		cell->addSubview(label);

		label = CALabel::createWithFrame(DRect(_px(160), _px(70), _px(200), _px(30)));
		label->setText(crossapp_format_string("%s", timeToString(m_msgList[row].startTime).c_str()));
		label->setFontSize(_px(28));
		label->setColor(CAColor_gray);
		cell->addSubview(label);

		imageView = CAImageView::createWithImage(CAImage::create("common/btn_rightarrow.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
		imageView->setFrame(DRect(_size.width - _px(90), (_size.height - _px(50)) / 2, _px(50), _px(50)));
		cell->addSubview(imageView);

        //CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
        //sView->setFrame(DRect(_px(0), _px(0), _size.width, _size.height));
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
    return _px(120);
}

void NoticeViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void NoticeViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}
