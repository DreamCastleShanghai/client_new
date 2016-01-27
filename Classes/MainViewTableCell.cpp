
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"
#include "FServerTime.h"
#include "ConstData.h"

MainViewTableCell::MainViewTableCell()
: m_titleLabel(NULL),
  m_isStore(true),
  m_urlImageView(NULL)
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

    m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
    tempRect = ConstData::getSessionCellLogoRect();
    m_urlImageView->setFrame(tempRect);
    m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    m_urlImageView->setImage(CAImage::create("common/bg.png"));
    this->getContentView()->addSubview(m_urlImageView);

    tempRect = ConstData::getSessionCellTitleRect();
    tempRect.size.width = _size.width - tempRect.origin.x * 2;
	m_titleLabel = CALabel::createWithFrame(tempRect);
	m_titleLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	m_titleLabel->setTextAlignment(CATextAlignmentLeft);
	m_titleLabel->setFontSize(_px(27));
	this->getContentView()->addSubview(m_titleLabel);

    tempRect = ConstData::getSessionCellTimeRect();
    tempRect.size.width = _size.width / 2;
    m_timeLabel = CALabel::createWithFrame(tempRect);
    m_timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    m_timeLabel->setTextAlignment(CATextAlignmentLeft);
    //m_timeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_timeLabel->setFontSize(_px(25));
    this->getContentView()->addSubview(m_timeLabel);
    
    tempRect = ConstData::getSessionCellLocationRect();
    tempRect.size.width = _size.width - _px(210);
	m_locationLabel = CALabel::createWithFrame(tempRect);
	m_locationLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
	m_locationLabel->setTextAlignment(CATextAlignmentLeft);
	m_locationLabel->setFontSize(_px(25));
	this->getContentView()->addSubview(m_locationLabel);

    //CAView* view = CAView::createWithFrame(DRect(_size.width - _px(140), _px(40), _px(100), _px(50)));
	//this->getContentView()->addSubview(view);

    tempRect = ConstData::getSessionCellLikeRect();
    tempRect.origin.x = _size.width - _px(140);
	m_likeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
	m_likeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	CAButton* button = CAButton::createWithFrame(tempRect, CAButtonTypeRoundedRect);
	button->setAllowsSelected(true);
	button->setBackGroundViewForState(CAControlStateAll, m_likeBtnImage);
	button->setTag(300);
	button->addTarget(this, CAControl_selector(MainViewTableCell::buttonCallback), CAControlEventTouchUpInSide);
	this->addSubview(button);
    //view->addSubview(button);
    
    tempRect = ConstData::getSessionCellLikeLableRect();
    tempRect.origin.x = _size.width - _px(80);
    m_likeNumLabel = CALabel::createWithFrame(tempRect);
    m_likeNumLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
    m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
    m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_likeNumLabel->setFontSize(_px(28));
    this->addSubview(m_likeNumLabel);
    //view->addSubview(m_likeNumLabel);
    
    tempRect = ConstData::getSessionCellDetailArrowRect();
    tempRect.origin.x = _size.width - _px(60);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/btn_rightarrow.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    imageView->setFrame(tempRect);
    this->getContentView()->addSubview(imageView);
    
    tempRect = ConstData::getSessionCellCollectionRect();
    tempRect.origin.x = _size.width - _px(200);
    m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
    m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	button = CAButton::createWithFrame(tempRect, CAButtonTypeRoundedRect);
	button->setAllowsSelected(true);
	button->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
	button->setTag(200);
	button->addTarget(this, CAControl_selector(MainViewTableCell::buttonCallback), CAControlEventTouchUpInSide);
	this->getContentView()->addSubview(button);

	m_titleLabel->setText(m_msg->m_title);
	m_locationLabel->setText(crossapp_format_string("Location: %s", m_msg->m_location.c_str()));
	m_timeLabel->setText(crossapp_format_string("%s - %s", timeToString(m_msg->m_startTime).c_str(), timeToString(m_msg->m_endTime).c_str()));//%lld
	m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
	m_urlImageView->setUrl(m_msg->m_imageUrl);

	m_isStore = m_msg->m_stored;
	m_canStore = true;
	if (m_isStore)
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
	}
	else
	{
		m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
	}

	m_canLike = !(m_msg->m_liked);
	if (m_canLike)
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like.png"));
	}
	else
	{
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}

}

void MainViewTableCell::requstStore()
{
	if (m_canStore)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[1];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", m_isStore ? 1 : 0);
		//key_value["sign"] = getSign(key_value);
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onStoreRequestFinished));

		m_canStore = false;
	}
}

void MainViewTableCell::requestLike()
{
	if (m_canLike)
	{
		std::map<std::string, std::string> key_value;
		key_value["tag"] = sessionViewTag[2];
		key_value["sid"] = crossapp_format_string("%d", m_msg->m_sessionId);
		key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
		key_value["v"] = crossapp_format_string("%d", 1);
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
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        m_isStore = !m_isStore;
        m_msg->m_stored = m_isStore;
        if (m_isStore)
        {
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect_pre.png"));
        }
        else
        {
            m_storeBtnImage->setImage(CAImage::create("common/btn_collect.png"));
        }
    }
    m_canStore = true;
}

void MainViewTableCell::onLikeRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		m_msg->m_likeNum += 1;
		m_msg->m_liked = true;
		m_canLike = false;
		m_likeNumLabel->setText(crossapp_format_string("%d", m_msg->m_likeNum));
		m_likeBtnImage->setImage(CAImage::create("common/btn_like_pre.png"));
	}
	else
	{
		m_canLike = !(m_msg->m_liked);
	}
}