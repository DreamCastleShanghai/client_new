
#ifndef __Main__ViewTableCell__
#define __Main__ViewTableCell__

#include "RootWindow.h"

USING_NS_CC;

class MainViewTableCell : public CATableViewCell
{
public:

	MainViewTableCell();
	virtual ~MainViewTableCell();

	static MainViewTableCell* create(const std::string& identifier, const DRect& _rect = DRectZero);
	virtual void highlightedTableViewCell();
	virtual void selectedTableViewCell();

	void initWithCell();
	void setModel(sessionMsg& cellmodel);
    
    void storeBtnCallBack(CAControl* btn, DPoint point);
    void onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

    CALabel*								m_titleLabel;
    CALabel*								m_lectureLabel;
    CALabel*								m_lectureDetailLabel;
    CALabel*								m_timeLabel;
    CALabel*                                m_likeNumLabel;
    CAImageView*                            m_storeBtnImage;
    CommonUrlImageView*						m_urlImageView;
    
    struct sessionMsg*							m_msgInfo;
    bool									m_isStore;
    bool									m_canStore;
};

#endif /* defined(__Login__ViewController__) */
