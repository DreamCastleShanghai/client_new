
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"
#include "FServerTime.h"
#include "ConstData/ConstRect.h"
#include "MainViewController.h"
#include "MyStatusViewController.h"
#include "FNoticeManager.h"

MainViewTableCell::MainViewTableCell()
: m_titleLabel(NULL)
, m_lectureLabel(NULL)
, m_locationLabel(NULL)
, m_timeLabel(NULL)
, m_likeNumLabel(NULL)
, m_storeBtnImage(NULL)
, m_likeBtn(NULL)
, m_likeBtnImage(NULL)
, m_urlImageView(NULL)
, m_rightArrow(NULL)
, m_msg(NULL)
//, m_isStore(true)
//, m_canStore(true)
//, m_isLike(true)
{
	this->setAllowsSelected(false);
}

MainViewTableCell::~MainViewTableCell()
{

}

MainViewTableCell* MainViewTableCell::create(const std::string& identifier, const DRect& _rect)
{
	MainViewTableCell* tableViewCell = new MainViewTableCell();
	if (tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
	{
		tableViewCell->setFrame(_rect);
		tableViewCell->autorelease();
		return tableViewCell;
	}
	CC_SAFE_DELETE(tableViewCell);
	return NULL;
}

void MainViewTableCell::highlightedTableViewCell()
{
	this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void MainViewTableCell::selectedTableViewCell()
{
	this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void MainViewTableCell::initWithCell(sessionMsg &msg)
{
	DSize _size = this->getFrame().size;
    DRect tempRect;
    
	m_msg = &msg;
    
    if (m_msg == NULL) {
        return;
    }

    // session logo
    tempRect = ConstRect::getSessionCellLogoRect();
    m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/head_bg.png"));
    if (m_urlImageView) {
        m_urlImageView->setFrame(tempRect);
        m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        //m_urlImageView->setImage(CAImage::create("common/head_bg.png"));
        m_urlImageView->setUrl(m_msg->m_imageUrl);
        //CCLog("logo : %s", m_msg->m_imageUrl.c_str());
        //m_urlImageView->setImage(CAImage::create("common/head_bg.png"));
       this->getContentView()->addSubview(m_urlImageView);
    }

    // session title
    tempRect = ConstRect::getSessionCellTitleRect();
    tempRect.size.width = _size.width - tempRect.origin.x * 2;
	m_titleLabel = CALabel::createWithFrame(tempRect);
    if (m_titleLabel) {
        m_titleLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
        m_titleLabel->setTextAlignment(CATextAlignmentLeft);
        m_titleLabel->setFontSize((27));
        m_titleLabel->setText(m_msg->m_title);
        this->getContentView()->addSubview(m_titleLabel);
    }

    // session time
    tempRect = ConstRect::getSessionCellTimeRect();
    tempRect.size.width = _size.width / 2;
    m_timeLabel = CALabel::createWithFrame(tempRect);
    if (m_timeLabel) {
        m_timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
        m_timeLabel->setTextAlignment(CATextAlignmentLeft);
        //m_timeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
        m_timeLabel->setFontSize((25));
        m_timeLabel->setText(crossapp_format_string("%s - %s", timeToString(m_msg->m_startTime).c_str(), timeToString(m_msg->m_endTime).c_str()));//%lld
        this->getContentView()->addSubview(m_timeLabel);
    }
    
    // session location
    tempRect = ConstRect::getSessionCellLocationRect();
    tempRect.size.width = _size.width - (210);
	m_locationLabel = CALabel::createWithFrame(tempRect);
    if (m_locationLabel) {
        m_locationLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
        m_locationLabel->setTextAlignment(CATextAlignmentLeft);
        m_locationLabel->setFontSize((25));
        m_locationLabel->setText(crossapp_format_string("Location: %s", m_msg->m_location.c_str()));
        this->getContentView()->addSubview(m_locationLabel);
    }

    //CAView* view = CAView::createWithFrame(DRect(_size.width - (140), (40), (100), (50)));
	//this->getContentView()->addSubview(view);

    // session like button
    tempRect = ConstRect::getSessionCellLikeRect();
    tempRect.origin.x = _size.width - (140);
	m_likeBtn = CAButton::createWithFrame(tempRect, CAButtonTypeRoundedRect);
    if (m_likeBtn) {
        m_likeBtn->setAllowsSelected(true);
        m_likeBtn->setTag(300);
        m_likeBtn->addTarget(this, CAControl_selector(MainViewTableCell::buttonCallback), CAControlEventTouchUpInSide);
        this->addSubview(m_likeBtn);
        m_likeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
        if (m_likeBtnImage) {
            m_likeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
            m_likeBtn->setBackgroundViewForState(CAControlStateAll, m_likeBtnImage);
        }
    }
    //view->addSubview(button);
    
    // session like number label
    tempRect = ConstRect::getSessionCellLikeLableRect();
    tempRect.origin.x = _size.width - (80);
    m_likeNumLabel = CALabel::createWithFrame(tempRect);
    if (m_likeNumLabel) {
        m_likeNumLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
        m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
        m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        m_likeNumLabel->setFontSize((28));
        m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
        this->addSubview(m_likeNumLabel);
    }
    //view->addSubview(m_likeNumLabel);
    
    // right arrow button
    tempRect = ConstRect::getSessionCellDetailArrowRect();
    tempRect.origin.x = _size.width - (60);
    m_rightArrow = CAImageView::createWithImage(CAImage::create("common/btn_rightarrow.png"));
    if (m_rightArrow) {
        m_rightArrow->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
        m_rightArrow->setFrame(tempRect);
        this->getContentView()->addSubview(m_rightArrow);
    }
    
    // session store button
    tempRect = ConstRect::getSessionCellCollectionRect();
    tempRect.origin.x = _size.width - (200);
	m_storeBtn = CAButton::createWithFrame(tempRect, CAButtonTypeRoundedRect);
    if (m_storeBtn) {
        m_storeBtn->setAllowsSelected(true);
        m_storeBtn->setTag(200);
        m_storeBtn->addTarget(this, CAControl_selector(MainViewTableCell::buttonCallback), CAControlEventTouchUpInSide);
        this->getContentView()->addSubview(m_storeBtn);
        m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
        if (m_storeBtnImage) {
            m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
            m_storeBtn->setBackgroundViewForState(CAControlStateAll, m_storeBtnImage);
        }
    }


//	m_isStore = m_msg->m_stored;
//	m_canStore = true;
	if (m_msg->m_stored)
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
	}
	else
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
	}

//	m_canLike = !(m_msg->m_liked);
//    m_isLike = m_msg->m_liked;
	if (m_msg->m_liked)
	{
        m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}
	else
	{
        m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
	}

}

void MainViewTableCell::requstStore()
{
//	if (m_canStore)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[1];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", m_msg->m_stored ? 0 : 1);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onStoreRequestFinished));

//		m_canStore = false;
	}
}

void MainViewTableCell::requestLike()
{
//	if (m_canLike)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[2];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        key_value["v"] = crossapp_format_string("%d", m_msg->m_liked ? 0 : 1);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onLikeRequestFinished));
	}
}

void MainViewTableCell::buttonCallback(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 200)
	{
		requstStore();
	}
	else if (btn->getTag() == 300)
	{
		requestLike();
	}
    
}

void MainViewTableCell::onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    
    if (status == HttpResponseSucceed)
    {
//        CSJson::FastWriter writer;
//        string tempjson = writer.write(json);
//        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if (value["r"].asBool() == true)
        {
            FNoticeManager::sharedFNoticeManager()->addNotice(m_msg->m_sessionId, notice_session, m_msg->m_title, m_msg->m_startTime, m_msg->m_endTime);
            m_msg->m_stored = true;
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
        }
        else
        {
            FNoticeManager::sharedFNoticeManager()->deleteNotice(m_msg->m_sessionId);
            m_msg->m_stored = false;
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
        }
        updateTable();
    }
//    m_canStore = true;
}

void MainViewTableCell::onLikeRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
        /*
		m_msg->m_likeNum += 1;
		m_msg->m_liked = true;
//		m_canLike = false;
		m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
        */
//        CSJson::FastWriter writer;
//        string tempjson = writer.write(json);
//        CCLog("receive json == %s",tempjson.c_str());
        
        const CSJson::Value& value = json["result"];
        if (value["r"].asBool() == true)
        {
            m_msg->m_likeNum += 1;
            m_msg->m_liked = true;
            m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
        }
        else
        {
            m_msg->m_likeNum -= 1;
            m_msg->m_liked = false;
            m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
        }
        m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
        updateTable();
	}
    /*
	else
	{
		m_canLike = !(m_msg->m_liked);
	}
     */
}

void MainViewTableCell::updateTable()
{
    MainViewController* mv = (MainViewController*)(RootWindow::getInstance()->getHomeView());
    if (mv)
        mv->refreshTable();

    MyStatusViewController* sv = (MyStatusViewController*)(RootWindow::getInstance()->getMyView());
    if (sv) {
        sv->refreshTable();
    }
}