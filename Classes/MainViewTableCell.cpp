
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"
#include "FServerTime.h"

MainViewTableCell::MainViewTableCell()
: m_titleLabel(NULL),
m_lectureLabel(NULL),
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

void MainViewTableCell::initWithCell()
{
	DSize _size = this->getFrame().size;
    
    m_urlImageView = CommonUrlImageView::createWithImage(CAImage::create("common/bg.png"));
    //createWithFrame(DRect(_px(30), _px(40), _px(80), _px(80)));
    m_urlImageView->setFrame(DRect(_px(30), _px(40), _px(80), _px(80)));
    m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    m_urlImageView->setImage(CAImage::create("common/bg.png"));
    this->getContentView()->addSubview(m_urlImageView);

	m_titleLabel = CALabel::createWithFrame(DRect(_px(140), _px(40), _size.width / 2, _px(40)));
	m_titleLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
	m_titleLabel->setTextAlignment(CATextAlignmentLeft);
	m_titleLabel->setFontSize(_px(32));
	this->getContentView()->addSubview(m_titleLabel);

    m_timeLabel = CALabel::createWithFrame(DRect(_px(140), _px(90), _size.width / 2, _px(30)));
    m_timeLabel->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
    m_timeLabel->setTextAlignment(CATextAlignmentLeft);
    //m_timeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_timeLabel->setFontSize(_px(28));
    this->getContentView()->addSubview(m_timeLabel);
    
	m_lectureLabel = CALabel::createWithFrame(DRect(_px(140), _px(130), _size.width / 2, _px(25)));
	m_lectureLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
	m_lectureLabel->setTextAlignment(CATextAlignmentLeft);
	m_lectureLabel->setFontSize(_px(20));
	this->getContentView()->addSubview(m_lectureLabel);
    
    m_lectureDetailLabel = CALabel::createWithFrame(DRect(_px(140), _px(165), _size.width - _px(210), _px(25)));
    m_lectureDetailLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
    m_lectureDetailLabel->setTextAlignment(CATextAlignmentLeft);
    m_lectureDetailLabel->setFontSize(_px(20));
    this->getContentView()->addSubview(m_lectureDetailLabel);

    CAView* view = CAView::createWithFrame(DRect(_size.width - _px(140), _px(40), _px(100), _px(50)));
    
    CAImageView* likeImg = CAImageView::createWithImage(CAImage::create("common/btn_like.png"));
    likeImg->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    likeImg->setFrame(DRect(0, 0, _px(50), _px(50)));
    view->addSubview(likeImg);
    
    m_likeNumLabel = CALabel::createWithFrame(DRect(_px(50), _px(20), _px(50), _px(30)));
    m_likeNumLabel->setColor(ccc4(0xa1, 0xa1, 0xa1, 0xff));
    m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
    m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    m_likeNumLabel->setFontSize(_px(28));
    view->addSubview(m_likeNumLabel);
    this->getContentView()->addSubview(view);
    
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/btn_rightarrow.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    imageView->setFrame(DRect(_size.width - _px(60), _px(100), _px(50), _px(50)));
    this->getContentView()->addSubview(imageView);
    
    m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/btn_collect_pre.png"));
    m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    CAButton* storeBtn = CAButton::createWithFrame(DRect(_size.width - _px(200), _px(40), _px(50), _px(50)), CAButtonTypeRoundedRect);
    storeBtn->setAllowsSelected(true);
    storeBtn->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
    storeBtn->addTarget(this, CAControl_selector(MainViewTableCell::storeBtnCallBack), CAControlEventTouchUpInSide);
    this->getContentView()->addSubview(storeBtn);
}

void MainViewTableCell::setModel(newsMsg &cellmodel)
{
    m_msgInfo = &cellmodel;
    
    m_titleLabel->setText(cellmodel.m_title);
    m_lectureLabel->setText(cellmodel.m_lecturer);
    m_lectureDetailLabel->setText(cellmodel.m_detail);
    m_timeLabel->setText(crossapp_format_string("%s - %s", timeToString(cellmodel.m_startTime).c_str(), timeToString(cellmodel.m_endTime).c_str()));//%lld
    m_likeNumLabel->setText(crossapp_format_string("%d", cellmodel.m_likeNum));
    m_urlImageView->setUrl(cellmodel.m_imageUrl);
    
    m_isStore = true;
    m_canStore = true;
    
}

void MainViewTableCell::storeBtnCallBack(CAControl* btn, DPoint point)
{
    if (m_canStore)
    {
        std::map<std::string, std::string> key_value;
        key_value["tag"] = sessionViewTag[1];
        key_value["sid"] = crossapp_format_string("%d", m_msgInfo->m_sessionId);
        key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
        //key_value["sto"] = crossapp_format_string("%d", m_isStore ? 1 : 0);
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onStoreRequestFinished));
        
        m_canStore = false;
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
        m_msgInfo->m_stored = m_isStore;
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