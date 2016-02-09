
#include "PrizeViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

PrizeViewController::PrizeViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
{
    m_pointInfo.m_userId = 0;
}

PrizeViewController::~PrizeViewController()
{

}

void PrizeViewController::viewDidLoad()
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
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(PrizeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Prize");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);
    requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void PrizeViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void PrizeViewController::initMsgTableView()
{
    if (m_pointInfo.m_userId == 0)
    {
        showAlert();
        return;
    }
    
    m_msgTableView = CATableView::createWithFrame(DRect(0, _px(120), m_winSize.width, _px(600)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    m_msgTableView->setSeparatorColor(ccc4(200, 200, 200, 80));
    //m_msgTableView->setSeparatorViewHeight(_px(2));
    this->getView()->addSubview(m_msgTableView);
    

    CAButton* button = CAButton::createWithFrame(DRect(_px(40), m_winSize.height - _px(160), m_winSize.width - _px(80), _px(120)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->setTitleColorForState(CAControlStateAll, CAColor_white);
    button->setTitleForState(CAControlStateAll, "View My Points");
    button->setTitleFontSize(_px(36));
    button->addTarget(this, CAControl_selector(PrizeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(200);
    this->getView()->addSubview(button);
}

void PrizeViewController::requestMsg()
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
    key_value["tag"] = userInfoTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(PrizeViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(PrizeViewController::initMsgTableView));
    }
}

void PrizeViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
    else if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
        requestMsg();
    }
    else if (btn->getTag() == 200)
    {
        
    }
}

void PrizeViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
//        const CSJson::Value& value = json["result"];
//        m_pointInfo.m_userId = value["uid"].asInt();
//        m_pointInfo.m_point = value["pnt"].asInt();
//        for (int i = 0; i < value["pid"].size(); i++)
//        {
//            m_pointInfo.m_hasPrizeId.push_back(value["pid"].asInt());
//        }
//        m_pointInfo.m_pointRank = value["prk"].asInt();
//        m_pointInfo.m_rankLimit = value["rlt"].asInt();
//        m_pointInfo.m_miLimit = value["mlt"].asInt();
    }
    else
    {
        //showAlert();
    }
#ifdef LOCALTEST
    {
        m_pointInfo.m_userId = 100;
        m_pointInfo.m_pointRank = 256;
        m_pointInfo.m_point = 200;
        m_pointInfo.m_hasPrizeId.push_back(0);
        m_pointInfo.m_rankLimit = 10;
        m_pointInfo.m_miLimit = 300;
    }
#endif
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}


void PrizeViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(DRect(_px(0), _px(120), m_winSize.width, m_winSize.height - _px(120)));
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(120)));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setFrame(DRect(_px(0), _px(0), m_winSize.width, m_winSize.height - _px(120)));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackgroundViewForState(CAControlStateNormal, bg);
    btn5->setBackgroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(PrizeViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}


CATableViewCell* PrizeViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    
    CATableViewCell* cell = dynamic_cast<CATableViewCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
//        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
//        CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
//        sView->setFrame(DRect(_px(0), _px(0), _size.width, _size.height));
//        cell->setBackgroundView(sView);
//
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        if(row == 0)
        {
            CAView* view = CAView::createWithFrame(DRect(_px(20), _px(40), _px(80), _px(80)));
            CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/prize_mi_bg.png"));
            imageView->setFrame(DRect(_px(0), _px(0), _px(80), _px(80)));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            view->addSubview(imageView);
            imageView = CAImageView::createWithImage(CAImage::create("main/nav_prize.png"));
            imageView->setFrame(DRect(_px(10), _px(10), _px(60), _px(60)));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            view->addSubview(imageView);
            
            cell->addSubview(view);
            
            CALabel* label = CALabel::createWithFrame(DRect(_px(140), _px(40), _size.width / 2, _px(40)));
            label->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
            label->setTextAlignment(CATextAlignmentLeft);
            label->setText("MI Band");
            label->setFontSize(_px(32));
            cell->addSubview(label);
            
            label = CALabel::createWithFrame(DRect(_px(140), _px(90), _size.width / 2, _px(30)));
            label->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
            label->setTextAlignment(CATextAlignmentLeft);
            label->setText(crossapp_format_string("%d Points", m_pointInfo.m_miLimit));
            label->setFontSize(_px(26));
            cell->addSubview(label);
            
            CAButton* button = CAButton::createWithFrame(DRect(m_winSize.width - _px(190), _px(50), _px(150), _px(50)), CAButtonTypeSquareRect);
            button->setTitleForState(CAControlStateAll, "Exchange");
            button->setTitleColorForState(CAControlStateAll, ccc4(0, 0xA8, 0xFC, 0xff));
            button->setTitleColorForState(CAControlStateDisabled, CAColor_gray);
            button->setTitleFontSize(_px(25));
            if(m_pointInfo.m_point < m_pointInfo.m_miLimit)
            {
                button->setControlStateDisabled();
            }
            
            for (int i = 0; i < m_pointInfo.m_hasPrizeId.size(); i++)
            {
                if (m_pointInfo.m_hasPrizeId[i] == 0)
                {
                    button->setTitleForState(CAControlStateAll, "Exchanged");
                    button->setControlStateDisabled();
                }
            }
            cell->addSubview(button);
            
            CAProgress* progress = CAProgress::create();
            progress->setFrame(DRect(_px(140), _px(130), m_winSize.width - _px(180), _px(50)));
            progress->setProgressTrackImage(CAImage::create("seggreen_bg.png"));
            progress->setProgressTintColor(ccc4(0x5f, 0xd5, 0xc7, 0xff));
            progress->setProgressTrackColor(CAColor_gray);
            
            label = CALabel::createWithFrame(DRect(_px(0), _px(0), m_winSize.width - _px(180), _px(50)));
            label->setColor(ccc4(0x23, 0x34, 0x38, 0xff));
            label->setTextAlignment(CATextAlignmentCenter);
            label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
            label->setFontSize(_px(32));
            if (m_pointInfo.m_point < m_pointInfo.m_miLimit) {
                label->setText(crossapp_format_string("my point: %d", m_pointInfo.m_point));
                progress->setProgress((float)m_pointInfo.m_point / m_pointInfo.m_miLimit);
            }
            else if(m_pointInfo.m_point >= m_pointInfo.m_miLimit)
            {
                label->setText("Qualified");
                progress->setProgress(1.0f);
            }
            else
            {
                label->setText("Exchanged");
                progress->setProgress(0.f);
            }
            progress->addSubview(label);
            cell->addSubview(progress);
            
        }
        else if(row == 1)
        {
            CAView* view = CAView::createWithFrame(DRect(_px(20), _px(40), _px(80), _px(80)));
            CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/prize_kindle_bg.png"));
            imageView->setFrame(DRect(_px(0), _px(0), _px(80), _px(80)));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            view->addSubview(imageView);
            imageView = CAImageView::createWithImage(CAImage::create("main/nav_prize.png"));
            imageView->setFrame(DRect(_px(10), _px(10), _px(60), _px(60)));
            imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
            view->addSubview(imageView);
            
            cell->addSubview(view);
            
            CALabel* label = CALabel::createWithFrame(DRect(_px(140), _px(40), _size.width / 2, _px(40)));
            label->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
            label->setTextAlignment(CATextAlignmentLeft);
            label->setText("Kindle");
            label->setFontSize(_px(32));
            cell->addSubview(label);
            
            label = CALabel::createWithFrame(DRect(_px(140), _px(90), _size.width / 2, _px(30)));
            label->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
            label->setTextAlignment(CATextAlignmentLeft);
            label->setText("Top 10 of Total Points");
            label->setFontSize(_px(26));
            cell->addSubview(label);
            
            label = CALabel::createWithFrame(DRect(_px(140), _px(130), _size.width / 2, _px(30)));
            label->setColor(ccc4(0x5f, 0x5f, 0x5f, 0xff));
            label->setTextAlignment(CATextAlignmentLeft);
            label->setText(crossapp_format_string("Current Ranking: %d", m_pointInfo.m_pointRank));
            label->setFontSize(_px(26));
            cell->addSubview(label);
            
            CAButton* button = CAButton::createWithFrame(DRect(m_winSize.width - _px(190), _px(50), _px(150), _px(50)), CAButtonTypeSquareRect);
            button->setTitleForState(CAControlStateAll, "Exchange");
            button->setTitleColorForState(CAControlStateAll, ccc4(0, 0xA8, 0xFC, 0xff));
            button->setTitleColorForState(CAControlStateDisabled, CAColor_gray);
            button->setTitleFontSize(_px(25));
            if (m_pointInfo.m_pointRank > m_pointInfo.m_rankLimit)
            {
                button->setControlStateDisabled();
            }
            for (int i = 0; i < m_pointInfo.m_hasPrizeId.size(); i++)
            {
                if (m_pointInfo.m_hasPrizeId[i] == 1)
                {
                    button->setTitleForState(CAControlStateAll, "Exchanged");
                    button->setControlStateDisabled();
                }
            }

            cell->addSubview(button);
        }
    }
    //cell->setModel(m_msgFilter[row]);
    
    return cell;
    
}

unsigned int PrizeViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int PrizeViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 2;
}

unsigned int PrizeViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(240);
}

void PrizeViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{

}

void PrizeViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}
