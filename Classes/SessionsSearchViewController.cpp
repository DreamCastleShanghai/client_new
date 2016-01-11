
#include "SessionsSearchViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

SessionsSearchViewController::SessionsSearchViewController()
{
	m_alertView = NULL;
	m_pLoading = NULL;
}

SessionsSearchViewController::~SessionsSearchViewController()
{

}

void SessionsSearchViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;

	CAScale9ImageView* tabBg = CAScale9ImageView::createWithImage(CAImage::create("common/yellow_bg.png"));
	tabBg->setCapInsets(DRect(1, 1, 1, 1));
	tabBg->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(200)));
	this->getView()->addSubview(tabBg);

	CAButton* btn = CAButton::createWithFrame(DRect(_px(20), _px(50), _px(64), _px(64)), CAButtonTypeRoundedRect);
	CAImageView* imageView = CAImageView::createWithImage(CAImage::create("source_material/btn_left_white.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	btn->setTag(100);
	btn->setBackGroundViewForState(CAControlStateAll, imageView);
	btn->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	CALabel* mc = CALabel::createWithFrame(DRect(_px(120), _px(62), _px(200), _px(50)));
	mc->setFontSize(_px(40));
	mc->setText("Search");
	mc->setColor(CAColor_white);
	mc->setFontName("fonts/arial.ttf");
	this->getView()->addSubview(mc);

	m_searchTextField = CATextField::createWithFrame(DRect(40, _px(120), m_winSize.width - 178, _px(70)));
	m_searchTextField->setTextColor(CAColor_orange);
	m_searchTextField->setFontSize(_px(40));
	m_searchTextField->setPlaceHolder("Customer");
	this->getView()->addSubview(m_searchTextField);

	btn = CAButton::createWithFrame(DRect(m_winSize.width - 100, _px(130), _px(50), _px(50)), CAButtonTypeRoundedRect);
	imageView = CAImageView::createWithImage(CAImage::create("session/search_btn.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	btn->setTag(200);
	btn->setBackGroundViewForState(CAControlStateAll, imageView);
	btn->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	CATableView* tableView = CATableView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	tableView->setTableViewDataSource(this);
	tableView->setTableViewDelegate(this);
	tableView->setScrollViewDelegate(this);
	tableView->setAllowsSelection(true);
	CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
	refreshDiscount->setLabelColor(CAColor_black);
	CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
	refreshDiscount1->setLabelColor(CAColor_black);
	tableView->setFooterRefreshView(refreshDiscount);
	tableView->setHeaderRefreshView(refreshDiscount1);
	m_msgTableView = tableView;
	m_pageViewCell = CAView::createWithFrame(DRect(0, _px(200), m_winSize.width, m_winSize.height - _px(200)));
	m_pageViewCell->addSubview(tableView);

	this->getView()->addSubview(m_pageViewCell);


	if (m_msg.empty())
	{
		requestMsg();

		m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
		m_pageViewCell->insertSubview(m_pLoading, CAWindowZOderTop);
		m_pLoading->setLoadingMinTime(0.5f);
	}

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionsSearchViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SessionsSearchViewController::requestMsg() // all(0) track(1) format(2) my_all(3)
{
	if (m_alertView) {
		m_pageViewCell->removeSubview(m_alertView);
		m_alertView = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
	key_value["type"] = "0";
	key_value["section"] = "0";
	key_value["limit"] = "20";
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsSearchViewController::onRequestFinished));
}

void SessionsSearchViewController::requestApendMsg()
{
	if (m_alertView) {
		m_pageViewCell->removeSubview(m_alertView);
		m_alertView = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
	key_value["type"] = "0";
	key_value["section"] = crossapp_format_string("%d", m_section);
	key_value["limit"] = "20";
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsSearchViewController::onRequestFinished));
}

void SessionsSearchViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
	requestMsg();
}

void SessionsSearchViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
	requestApendMsg();
}

void SessionsSearchViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 100)
	{
		RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else if (btn->getTag() == 200)
	{

	}
	else if (btn->getTag() == 500)
	{
		requestMsg();
		if (m_pLoading == NULL)
		{
			m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, 50, 50));
			m_pageViewCell->insertSubview(m_pLoading, CAWindowZOderTop);
			m_pLoading->setLoadingMinTime(0.5f);
		}
	}
}

void SessionsSearchViewController::showAlert()
{
	if (m_alertView) {
		m_pageViewCell->removeSubview(m_alertView);
		m_alertView = NULL;
	}

	m_alertView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	m_pageViewCell->addSubview(m_alertView);

	CAImageView* bg = CAImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	bg->setImage(CAImage::create("common/bg.png"));

	CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
	btn5->setTag(500);
	btn5->setCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, m_winSize.width, m_winSize.height - _px(200)));
	btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
	btn5->setBackGroundViewForState(CAControlStateNormal, bg);
	btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
	btn5->addTarget(this, CAControl_selector(SessionsSearchViewController::buttonCallBack), CAControlEventTouchUpInSide);
	m_alertView->addSubview(btn5);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(60)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	test->setFontSize(_px(40));
	test->setText("Network cannot connect!");
	m_alertView->addSubview(test);

}

void SessionsSearchViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];
		int length = value.size();
		m_msg.clear();

		for (int index = 3; index < length; index++)
		{
			sessionMsg temp_msg;
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
	else if (m_msg.empty())
	{
		//showAlert(id);
	}

	{
		m_msg.clear();
		for (int i = 0; i < 7; i++)
		{
			sessionMsg temp_msg;
			temp_msg.m_sessionId = 200 + i;
			temp_msg.m_title = "Customer Success";
			temp_msg.m_lecturer = "Lisa Chen";
			temp_msg.m_track = "Customer";
			temp_msg.m_format = "Dev Faire";
			cc_timeval ct;
			CCTime::gettimeofdayCrossApp(&ct, NULL);
			temp_msg.m_startTime = ct.tv_sec + 3500;
			temp_msg.m_likeNum = 20;
			temp_msg.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
			m_msg.push_back(temp_msg);
		}
	}

	if (m_pLoading)
	{
		m_pLoading->stopAnimating();
		m_pageViewCell->removeSubview(m_pLoading);
		m_pLoading = NULL;
	}

	if (m_msgTableView)
	{
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
		cell->initWithCell();
	}
	cell->setModel(m_msg[row]);

	return cell;
}

unsigned int SessionsSearchViewController::numberOfSections(CATableView *table)
{
	return 1;
}

unsigned int SessionsSearchViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return m_msg.size();
}

unsigned int SessionsSearchViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return _px(170);
}

void SessionsSearchViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    SessionDetailViewController* vc = new SessionDetailViewController(m_msg[row]);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void SessionsSearchViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}


