
#include "SessionsSearchViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FDataManager.h"

SessionsSearchViewController::SessionsSearchViewController(int type)
: m_alertView(NULL),
  m_pLoading(NULL),
  m_type(type)
{
    m_msg = FDataManager::getInstance()->getSessionMsgs();
}

SessionsSearchViewController::~SessionsSearchViewController()
{

}

void SessionsSearchViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    m_searchTextField = CATextField::createWithFrame(DRect(_px(150), _px(40), m_winSize.width - _px(310), _px(60)));
    m_searchTextField->setTextColor(ccc4(0xa0, 0xa0, 0xa0, 0xff));
    m_searchTextField->setFontSize(_px(26));
    m_searchTextField->setFontName("fonts/arial.ttf");
    this->getView()->addSubview(m_searchTextField);
    
    button = CAButton::createWithFrame(DRect(m_winSize.width - _px(140), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    imageView = CAImageView::createWithImage(CAImage::create("common/nav_search.png"));
    //imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    //imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(30);
    this->getView()->addSubview(button);

    
    m_msgTableView = CATableView::createWithFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
    //m_msgTableView->setSeparatorViewHeight(_px(2));
    this->getView()->addSubview(m_msgTableView);
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionsSearchViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SessionsSearchViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 20)
	{
		RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else if (btn->getTag() == 30)
	{
        m_msgFilter.clear();
        string str = m_searchTextField->getText();
        if (str == "")
        {
            return;
        }
        for (std::vector<sessionMsg>::iterator it = m_msg->begin(); it != m_msg->end(); it++)
        {
            if(m_type == 1 && !it->m_stored) continue;
            transform(str.begin(), str.end(), str.begin(), ::tolower);
            CCLog("%s", str.c_str());
            string str0 = it->m_title;
            transform(str0.begin(), str0.end(), str0.begin(), ::tolower);
            string strb = it->m_location;
            transform(strb.begin(), strb.end(), strb.begin(), ::tolower);
            string str1 = it->m_track;
            transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
            string str2 = it->m_format;
            transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
            if ((str0.find(str)!= string::npos ||
                 strb.find(str)!= string::npos ||
                 str1.find(str)!= string::npos ||
                 str2.find(str) != string::npos))
            {
                m_msgFilter.push_back(&(*it));
            }
        }
        m_msgTableView->reloadData();
	}
}

CATableViewCell* SessionsSearchViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
	DSize _size = cellSize;
	MainViewTableCell* cell = dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
	if (cell == NULL)
	{
		cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
		cell->initWithCell(*m_msgFilter[row]);
	}

	return cell;
}

unsigned int SessionsSearchViewController::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int SessionsSearchViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return (int)m_msgFilter.size();
}

unsigned int SessionsSearchViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return _px(240);
}

void SessionsSearchViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    SessionDetailViewController* vc = new SessionDetailViewController(*m_msgFilter[row]);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void SessionsSearchViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}


