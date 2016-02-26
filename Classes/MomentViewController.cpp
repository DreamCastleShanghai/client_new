
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
, m_currentCategory(filterMoments[0])
, m_canDelete(true)
//, m_browView(NULL)
, m_msgTableView(NULL)
, m_myCollectionView(NULL)
, m_filterView(NULL)
, m_leftBtn(NULL)
, m_rightBtn(NULL)
, m_tagBtn(NULL)
, m_deleteId(-1)
{
    m_allMsg.clear();
    m_allFilterMsg.clear();
    m_myMsg.clear();
    m_likeNumLabelVec.clear();
    m_likeBtnBG.clear();
    m_segView[0] = NULL;
    m_segView[1] = NULL;
}

MomentViewController::~MomentViewController()
{

}

void MomentViewController::viewDidAppear()
{
    /*
    if (FDataManager::getInstance()->isUserDirty()) {
        FDataManager::getInstance()->setUserDirty(false);
    }
    if (FDataManager::getInstance()->isRankDirty()) {
        FDataManager::getInstance()->setRankDirty(false);
    }
     */
    //requestMsg(Type_me);
    CCLog("appear ------------------------------------- ");
    CCLog("segtype : %d", m_segType);
    CCLog("m_currentCategory : %s", m_currentCategory.c_str());
//    if (FDataManager::getInstance()->isPhotoDirty()) {
//        FDataManager::getInstance()->setPhotoDirty(false);
//        requestMsg(Type_all);
//    }
    
    requestMsg(m_segType);
}

void MomentViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAView* sView = CAView::createWithFrame(DRect((0), (0), m_winSize.width, (240)));
    if (sView) {
        sView->setColor(SAP_DEFAULT_COLOR);
        this->getView()->addSubview(sView);
    }
    
    CAButton* bkBtn = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    if (bkBtn) {
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
        if (imageView) {
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            imageView->setFrame(DRect((20), (20), (80), (80)));
            bkBtn->setBackgroundViewForState(CAControlStateAll, imageView);
        }
        bkBtn->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
        bkBtn->setTag(20);
        this->getView()->addSubview(bkBtn);
    }

	m_tagBtn = CAButton::createWithFrame(DRect((m_winSize.width - (200)) / 2, (30), (250), (100)), CAButtonTypeCustom);
    if (m_tagBtn) {
        m_tagBtn->setTitleForState(CAControlStateAll, "Moments");
        m_tagBtn->setTitleFontName(SAP_FONT_ARIAL);
        m_tagBtn->setTitleFontSize(SAP_TITLE_FONT_SIZE);
        m_tagBtn->setTitleColorForState(CAControlStateAll, CAColor_white);
        
        m_tagBtn->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_tagBtn->setTag(30);
        this->getView()->addSubview(m_tagBtn);
        
        // down arrow
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("session/down.png"));
        if (imageView) {
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
            imageView->setFrame(DRect((190), (25), (55), (55)));
            m_tagBtn->addSubview(imageView);
        }
    }


	CAButton* uploadbutton = CAButton::createWithFrame(DRect(m_winSize.width - (100), (35), (70), (70)), CAButtonTypeCustom);
    if (uploadbutton) {
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("moments/upload_icon_white.png"));
        if (imageView) {
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
            imageView->setFrame(DRect((20), (20), (80), (80)));
            uploadbutton->setBackgroundViewForState(CAControlStateAll, imageView);
        }
        uploadbutton->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
        uploadbutton->setTag(40);
        this->getView()->addSubview(uploadbutton);
    }
    
    //CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (40)));
    //label->setTextAlignment(CATextAlignmentCenter);
    //label->setColor(CAColor_white);
    //label->setFontSize((40));
    //label->setText("Moments");
    //label->setFontName("fonts/arial.ttf");
    //sView->addSubview(label);

    /*
	CALabel* label = CALabel::createWithFrame(DRect(0, (130), m_winSize.width / 2, (40)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize((30));
	label->setText("Photos");
	label->setFontName("fonts/arial.ttf");
	sView->addSubview(label);

	label = CALabel::createWithFrame(DRect(m_winSize.width / 2, (130), m_winSize.width / 2, (40)));
	label->setTextAlignment(CATextAlignmentCenter);
	label->setColor(CAColor_white);
	label->setFontSize((30));
	label->setText("My Posts");
	label->setFontName("fonts/arial.ttf");
	sView->addSubview(label);
*/
    if (m_leftBtn == NULL) {
        m_leftBtn = CAButton::createWithFrame(DRect((0), (120), m_winSize.width / 2, (60)), CAButtonTypeCustom);
    }
    if (m_leftBtn) {
        m_leftBtn->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_leftBtn->setTag(200);
        m_leftBtn->setTitleFontSize(30);
        m_leftBtn->setTitleColorForState(CAControlStateAll, SAP_WHITE_LOW);
        m_leftBtn->setTitleColorForState(CAControlStateSelected, CAColor_white);
        m_leftBtn->setTitleFontName(SAP_FONT_ARIAL);
        m_leftBtn->setTitleForState(CAControlStateAll, "Photos");
        this->getView()->addSubview(m_leftBtn);
        m_leftBtn->setControlState(CAControlStateSelected);
    }
    
    if (m_rightBtn == NULL) {
        m_rightBtn = CAButton::createWithFrame(DRect(m_winSize.width / 2, (120), m_winSize.width / 2, (60)), CAButtonTypeCustom);
    }
    if (m_rightBtn) {
        m_rightBtn->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
        m_rightBtn->setTag(201);
        m_rightBtn->setTitleFontSize(30);
        m_rightBtn->setTitleColorForState(CAControlStateAll, SAP_WHITE_LOW);
        m_rightBtn->setTitleColorForState(CAControlStateSelected, CAColor_white);
        m_rightBtn->setTitleFontName(SAP_FONT_ARIAL);
        m_rightBtn->setTitleForState(CAControlStateAll, "My Posts");
        this->getView()->addSubview(m_rightBtn);
    }
    
    /*
	button = CAButton::createWithFrame(DRect(m_winSize.width / 2, (120), m_winSize.width / 2, (60)), CAButtonTypeCustom);
	button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
	button->setTag(201);
	this->getView()->addSubview(button);
*/
    /*
	m_browView = CAScale9ImageView::createWithFrame(DRect(0, (170), m_winSize.width / 2, (10)));
	m_browView->setImage(CAImage::create("common/gray_bg.png"));
	this->getView()->addSubview(m_browView);
     */

	for (int i = 0; i < 2; i++)
	{
		m_segView[i] = CAView::createWithFrame(DRect(0, (180), m_winSize.width, m_winSize.height - (180)));
		this->getView()->addSubview(m_segView[i]);

	}
	m_segView[1]->setVisible(false);

	

	m_msgTableView = CATableView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (180)));
	m_msgTableView->setTableViewDataSource(this);
	m_msgTableView->setTableViewDelegate(this);
	m_msgTableView->setAllowsSelection(true);
	m_msgTableView->setScrollViewDelegate(this);
	m_msgTableView->setSeparatorColor(CAColor_clear);
	m_segView[0]->addSubview(m_msgTableView);

	CAPullToRefreshView* headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
	headerRefreshView->setTag(0);
	m_msgTableView->setHeaderRefreshView(headerRefreshView);

	m_myCollectionView = CACollectionView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - (180)));
	m_myCollectionView->setAllowsSelection(true);
	//p_Conllection->setAllowsMultipleSelection(true);
	m_myCollectionView->setCollectionViewDelegate(this);
	m_myCollectionView->setCollectionViewDataSource(this);
	m_myCollectionView->setScrollViewDelegate(this);
	//m_myCollectionView->setHoriInterval((0));
	//m_myCollectionView->setVertInterval((0));
	m_segView[1]->addSubview(m_myCollectionView);

	headerRefreshView = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
	headerRefreshView->setTag(1);
	m_myCollectionView->setHeaderRefreshView(headerRefreshView);

	m_filterView = CAView::createWithFrame(DRect((m_winSize.width - (200)) / 2, (100), (240), (80) * MOMENTSFILTERNUM));
	m_filterView->setColor(ccc4(0, 0, 0, 128));
	this->getView()->addSubview(m_filterView);
	m_filterView->setVisible(false);

	for (int i = 0; i < MOMENTSFILTERNUM; i++)// filterMoments
	{
		CAButton* button = CAButton::createWithFrame(DRect((0), (80) * i, (240), (80)), CAButtonTypeCustom);
        if (button) {
            button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
            button->setTextTag(filterMoments[i]);
            button->setTitleForState(CAControlStateAll, crossapp_format_string("#%s", filterMoments[i]));
            button->setTitleFontSize((27));
            button->setTitleColorForState(CAControlStateAll, CAColor_white);//CAColor_gray);
            m_filterView->addSubview(button);
        }
	}

    requestMsg(Type_all);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void MomentViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}
void MomentViewController::requestMsg()
{
    CCLog("segtype : %d", m_segType);
    if  (m_segType == Type_all)
        requestMsg(m_segType);
}

void MomentViewController::requestMsg(int type)
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
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
        
//        const CSJson::Value& value = json["result"];
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
        m_msgTableView->reloadData();
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
        vc->autorelease();
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
        m_currentAllNum = 0;
        m_allMsg.clear();
		if (m_allMsg.empty())
		{
			requestMsg(m_segType);
		}
//		m_browView->setFrame(DRect(0, (170), m_winSize.width / 2, (10)));
		m_segView[0]->setVisible(true);
		m_segView[1]->setVisible(false);
        m_leftBtn->setControlState(CAControlStateSelected);
        m_rightBtn->setControlState(CAControlStateNormal);
    }
	else if (btn->getTag() == 201)
	{
		m_segType = Type_me;
		if (m_myMsg.empty())
		{
			requestMsg(m_segType);
		}
//		m_browView->setFrame(DRect(m_winSize.width / 2, (170), m_winSize.width / 2, (10)));
		m_segView[0]->setVisible(false);
		m_segView[1]->setVisible(true);
        m_leftBtn->setControlState(CAControlStateNormal);
        m_rightBtn->setControlState(CAControlStateSelected);
	}
	else if (btn->getTag() >= 300 && btn->getTag() < 400)
	{
//		if (!m_canLike) return;
		requestLike(btn->getTag() - 300);
	}
	else if (btn->getTag() >= 400 && btn->getTag() < 500)
	{
		if (!m_canDelete) return;
        //CAAlertView *alertView = CAAlertView::createWithText("", "Delete this photo?", "Yes", "No");
        //alertView->setTarget(this, CAAlertView_selector(MomentViewController::onDeleteAlert));
        //alertView->show();
        //m_deleteId = btn->getTag() - 400;
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
        if (m_tagBtn) {
            if (strcmp(filterMoments[0], m_currentCategory.c_str()))
                m_tagBtn->setTitleForState(CAControlStateAll, m_currentCategory);
            else
                m_tagBtn->setTitleForState(CAControlStateAll, "Moments");
        }
		m_filterView->setVisible(false);
	}
}

void MomentViewController::onDeleteAlert(int bid)
{
    switch (bid) {
        case 0:
            requestDelete(m_deleteId);
            //requestDelete(btn->getTag() - 400);
            break;
        default:
            break;
    }
    m_deleteId = -1;
}

void MomentViewController::refreshAllFilterMsg(const char* category)
{
    CCLog("refresh all filter msg ------------------------------------- %d", m_allMsg.size());
	m_allFilterMsg.clear();
	for (int i = 0; i < m_allMsg.size(); i++)
	{
        CCLog("cat : %s", m_allMsg[i].caterory.c_str());
		if (!(strcmp(m_allMsg[i].caterory.c_str(), category)) || !(strcmp(filterMoments[0], category)))
		{
			m_allFilterMsg.push_back(&(m_allMsg[i]));
		}
	}
}

void MomentViewController::onRequestAllFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    CCLog("onRequestAllFinished ------------------------------------- ");
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
    //refreshAllFilterMsg(m_currentCategory.c_str());
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
    
    p_alertView = CAView::createWithFrame(DRect((0), (240), m_winSize.width, m_winSize.height - (240)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect((0), (0), m_winSize.width, m_winSize.height - (240)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect((0), (0), m_winSize.width, m_winSize.height - (240)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - (400), m_winSize.width, (30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize((24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}


CATableViewCell* MomentViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    CCLog("row : %d", row);
    CCLog("m_allFilterMsg : %d", m_allFilterMsg.size());
	std::string picId = crossapp_format_string("%d", m_allFilterMsg.at(row)->picId);
    CCLog("picId : %s", picId.c_str());
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

		CAView* view = CAView::createWithFrame(DRect(0, _size.height - (120), _size.width, (120)));
		view->setColor(ccc4(0, 0, 0, 128));
		cell->addSubview(view);

		temImage = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
		temImage->setFrame(DRect((40), (20), (80), (80)));
		//CommonUrlImageView::createWithFrame(DRect(0, 0, _size.width, _size.height));
		temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		temImage->setImage(CAImage::create("common/head_bg.png"));
		temImage->setUrl(m_allFilterMsg.at(row)->m_iconUrl);
		temImage->setTouchEnabled(false);
		view->addSubview(temImage);

		CALabel* label = CALabel::createWithFrame(DRect((140), (20), m_winSize.width - (140), (35)));
		label->setTextAlignment(CATextAlignmentLeft);
        label->setColor(ccc4(0x0f, 0xaa, 0xff, 0xff));//CAColor_blue);
		label->setFontSize((30));
		label->setText(m_allFilterMsg.at(row)->name);
		label->setFontName(SAP_FONT_ARIAL);
		view->addSubview(label);

		label = CALabel::createWithFrame(DRect((140), (60), m_winSize.width - (140), (30)));
		label->setTextAlignment(CATextAlignmentLeft);
		label->setColor(CAColor_white);
		label->setFontSize((25));
		label->setText(crossapp_format_string("#%s", m_allFilterMsg.at(row)->caterory.c_str()));
		label->setFontName(SAP_FONT_ARIAL);
		view->addSubview(label);

		CAButton* button = CAButton::createWithFrame(DRect(m_winSize.width - (160), (40), (50), (50)), CAButtonTypeCustom);
		CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
		imageView->setFrame(DRect((20), (20), (80), (80)));
        if (m_allFilterMsg.at(row)->liked)
            imageView->setImage(CAImage::create("common/btn_like_pre.png"));
		button->setBackgroundViewForState(CAControlStateAll, imageView);
		button->addTarget(this, CAControl_selector(MomentViewController::buttonCallBack), CAControlEventTouchUpInSide);
		button->setTag(300 + row);
		view->addSubview(button);
        m_likeBtnBG.push_back(imageView);

		label = CALabel::createWithFrame(DRect(m_winSize.width - (100), (50), (200), (30)));
		label->setTextAlignment(CATextAlignmentLeft);
		label->setColor(CAColor_white);
		label->setFontSize((25));
		label->setText(crossapp_format_string("%d", m_allFilterMsg.at(row)->likeNum));
		label->setFontName(SAP_FONT_ARIAL);
		view->addSubview(label);
		m_likeNumLabelVec.push_back(label);

    }
    
    return cell;
    
}

void MomentViewController::scrollViewHeaderBeginRefreshing(CAScrollView* view)
{
	CATableView* tableView = (CATableView*)view;
	CAView* headView = tableView->getHeaderRefreshView();
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
    vc->setParent(this);
    vc->autorelease();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MomentViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}


void MomentViewController::collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item)
{
    MomentsDetailViewController* vc = new MomentsDetailViewController(m_myMsg[row * 2 + item], 1);
    vc->init();
    vc->setParent(this);
    vc->autorelease();
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

		CAButton* button = CAButton::createWithFrame(DRect(_size.width - (60), 0, (60), (60)), CAButtonTypeCustom);
		CAImageView* imageView = CAImageView::createWithImage(CAImage::create("moments/delete.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
		imageView->setFrame(DRect((0), (0), (60), (60)));
		button->setBackgroundViewForState(CAControlStateAll, imageView);
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