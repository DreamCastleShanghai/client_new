
#ifndef __SessionDetail__ViewController__
#define __SessionDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

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
    
    void adjustSurveyBtn(float dt);
    
private:
    
	DSize													m_winSize;
	CAView*													m_surveyButtonView;
	CALabel*												m_surveyTimeLabel;

	sessionMsg*                                             m_msg;
	sessionDetailMsg										m_detailMsg;
    CAImageView*                                            m_storeBtnImage;

	CAImageView*                                            m_likeBtnImage;
	CALabel*												m_likeNumLabel;

	CAView*													p_alertView;
	CAActivityIndicatorView*								p_pLoading;
    
    bool                                                    m_isSurveyed;
    CAButton*                                               m_surveyBtn;
    CALabel*                                                m_surveyBtnLabel1;
    CALabel*                                                m_surveyBtnLabel2;
};


#endif /* defined(__HelloCpp__ViewController__) */
