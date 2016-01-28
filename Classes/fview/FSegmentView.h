
#ifndef __FSegment__View__
#define __FSegment__View__

#include <iostream>
#include "CrossApp.h"

class FSegmentView;

typedef void (CAObject::*SEL_FSegmentView)(FSegmentView*, int index);
#define FSegmentControl_selector(_SELECTOR) (SEL_FSegmentedControl)(&_SELECTOR)

class FSegmentView : public CAView
{
    
public:

	FSegmentView();
    
	virtual ~FSegmentView();
    
    virtual bool initWithFrame(const DRect& rect);
    
    static FSegmentView* createWithFrame(const DRect& rect, int num);
    
    static FSegmentView* createWithFrame(const DRect &rect, int num, int type);
    
    virtual void addTarget(CAObject* target, SEL_CAControl selector, CAControlEvents event);
    
    virtual void setItemFocus(int index);
    
    void buttonCallBack(CAControl* btn, DPoint point);
//    
    virtual void setItemBackGroundImage(CAImageView* imageView, int index);
//    
    virtual void setItemTile(std::string tile, int index);
    
    virtual void setItemTile(std::string tile, int index, int type);
//    
//    virtual void setItemTileFontName(std::string fontName);
//    
//    virtual void setItemTileFontSize(int size);
//    
//    virtual void setItemTileDRect(DRect rect);
    
    virtual void setTag(int tag, int index);
    
    virtual std::vector<CAButton*>& getButtonList() { return m_fSegButtonList; }
    
    void addTarget(CAObject* target, SEL_FSegmentView selector);
    
    void resetSegment();
    
protected:
    
    

    
private:

	DSize													m_fSegSize;
    std::vector<CAButton*>                                  m_fSegButtonList;
    
    CAObject*                                               m_pTarget;
    SEL_CAControl                                           m_pCallFunc;
    CAControlEvents                                         m_pEvent;
    int                                                     m_type;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
