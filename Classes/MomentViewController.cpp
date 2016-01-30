
#include "MomentViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"
#include "FSegmentView.h"
#include "PhotoViewController.h"
#include "MomentsDetailViewController.h"

MomentViewController::MomentViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_segType(0)
, m_currentAllNum(0)
, m_currentMyNum(0)
, m_currentCategory("all")
, m_canDelete(true)
//, m_canLike(true)
{
    m_likeNumLabelVec.clear();
    m_likeBtnBG.clear();
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

	button = CAButton::createWithFrame(DRect((m_winSize.width - _px(200)) / 2, _px(30), _px(200), _px(100)), CAButtonTypeCustom);
	button->setTitleForState(CAControlStateAll, "Moments");
	button->setTitleFontSize(_px(40));
	button->setTitleColorForState(CAControlStateAll, CAColor_white);

	button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(30);
	this->getView()->addSubview(button);

    // down arrow
	imageView = CAImageView::createWithImage(CAImage::create("session/down.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	imageView->setFrame(DRect(_px(180), _px(0), _px(80), _px(80)));
	button->addSubview(imageView);

	button = CAButton::createWithFrame(DRect(m_winSize.width - _px(100), _px(30), _px(70), _px(70)), CAButtonTypeCustom);
	imageView = CAImageView::createWithImage(CAImage::create("moments/upload_icon.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
	imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(40);
	this->getView()->addSubview(button);
    
    //CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    //label->setTextAlignment(CATextAlignmentCenter);
    //label->setColor(CAColor_white);
    //label->setFontSize(_px(40));
    //label->setText("Moments");
    //label->setFontName("fonts/arial.ttf");
    //sView->addSubview(label);

	CALabel* label = CALabel::createWithFrame(DRect(0, _px(160), m_winSize.width / 2, _px(40)));
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
	m_browView->setImage(CAImage::create("common/gray_bg.png"));
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

	footerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
	footerRefreshView->setTag(1);
	m_myCollectionView->setFooterRefreshView(footerRefreshView);

	m_filterView = CAView::createWithFrame(DRect((m_winSize.width - _px(200)) / 2, _px(100), _px(240), _px(80) * MOMENTSFILTERNUM));
	m_filterView->setColor(ccc4(0, 0, 0, 128));
	this->getView()->addSubview(m_filterView);
	m_filterView->setVisible(false);

	for (int i = 0; i < MOMENTSFILTERNUM; i++)// filterMoments
	{
		button = CAButton::createWithFrame(DRect(_px(0), _px(80) * i, _px(240), _px(80)), CAButtonTypeCustom);
		button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTextTag(filterMoments[i]);
		button->setTitleForState(CAControlStateAll, crossapp_format_string("#%s", filterMoments[i]));
		button->setTitleFontSize(_px(27));
		button->setTitleColorForState(CAControlStateAll, CAColor_gray);
		m_filterView->addSubview(button);
	}

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
		key_value["cat"] = m_currentCategory;
		key_value["psid"] = crossapp_format_string("%d", m_currentAllNum);
		key_value["cnt"] = crossapp_format_string("%d", 5);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MomentViewController::onRequestAllFinished));
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


void MomentViewController::requestLike(int index)
{
	m_currentLike = index;
//	m_canLike = false;
	std::map<std::string, std::string> key_value;
	key_value["tag"] = momentsTag[2];
	key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
	key_value["pwid"] = crossapp_format_string("%d", m_allMsg[index].picId);
    key_value["v"] = crossapp_format_string("%d", m_allMsg[index].liked ? 0 : 1 );
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MomentViewController::onRequestLikeFinished));
}

void MomentViewController::onRequestLikeFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        bool isLike = json["result"]["r"].asBool();
        if (isLike)
        {
			m_allMsg[m_currentLike].liked = true;
			m_allMsg[m_currentLike].likeNum += 1;
//			m_likeNumLabelVec[m_currentLike]->setText(crossapp_format_string("%d", m_allMsg[m_currentLike].likeNum));
//            m_likeBtnBG[m_currentLike]->setImage(CAImage::create("common/btn_like_pre.png"));
        }
        else
        {
            m_allMsg[m_currentLike].liked = false;
            m_allMsg[m_currentLike].likeNum -= 1;
//            m_likeNumLabelVec[m_currentLike]->setText(crossapp_format_string("%d", m_allMsg[m_currentLike].likeNum));
//            m_likeBtnBG[m_currentLike]->setImage(CAImage::create("common/btn_like.png"));
        }
        m_msgTableView->reloadData();
        
    }
//	m_canLike = true;
}

void MomentViewController::requestDelete(int index)
{
	m_currentDelete = index;
	m_canDelete = false;
	std::map<std::string, std::string> key_value;
	key_value["tag"] = momentsTag[3];
	key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
	key_value["filepath"] = m_myMsg[index].m_imageUrl;
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MomentViewController::onRequestDeleteFinished));
}

void MomentViewController::onRequestDeleteFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		CSJson::FastWriter writer;
		string tempjson = writer.write(json);
		CCLog("receive json == %s", tempjson.c_str());

		const CSJson::Value& value = json["result"];
		if (value["r"] == "1")
		{
			int index = 0;
			std::vector<photoMsg>::iterator it = m_myMsg.begin();
			for (it = m_myMsg.begin(); it != m_myMsg.end(); it++)
			{
				if (m_currentDelete == index)
				{
					it = m_myMsg.erase(it);
					break;
				}
				index++;
			}
		}
		m_myCollectionView->reloadData();
	}

	m_canDelete = true;
}

void MomentViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
	else if (btn->getTag() == 30)
	{
		m_filterView->setVisible(!(m_filterView->isVisible()));
	}
    else if (btn->getTag() == 40)
    {
        PhotoViewController* vc = new PhotoViewController(1);
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
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
		m_browView->setFrame(DRect(0, _px(230), m_winSize.width / 2, _px(10)));
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
		m_browView->setFrame(DRect(m_winSize.width / 2, _px(230), m_winSize.width / 2, _px(10)));
		m_segView[0]->setVisible(false);
		m_segView[1]->setVisible(true);
	}
	else if (btn->getTag() >= 300 && btn->getTag() < 400)
	{
//		if (!m_canLike) return;
		requestLike(btn->getTag() - 300);
	}
	else if (btn->getTag() >= 400 && btn->getTag() < 500)
	{
		if (!m_canDelete) return;
		requestDelete(btn->getTag() - 400);
	}
	else // filter button
	{
		for (int i = 0; i < MOMENTSFILTERNUM; i++)
		{
			if (!strcmp(filterMoments[i], btn->getTextTag().c_str()))
			{
				m_currentCategory = filterMoments[i];
				refreshAllFilterMsg(m_currentCategory.c_str());
				m_msgTableView->reloadData();
			}
		}
		m_filterView->setVisible(false);
	}
}

void MomentViewController::refreshAllFilterMsg(const char* category)
{
	m_allFilterMsg.clear();
	for (int i = 0; i < m_allMsg.size(); i++)
	{
		if (!(strcmp(m_allMsg[i].caterory.c_str(), category)) || !(strcmp("all", category)))
		{
			m_allFilterMsg.push_back(&(m_allMsg[i]));
		}
	}
}

void MomentViewController::onRequestAllFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
		const CSJson::Value& value = json["result"];
		int length = value["pl"].size();
		for (int i = 0; i < length; i++)
		{
			photoMsg temp;
			temp.picId = value["pl"][i]["PictureWallId"].asInt();
			temp.userId = value["pl"][i]["UserId"].asInt();
			temp.m_imageUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["pl"][i]["Picture"].asCString());
			temp.m_iconUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["pl"][i]["Icon"].asCString());
			temp.caterory = value["pl"][i]["Category"].asString();
			temp.comment = value["pl"][i]["Comment"].asString();
			temp.name = crossapp_format_string("%s %s", value["pl"][i]["LastName"].asCString(), value["pl"][i]["FirstName"].asCString());
            temp.liked = value["pl"][i]["IsLiked"].asBool();
            temp.likeNum = value["pl"][i]["LikeFlagCnt"].asInt();
			m_allMsg.push_back(temp);
		}
		refreshAllFilterMsg(m_currentCategory.c_str());
		m_currentAllNum += length;
    }
    else
    {
        //showAlert();
    }
#ifdef LOCALTEST
    {
		for (int i = 0; i < 5; i++)
		{
			photoMsg temp;
			temp.picId = 1;
			temp.userId = 2;
			temp.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
			temp.m_iconUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
			temp.liked = false;
			temp.likeNum = 100;
			temp.caterory = "cate";
			temp.comment = "comment";
			temp.name = "Alex Chen";
			m_allMsg.push_back(temp);
		}
		refreshAllFilterMsg(m_currentCategory.c_str());
		m_currentAllNum += 5;
    }
#endif
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
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
		const CSJson::Value& value = json["result"];
		int length = value["pl"].size();
        m_myMsg.clear();
		for (int i = 0; i < length; i++)
		{
			photoMsg temp;
			temp.picId = value["pl"][i]["PictureWallId"].asInt();
			temp.userId = value["pl"][i]["UserId"].asInt();
			temp.m_imageUrl = value["pl"][i]["Picture"].asString();
			//temp.m_iconUrl = crossapp_format_string("%s%s", imgPreUrl.c_str(), value["pl"][i]["Icon"].asCString());
			temp.caterory = value["pl"][i]["Category"].asString();
			temp.comment = value["pl"][i]["Comment"].asString();
			//temp.name = crossapp_format_string("%s %s", value["pl"][i]["LastName"].asCString(), value["pl"][i]["FirstName"].asCString());
			m_myMsg.push_back(temp);
		}
		m_currentMyNum += length;
	}
	else
	{
		//showAlert();
	}
#ifdef LOCALTEST
	{
		for (int i = 0; i < 5; i++)
		{
			photoMsg temp;
			temp.picId = 1;
			temp.userId = 2;
			temp.m_imageUrl = "http://img1.gtimg.com/14/1468/146894/14689486_980x1200_0.png";
			temp.m_iconUrl = "http://imgsrc.baidu.com/forum/pic/item/53834466d0160924a41f433bd50735fae6cd3452.jpg";
			temp.liked = false;
			temp.likeNum = 100;
			temp.caterory = "cate";
			temp.comment = "comment";
			temp.name = "Alex Chen";
			m_myMsg.push_back(temp);
		}
		m_currentMyNum += 5;
	}
#endif
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
    
	std::string picId = crossapp_format_string("%d", m_allFilterMsg.at(row)->picId);
    table->dequeueReusableCellWithIdentifier(picId.c_str());
    CATableViewCell* cell = NULL;//dynamic_cast<CATableViewCell*>();
    if (cell == NULL)
    {
        cell = MainViewTableCell::create(picId, DRect(0, 0, _size.width, _size.height));
		
		CommonUrlImageView* temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png")); 
		temImage->setFrame(DRect(0, 0, _size.width, _size.height));
		//CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
		temImage->setImage(CAImage::create("common/bg.png"));
		temImage->setUrl(m_allFilterMsg.at(row)->m_imageUrl);
		temImage->setTouchEnabled(false);
		cell->addSubview(temImage);

		CAView* view = CAView::createWithFrame(DRect(0, _size.height - _px(120), _size.width, _px(120)));
		view->setColor(ccc4(0, 0, 0, 128));
		cell->addSubview(view);

		temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
		temImage->setFrame(DRect(_px(40), _px(20), _px(80), _px(80)));
		//CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		temImage->setImage(CAImage::create("common/bg.png"));
		temImage->setUrl(m_allFilterMsg.at(row)->m_iconUrl);
		temImage->setTouchEnabled(false);
		view->addSubview(temImage);

		CALabel* label = CALabel::createWithFrame(DRect(_px(140), _px(20), m_winSize.width - _px(140), _px(35)));
		label->setTextAlignment(CATextAlignmentLeft);
		label->setColor(CAColor_blue);
		label->setFontSize(_px(30));
		label->setText(m_allFilterMsg.at(row)->name);
		label->setFontName("fonts/arial.ttf");
		view->addSubview(label);

		label = CALabel::createWithFrame(DRect(_px(140), _px(60), m_winSize.width - _px(140), _px(30)));
		label->setTextAlignment(CATextAlignmentLeft);
		label->setColor(CAColor_white);
		label->setFontSize(_px(25));
		label->setText(crossapp_format_string("#%s", m_allFilterMsg.at(row)->caterory.c_str()));
		label->setFontName("fonts/arial.ttf");
		view->addSubview(label);

		CAButton* button = CAButton::createWithFrame(DRect(m_winSize.width - _px(160), _px(40), _px(50), _px(50)), CAButtonTypeCustom);
		CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
		imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
        if (m_allFilterMsg.at(row)->liked)
            imageView->setImage(CAImage::create("common/btn_like_pre.png"));
		button->setBackGroundViewForState(CAControlStateAll, imageView);
		button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTag(300 + row);
		view->addSubview(button);
        m_likeBtnBG.push_back(imageView);

		label = CALabel::createWithFrame(DRect(m_winSize.width - _px(100), _px(50), _px(200), _px(30)));
		label->setTextAlignment(CATextAlignmentLeft);
		label->setColor(CAColor_white);
		label->setFontSize(_px(25));
		label->setText(crossapp_format_string("%d", m_allFilterMsg.at(row)->likeNum));
		label->setFontName("fonts/arial.ttf");
		view->addSubview(label);
		m_likeNumLabelVec.push_back(label);

    }
    
    return cell;
    
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
	return (int)m_allFilterMsg.size();
}

unsigned int MomentViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return m_winSize.width;
}

void MomentViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
    MomentsDetailViewController* vc = new MomentsDetailViewController(*(m_allFilterMsg.at(row)), 0);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MomentViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}


void MomentViewController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    MomentsDetailViewController* vc = new MomentsDetailViewController(m_myMsg[row + item], 1);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MomentViewController::collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{

}

CACollectionViewCell* MomentViewController::collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item)
{
	if (m_myMsg.size() < row * 2 + item + 1)
	{
		return NULL;
	}
	DSize _size = cellSize;
	std::string picId = crossapp_format_string("%d", m_myMsg[row + item].picId);
    collectionView->dequeueReusableCellWithIdentifier(picId.c_str());
	CACollectionViewCell* p_Cell = NULL;//
	if (p_Cell == NULL)
	{
		p_Cell = CACollectionViewCell::create(picId);

		//CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		CommonUrlImageView* temImage = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
		temImage->setFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
		temImage->setImage(CAImage::create("common/bg.png"));
		temImage->setUrl(crossapp_format_string("%s%s", imgPreUrl.c_str(), m_myMsg[row * 2 + item].m_imageUrl.c_str()));
		temImage->setTouchEnabled(false);
		p_Cell->addSubview(temImage);

		CAButton* button = CAButton::createWithFrame(DRect(_size.width - _px(80), 0, _px(80), _px(80)), CAButtonTypeCustom);
		CAImageView* imageView = CAImageView::createWithImage(CAImage::create("moments/delete.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		imageView->setFrame(DRect(_px(20), _px(20), _px(80), _px(80)));
		button->setBackGroundViewForState(CAControlStateAll, imageView);
		button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTag(400 + row * 2 + item);
		p_Cell->addSubview(button);
	}

	return p_Cell;
}

unsigned int MomentViewController::numberOfSections(CACollectionView *collectionView)
{
	return 1;
}

unsigned int MomentViewController::numberOfRowsInSection(CACollectionView *collectionView, unsigned int section)
{
	return ((int)m_myMsg.size() + 1) / 2;
}

unsigned int MomentViewController::numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row)
{
	return 2;
}

unsigned int MomentViewController::collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row)
{
	return m_winSize.width / 2;
}