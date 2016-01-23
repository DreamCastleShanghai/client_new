
#include "MomentViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"

MomentViewController::MomentViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_segType(0)
, m_currentAllNum(0)
, m_currentMyNum(0)
{

}

MomentViewController::~MomentViewController()
{

}

void MomentViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(240)));
    this->getView()->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(20), _px(100), _px(100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
    button->setBackGroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Moments");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

	label = CALabel::createWithFrame(DRect(0, _px(160), m_winSize.width / 2, _px(40)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(40));
	label->setText("Photos");
	label->setFontName("fonts/arial.ttf");
	sView->addSubview(label);

	label = CALabel::createWithFrame(DRect(m_winSize.width / 2, _px(160), m_winSize.width / 2, _px(40)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize(_px(40));
	label->setText("My Posts");
	label->setFontName("fonts/arial.ttf");
	sView->addSubview(label);

	button = CAButton::createWithFrame(DRect(_px(0), _px(120), m_winSize.width / 2, _px(120)), CAButtonTypeCustom);
	button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(200);
	this->getView()->addSubview(button);

	button = CAButton::createWithFrame(DRect(m_winSize.width / 2, _px(120), m_winSize.width / 2, _px(120)), CAButtonTypeCustom);
	button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(201);
	this->getView()->addSubview(button);

	m_browView = CAScale9ImageView::createWithFrame(DRect(0, _px(230), m_winSize.width / 2, _px(10)));
	m_browView->setImage(CAImage::create("source_material/stepper_dec_n.png"));
	this->getView()->addSubview(m_browView);

	for (int i = 0; i < 2; i++)
	{
		m_segView[i] = CAView::createWithFrame(DRect(0, _px(240), m_winSize.width, m_winSize.height - _px(240)));
		this->getView()->addSubview(m_segView[i]);

	}
	m_segView[1]->setVisible(false);

	

	m_msgTableView = CATableView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(240)));
	m_msgTableView->setTableViewDataSource(this);
	m_msgTableView->setTableViewDelegate(this);
	m_msgTableView->setAllowsSelection(true);
	m_msgTableView->setScrollViewDelegate(this);
	m_msgTableView->setSeparatorColor(CAColor_clear);
	m_segView[0]->addSubview(m_msgTableView);

	CAPullToRefreshView* footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
	footerRefreshView->setTag(0);
	m_msgTableView->setFooterRefreshView(footerRefreshView);

	m_myCollectionView = CACollectionView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(240)));
	m_myCollectionView->setAllowsSelection(true);
	//p_Conllection->setAllowsMultipleSelection(true);
	m_myCollectionView->setCollectionViewDelegate(this);
	m_myCollectionView->setCollectionViewDataSource(this);
	m_myCollectionView->setScrollViewDelegate(this);
	//m_myCollectionView->setHoriInterval(_px(0));
	//m_myCollectionView->setVertInterval(_px(0));
	m_segView[1]->addSubview(m_myCollectionView);

	CAPullToRefreshView* headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
	footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
	headerRefreshView->setTag(1);
	footerRefreshView->setTag(1);
	m_myCollectionView->setHeaderRefreshView(headerRefreshView);
	m_myCollectionView->setFooterRefreshView(footerRefreshView);

    requestMsg(Type_all);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void MomentViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void MomentViewController::requestMsg(int type)
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
	if (type == Type_all)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = momentsTag[0];
		key_value["cat"] = "png";
		key_value["psid"] = crossapp_format_string("%d", m_currentAllNum);
		key_value["cnt"] = crossapp_format_string("%d", 5);
		//key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MomentViewController::onRequestAllFinished));
		m_currentAllNum += 5;
	}
	else
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = momentsTag[1];
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MomentViewController::onRequestMyFinished));
	}

    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        //p_pLoading->setTargetOnCancel(this, callfunc_selector(MomentViewController::initMsgTableView));
    }
}

void MomentViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
		requestMsg(m_segType);
    }
    else if (btn->getTag() == 200)
    {
		m_segType = Type_all;
		if (m_allMsg.empty())
		{
			requestMsg(m_segType);
		}
		m_segView[0]->setVisible(true);
		m_segView[1]->setVisible(false);
    }
	else if (btn->getTag() == 201)
	{
		m_segType = Type_me;
		if (m_myMsg.empty())
		{
			requestMsg(m_segType);
		}
		m_segView[0]->setVisible(false);
		m_segView[1]->setVisible(true);
	}
}

void MomentViewController::onRequestAllFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
		const CSJson::Value& value = json["result"];
		int length = value["pl"].size();
		for (int i = 0; i < length; i++)
		{
			photoMsg temp;
			temp.picId = value["pl"][i]["PictureWallId"].asInt();
			temp.userId = value["pl"][i]["UserId"].asInt();
			temp.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["pl"][i]["Picture"].asCString());
			temp.caterory = value["pl"][i]["Category"].asString();
			temp.comment = value["pl"][i]["Comment"].asString();
			m_allMsg.push_back(temp);
		}
    }
    else
    {
        //showAlert();
    }
    
    {
		for (int i = 0; i < 5; i++)
		{
			photoMsg temp;
			temp.picId = 1;
			temp.userId = 2;
			temp.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
			temp.caterory = "cate";
			temp.comment = "comment";
			m_allMsg.push_back(temp);
		}
    }
    
	m_msgTableView->reloadData();

    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}

void MomentViewController::onRequestMyFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];
		int length = value["pl"].size();
		for (int i = 0; i < length; i++)
		{
			photoMsg temp;
			temp.picId = value["pl"][i]["PictureWallId"].asInt();
			temp.userId = value["pl"][i]["UserId"].asInt();
			temp.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["pl"][i]["Picture"].asCString());
			temp.caterory = value["pl"][i]["Category"].asString();
			temp.comment = value["pl"][i]["Comment"].asString();
			m_myMsg.push_back(temp);
		}
	}
	else
	{
		//showAlert();
	}

	{
		for (int i = 0; i < 5; i++)
		{
			photoMsg temp;
			temp.picId = 1;
			temp.userId = 2;
			temp.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
			temp.caterory = "cate";
			temp.comment = "comment";
			m_myMsg.push_back(temp);
		}
	}

	m_myCollectionView->reloadData();

	if (p_pLoading)
	{
		p_pLoading->stopAnimating();
	}
}

void MomentViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(DRect(_px(0), _px(240), m_winSize.width, m_winSize.height - _px(240)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(240)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(240)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}


CATableViewCell* MomentViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    
	std::string picId = crossapp_format_string("%d", m_allMsg[row].picId);
	CATableViewCell* cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier(picId.c_str()));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
		
		CommonUrlImageView* temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png")); 
		temImage->setFrame(DRect(0, 0, _size.width, _size.height));
		//CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
		temImage->setImage(CAImage::create("common/bg.png"));
		temImage->setUrl(m_allMsg[row].m_imageUrl);
		temImage->setTouchEnabled(false);
		cell->addSubview(temImage);

		CAView* view = CAView::createWithFrame(DRect(0, _size.height - _px(120), _size.width, _px(120)));
		view->setColor(ccc4(0, 0, 0, 128));
		cell->addSubview(view);

		temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
		temImage->setFrame(DRect(0, 0, _size.width, _size.height));
		//CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
		temImage->setImage(CAImage::create("common/bg.png"));
		temImage->setUrl(m_allMsg[row].m_imageUrl);
		temImage->setTouchEnabled(false);
		cell->addSubview(temImage);
    }
    
    return cell;
    
}

void MomentViewController::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{

}

void MomentViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
	CATableView* tableView = (CATableView*)view;
	CAView* headView = tableView->getFooterRefreshView();
	if (headView->getTag() == 0)
	{
		requestMsg(Type_all);
	}
	else
	{
		requestMsg(Type_me);
	}
}

unsigned int MomentViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int MomentViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return (int)m_allMsg.size();
}

unsigned int MomentViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return m_winSize.width;
}

void MomentViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void MomentViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}


void MomentViewController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

void MomentViewController::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

CACollectionViewCell* MomentViewController::collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
	DSize _size = cellSize;
	std::string picId = crossapp_format_string("%d", m_myMsg[row + item].picId);
	CACollectionViewCell* p_Cell = collectionView->dequeueReusableCellWithIdentifier(picId.c_str());
	if (p_Cell == NULL)
	{
		p_Cell = CACollectionViewCell::create("CrossApp");

		//CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		CommonUrlImageView* temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
		temImage->setFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
		temImage->setImage(CAImage::create("common/bg.png"));
		temImage->setUrl(m_myMsg[row + item].m_imageUrl);
		temImage->setTouchEnabled(false);
		p_Cell->addSubview(temImage);
	}

	return p_Cell;
}

unsigned int MomentViewController::numberOfSections(CACollectionView *collectionView)
{
	return 1;
}

unsigned int MomentViewController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
	return (m_myMsg.size() + 1) / 2;
}

unsigned int MomentViewController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
	return 2;
}

unsigned int MomentViewController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
	return m_winSize.width / 2;
}