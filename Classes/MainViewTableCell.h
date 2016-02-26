
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

	void requstStore();
	void requestLike();

	void initWithCell(sessionMsg &msg);
    
    void buttonCallback(CAControl* btn, DPoint point);
    void onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	void onLikeRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    static void updateTable();

private:

    CALabel*								m_titleLabel;
    CALabel*								m_lectureLabel;
    CALabel*								m_locationLabel;
    CALabel*								m_timeLabel;
    CAButton*                               m_storeBtn;
    CAImageView*                            m_storeBtnImage;
    CAButton*                               m_likeBtn;
	CAImageView*                            m_likeBtnImage;
    CALabel*                                m_likeNumLabel;
    CommonUrlImageView*						m_urlImageView;
    CAImageView*                            m_rightArrow;
    
    struct sessionMsg*						m_msg;

//    bool									m_isStore;
//    bool									m_canStore;
//	bool									m_isLike;


};

#endif /* defined(__Login__ViewController__) */
