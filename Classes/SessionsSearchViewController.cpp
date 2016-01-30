
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
  m_type(type),
  m_navTrackType(0),
  m_navFormatType(0)
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

	///
	m_filterView = CAView::createWithFrame(DRect(0, _px(120), m_winSize.width, _px(60)));
	CAScale9ImageView* sImageView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
	sImageView->setFrame(DRect(0, 0, m_winSize.width, _px(60)));
	m_filterView->addSubview(sImageView);
	this->getView()->addSubview(m_filterView);

	for (int i = 0; i < 2; i++)
	{
		CAButton* button = CAButton::createWithFrame(DRect(i * m_winSize.width / 2, 0, m_winSize.width / 2, _px(60)), CAButtonTypeCustom);
		button->setTitleForState(CAControlStateAll, filterItem[i]);
		button->setTitleFontName("fonts/arial.ttf");
        button->setTitleFontSize(_px(30));
		button->setTitleColorForState(CAControlStateAll, CAColor_gray);
        button->setTitleColorForState(CAControlStateSelected, CAColor_white);
		button->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTag(300 + i);
		button->setAllowsSelected(true);
		m_filterView->addSubview(button);

		m_downView[i] = CAView::createWithFrame(DRect(i * m_winSize.width / 2, _px(180), m_winSize.width / 2, _px(50) * TrackNum + _px(20)));
		CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
		imageView->setFrame(DRect(0, 0, m_winSize.width / 2, _px(50) * TrackNum + _px(20)));
		m_downView[i]->addSubview(imageView);
		m_downView[i]->setVisible(false);
		this->getView()->addSubview(m_downView[i]);
	}

	for (int i = 0; i < TrackNum; i++)
	{
		CAButton* button = CAButton::createWithFrame(DRect(_px(20), _px(50) * i, m_winSize.width / 2 - _px(40), _px(50)), CAButtonTypeCustom);
		button->setTitleForState(CAControlStateAll, trackFilterItem[i]);
		button->setTitleFontName("fonts/arial.ttf");
		button->setTitleColorForState(CAControlStateAll, CAColor_gray);
        button->setTitleColorForState(CAControlStateSelected, CAColor_white);
		button->setTitleFontSize(_px(27));
		button->setAllowsSelected(true);
		//CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/white_bg.png"));
		//button->setBackGroundViewForState(CAControlStateAll, imageView);
		imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
		button->setBackGroundViewForState(CAControlStateSelected, imageView);
		button->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTag(400 + i);
		m_downView[0]->addSubview(button);
		m_trackButtonVec.push_back(button);
	}

	for (int i = 0; i < FormatNum; i++)
	{
		CAButton* button = CAButton::createWithFrame(DRect(_px(20), _px(50) * i, m_winSize.width / 2 - _px(40), _px(50)), CAButtonTypeCustom);
		button->setTitleForState(CAControlStateAll, formatFilterItem[i]);
		button->setTitleFontName("fonts/arial.ttf");
		button->setTitleColorForState(CAControlStateAll, CAColor_gray);
		button->setTitleFontSize(_px(27));
		button->setAllowsSelected(true);
		//CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/white_bg.png"));
		//button->setBackGroundViewForState(CAControlStateAll, imageView);
		imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
		button->setBackGroundViewForState(CAControlStateSelected, imageView);
		button->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTag(500 + i);
		m_downView[1]->addSubview(button);
		m_formatButtonVec.push_back(button);
	}

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionsSearchViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SessionsSearchViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 300)
	{
		if (btn->getControlState() == CAControlStateSelected)
		{
			CAButton* button = (CAButton*)m_filterView->getSubviewByTag(301);
			button->setControlState(CAControlStateNormal);
			m_downView[0]->setVisible(true);
		}
		else
		{
			m_downView[0]->setVisible(false);
		}
		m_downView[1]->setVisible(false);
	}
	else if (btn->getTag() == 301)
	{
		if (btn->getControlState() == CAControlStateSelected)
		{
			CAButton* button = (CAButton*)m_filterView->getSubviewByTag(300);
			button->setControlState(CAControlStateNormal);
			m_downView[1]->setVisible(true);
		}
		else
		{
			m_downView[1]->setVisible(false);
		}
		m_downView[0]->setVisible(false);
	}
	else
	{
		CAButton* button = (CAButton*)m_filterView->getSubviewByTag(300);
		button->setControlState(CAControlStateNormal);
		button = (CAButton*)m_filterView->getSubviewByTag(301);
		button->setControlState(CAControlStateNormal);
		m_downView[0]->setVisible(false);
		m_downView[1]->setVisible(false);

	}


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
            string str3 = crossapp_format_string("%d", it->m_sessionId);
            transform(str3.begin(), str3.end(), str3.begin(), ::tolower);
            if ((str0.find(str) != string::npos ||
                 strb.find(str) != string::npos ||
                 str1.find(str) != string::npos ||
                 str2.find(str) != string::npos ||
                 str3.find(str) != string::npos))
            {
				m_msgSearchFilter.push_back(&(*it));
            }
        }
		refreshTableByFormat(m_navTrackType, m_navFormatType);
        m_msgTableView->reloadData();
	}
	else if (btn->getTag() >= 400 && btn->getTag() < 500)
	{
		if (btn->getControlState() == CAControlStateNormal)
		{
			m_navTrackType = 0;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
		else if (btn->getControlState() == CAControlStateSelected)
		{
			for (int i = 0; i < TrackNum; i++)
			{
				if (m_trackButtonVec[i] == btn)
					continue;
				m_trackButtonVec[i]->setControlState(CAControlStateNormal);
			}
			m_navTrackType = btn->getTag() - 400;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
	}
	else if (btn->getTag() >= 500 && btn->getTag() < 600)
	{
		if (btn->getControlState() == CAControlStateNormal)
		{
			m_navFormatType = 0;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
		else if (btn->getControlState() == CAControlStateSelected)
		{
			for (int i = 0; i < TrackNum; i++)
			{
				if (m_formatButtonVec[i] == btn)
					continue;
				m_formatButtonVec[i]->setControlState(CAControlStateNormal);
			}
			m_navFormatType = btn->getTag() - 500;
			refreshTableByFormat(m_navTrackType, m_navFormatType);
		}
	}
}

void SessionsSearchViewController::refreshTableByFormat(int track, int format)
{
	m_msgFilter.clear();
	if (track == 0 && format == 0)
	{
		for (std::vector<sessionMsg*>::iterator it = m_msgSearchFilter.begin(); it != m_msgSearchFilter.end(); it++)
		{
			m_msgFilter.push_back(*it);
		}
	}
	else if (track == 0)
	{
		for (std::vector<sessionMsg*>::iterator it = m_msgSearchFilter.begin(); it != m_msgSearchFilter.end(); it++)
		{
			if (!strcmp((*it)->m_format.c_str(), formatFilterItem[format]))
			{
				m_msgFilter.push_back(*it);
			}
		}
	}
	else if (format == 0)
	{
		for (std::vector<sessionMsg*>::iterator it = m_msgSearchFilter.begin(); it != m_msgSearchFilter.end(); it++)
		{
			if (!strcmp((*it)->m_track.c_str(), trackFilterItem[track]))
			{
				m_msgFilter.push_back(*it);
			}
		}
	}
	else
	{
		for (std::vector<sessionMsg*>::iterator it = m_msgSearchFilter.begin(); it != m_msgSearchFilter.end(); it++)
		{
			if (!strcmp((*it)->m_format.c_str(), formatFilterItem[format]) && !strcmp((*it)->m_track.c_str(), trackFilterItem[track]))
			{
				m_msgFilter.push_back(*it);
			}
		}
	}

	if (m_msgTableView)
	{
		m_msgTableView->setFrame(DRect(0, _px(180), m_winSize.width, m_winSize.height - _px(180)));
		m_msgTableView->reloadData();
	}
}

CATableViewCell* SessionsSearchViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
	DSize _size = cellSize;
    MainViewTableCell* cell = NULL;//dynamic_cast<MainViewTableCell*>();
	if (cell == NULL)
	{
        table->dequeueReusableCellWithIdentifier(crossapp_format_string("%d", m_msgFilter[row]->m_sessionId).c_str());
		cell = MainViewTableCell::create(crossapp_format_string("%d", m_msgFilter[row]->m_sessionId).c_str(), DRect(0, 0, _size.width, _size.height));
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


