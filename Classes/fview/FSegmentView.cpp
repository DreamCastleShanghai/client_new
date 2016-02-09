
#include "FSegmentView.h"

FSegmentView::FSegmentView()
: m_type(0)
{
    m_fSegButtonList.clear();
}

FSegmentView::~FSegmentView()
{

}

bool FSegmentView::initWithFrame(const DRect& rect)
{
    if (!CAView::initWithFrame(rect,CAColor_white))
    {
        return false;
    }
    return true;
}

FSegmentView* FSegmentView::createWithFrame(const DRect &rect, int num)
{
    FSegmentView * pRet = new FSegmentView();
    if (pRet && pRet->initWithFrame(rect))
    {
        for (int i = 0; i < num; i++)
        {
            DRect r((i + _px(1)) +  (rect.size.width - num - _px(1)) * i / num, _px(1), (rect.size.width - num) / num, rect.size.height - _px(2));
            CAButton* button = CAButton::createWithFrame(r, CAButtonTypeCustom);
            button->setAllowsSelected(true);
            CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("source_material/dsky_bg.png"));
            button->setBackgroundViewForState(CAControlStateSelected, sView);
            sView = CAScale9ImageView::createWithImage(CAImage::create("source_material/sky_bg.png"));
            button->setBackgroundViewForState(CAControlStateNormal, sView);
            pRet->m_fSegButtonList.push_back(button);
            pRet->addSubview(button);
        }
        pRet->m_fSegButtonList[0]->setControlState(CAControlStateSelected);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

FSegmentView* FSegmentView::createWithFrame(const DRect &rect, int num, int type)
{
    FSegmentView * pRet = new FSegmentView();
    if (pRet && pRet->initWithFrame(rect))
    {
        pRet->m_type = type;
        pRet->setColor(CAColor_clear);
        for (int i = 0; i < num; i++)
        {
            DRect r((i + _px(1)) +  (rect.size.width - num - _px(1)) * i / num, _px(1), (rect.size.width - num) / num, rect.size.height - _px(2));
            CAButton* button = CAButton::createWithFrame(r, CAButtonTypeCustom);
            button->setAllowsSelected(true);
            pRet->m_fSegButtonList.push_back(button);
            pRet->addSubview(button);
        }
        pRet->m_fSegButtonList[0]->setControlState(CAControlStateSelected);
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void FSegmentView::resetSegment()
{
    m_fSegButtonList[0]->setControlState(CAControlStateSelected);
    for (int i = 1; i < m_fSegButtonList.size(); i++)
    {
        m_fSegButtonList[i]->setControlState(CAControlStateNormal);
    }
}

void FSegmentView::setItemTile(std::string tile, int index)
{
    if(m_fSegButtonList.size() > index)
    {
        DRect r = m_fSegButtonList[index]->getFrame();
        
        CALabel* label =
            CALabel::createWithCenter(DRect(r.size.width / 2, r.size.height / 2, r.size.width - _px(5), r.size.height - _px(5)));
        label->setFontSize(r.size.height / 2);
        label->setColor(CAColor_white);
        label->setText(tile);
        label->setTextAlignment(CATextAlignmentCenter);
        label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        label->setTouchEnabled(false);
        m_fSegButtonList[index]->insertSubview(label, 11);
    }
}

void FSegmentView::setItemTile(std::string tile, int index, int type)
{
    m_fSegButtonList[index]->setTitleForState(CAControlStateAll, tile);
    m_fSegButtonList[index]->setTitleColorForState(CAControlStateAll, CAColor_white);
    m_fSegButtonList[index]->setTitleColorForState(CAControlStateSelected, ccc4(0xce, 0xea, 0xfd, 0xff));
}

void FSegmentView::setItemBackgroundImage(CAImageView* imageView, int index)
{
    if(imageView && m_fSegButtonList.size() > index)
    {
        DRect r = m_fSegButtonList[index]->getFrame();
        imageView->setCenter(DRect(r.size.width / 2, r.size.height / 2, r.size.width - _px(5), r.size.height - _px(5)));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
        imageView->setTouchEnabled(false);
        m_fSegButtonList[index]->insertSubview(imageView, 10);
    }
}

void FSegmentView::setTag(int tag, int index)
{
    if(m_fSegButtonList.size() > index)
    {
        m_fSegButtonList[index]->setTag(tag);
    }
}

void FSegmentView::addTarget(CAObject *target, SEL_CAControl selector, CAControlEvents event)
{
    for (int i = 0; i < m_fSegButtonList.size(); i++)
    {
        m_fSegButtonList[i]->addTarget(this, CAControl_selector(FSegmentView::buttonCallBack), event);
    }
    m_pTarget = target;
    m_pCallFunc = selector;
}

void FSegmentView::setItemFocus(int index)
{
    for (int i = 0; i < m_fSegButtonList.size(); i++)
    {
        if (i == index)
        {
            m_fSegButtonList[i]->setControlState(CAControlStateSelected);
        }
        else
        {
            m_fSegButtonList[i]->setControlState(CAControlStateNormal);
        }
    }
}

void FSegmentView::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getControlState() == CAControlStateNormal)
    {
        btn->setControlState(CAControlStateSelected);
        return;
    }
    for (int i = 0; i < m_fSegButtonList.size(); i++)
    {
        if (m_fSegButtonList[i] != btn)
        {
            m_fSegButtonList[i]->setControlState(CAControlStateNormal);
        }
    }
    if(m_pTarget && m_pCallFunc)
    {
        ((CAObject *)m_pTarget->*m_pCallFunc)(btn,point);
    }
}
