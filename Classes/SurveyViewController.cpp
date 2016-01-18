
#include "SurveyViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

SurveyViewController::SurveyViewController()
: p_alertView(NULL)
, p_pLoading(NULL)
, m_listView(NULL)
{
    
}

SurveyViewController::~SurveyViewController()
{

}

void SurveyViewController::viewDidLoad()
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
    button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    this->getView()->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, _px(70), m_winSize.width, _px(40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setFontSize(_px(40));
    label->setText("Sustainability Survey");
    label->setFontName("fonts/arial.ttf");
    sView->addSubview(label);

    requestMsg();
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SurveyViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SurveyViewController::initMsgTableView()
{
	if (m_msg.empty())
	{
		showAlert();
		return;
	}
	m_listView = CAListView::createWithFrame(DRect(_px(40), _px(120), m_winSize.width - _px(80), m_winSize.height - _px(220)));
	m_listView->setListViewDataSource(this);
	m_listView->setAllowsSelection(false);
	m_listView->setListViewOrientation(CAListViewOrientationVertical);
	//m_listView->setShowsScrollIndicators(false);
	m_listView->setSeparatorColor(CAColor_clear);
	m_listView->setBackGroundColor(ccc4(0xf6, 0xf6, 0xf6, 0xff));
	//m_filterListView->setScrollEnabled(false);

	this->getView()->addSubview(m_listView);

	CAButton* button = CAButton::createWithFrame(DRect(_px(40), m_winSize.height - _px(160), m_winSize.width - _px(80), _px(120)), CAButtonTypeCustom);
	CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/sky_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateAll, imageView);
	imageView = CAImageView::createWithImage(CAImage::create("common/gray_bg.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
	button->setBackGroundViewForState(CAControlStateDisabled, imageView);
	button->setTitleColorForState(CAControlStateAll, CAColor_white);
	button->setTitleForState(CAControlStateAll, "Finish");
	button->setTitleFontSize(_px(36));
	button->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
	//button->setControlState(CAControlStateDisabled);
	button->setTag(200);
	this->getView()->addSubview(button);
}

void SurveyViewController::requestMsg()
{
    if(p_alertView)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    std::map<std::string, std::string> key_value;
	key_value["tag"] = suveyTag[0];
    key_value["uid"] = crossapp_format_string("%d", FDataManager::getInstance()->getUserId());
    //key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SurveyViewController::onRequestFinished));
    {
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(SurveyViewController::initMsgTableView));
    }
}

void SurveyViewController::buttonCallBack(CAControl* btn, DPoint point)
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

void SurveyViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
    }
    else
    {
        //showAlert();
    }
    
    {
		for (int i = 0; i < 3; i++)
		{
			surveyDetail tmp;
			tmp.m_itemId = i;
			tmp.m_surveyId = 1;
			tmp.m_itemDetail = "title:  This is Photoshop's version of Lorem, This is Photoshop's version of Lorem,";
			for (int j = 0; j < 3; j++)
			{
				tmp.m_itemSelection[j] = "selection: This is Photoshop's version of Lorem, This is Photoshop's version of Lorem,";
			}
			m_msg.push_back(tmp);
		}
		
		
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
}


void SurveyViewController::showAlert()
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
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(SurveyViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(30)));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    test->setFontSize(_px(24));
    test->setText("Network cannot connect!");
    p_alertView->addSubview(test);

}


void SurveyViewController::listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index)
{

}

void SurveyViewController::listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index)
{

}

unsigned int SurveyViewController::numberOfIndex(CAListView *listView)
{
	int num = 1;

	return num;
}

unsigned int SurveyViewController::listViewHeightForIndex(CAListView *listView, unsigned int index)
{
	int width = _px(1000);
	return width;
}

CAListViewCell* SurveyViewController::listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index)
{
	DSize _size = cellSize;
	CAListViewCell* cell = (CAListViewCell*)listView->dequeueReusableCellWithIdentifier("ListViewCell");
	if (cell == NULL)
	{
		cell = CAListViewCell::create("ListViewCell");

		for (int i = 0; i < 3; i++)
		{
			CAView* view = CAView::createWithFrame(DRect(0, _px(40) + i * _px(300), _size.width, _px(300)));
			CALabel* label = CALabel::createWithFrame(DRect(0, 0, _size.width, _px(60)));
			label->setText(m_msg[index].m_itemDetail);
			label->setColor(CAColor_black);
			label->setFontSize(_px(24));
			label->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
			view->addSubview(label);
			for(int j = 0; j < 3; j++)
			{
				label = CALabel::createWithFrame(DRect(_px(40), _px(80) + j * _px(60), _size.width - _px(40), _px(60)));
				label->setText(m_msg[index].m_itemSelection[i]);
				label->setColor(CAColor_black);
				label->setFontSize(_px(24));
				label->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
				view->addSubview(label);
			}
			cell->addSubview(view);
		}
		
	}

	return cell;
}