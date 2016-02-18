
#include "NoticeDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

NoticeDetailViewController::NoticeDetailViewController(std::string &detail)
: m_detail(detail)
{
 
}

NoticeDetailViewController::~NoticeDetailViewController()
{

}

void NoticeDetailViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png"));
    sView->setFrame(DRect((0), (0), m_winSize.width, (120)));
    this->getView()->addSubview(sView);
    
    CAButton* button = CAButton::createWithFrame(DRect((0), (20), (100), (100)), CAButtonTypeCustom);
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("common/nav_back.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
    imageView->setFrame(DRect((20), (20), (80), (80)));
    button->setBackgroundViewForState(CAControlStateAll, imageView);
    button->addTarget(this, CAControl_selector(NoticeDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
    button->setTag(20);
    sView->addSubview(button);
    
    CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width / 2, (70), m_winSize.width, (40)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_white);
    label->setTouchEnabled(false);
    label->setFontSize((40));
    label->setText("Notice");
    label->setFontName(SAP_FONT_ARIAL);
    sView->addSubview(label);
    
    label = CALabel::createWithFrame(DRect(0, (200), m_winSize.width, (50)));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setColor(CAColor_gray);
    label->setTouchEnabled(false);
    label->setFontSize((40));
    label->setText("New message!");
    label->setFontName(SAP_FONT_ARIAL);
    this->getView()->addSubview(label);
    
    label = CALabel::createWithFrame(DRect(40, (300), m_winSize.width - 80, (800)));
    label->setTextAlignment(CATextAlignmentLeft);
    label->setColor(CAColor_gray);
    label->setTouchEnabled(false);
    label->setFontSize((30));
    label->setText(m_detail);
    label->setFontName(SAP_FONT_ARIAL);
    this->getView()->addSubview(label);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void NoticeDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void NoticeDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 20)
    {
        RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
    }
}