
#ifndef __SessionDetail__ViewController__
#define __SessionDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

struct surveyFeedBack
{
	int score;
	CHAR* detail;
};

class SessionDetailViewController :
	public CAViewController
{
    
public:

	SessionDetailViewController(sessionMsg &msg);
    
	virtual ~SessionDetailViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
	void requestLike();
	void onLikeRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    void requestStore();
    
    void onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

	DSize													m_winSize;
	CAView*													m_surveyButtonView[2];
	CALabel*												m_surveyTimeLabel;

	sessionMsg*                                             m_msg;
	sessionDetailMsg										m_detailMsg;
    bool                                                    m_canStore;
    bool                                                    m_isStore;
    CAImageView*                                            m_storeBtnImage;

	bool                                                    m_canLike;
	bool                                                    m_isLiked;
	CAImageView*                                            m_likeBtnImage;
	CALabel*												m_likeNumLabel;

	CAView*													p_alertView;
	CAActivityIndicatorView*								p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
