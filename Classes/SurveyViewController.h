
#ifndef __Survey__ViewController__
#define __Survey__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SurveyViewController
: public CAViewController
{
    
public:

	SurveyViewController(int sessionId);
    
	virtual ~SurveyViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void requestMsg();
    
    void getSurveyInfo();

    void onSurveyInfoFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

private:

	DSize													m_winSize;
	surveyDetail                                            m_surveyDetail;
//	int														m_score;
	int														m_sessionId;
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
	CALabel*												m_alertLabel;
	std::vector<CAButton*>									m_scoreButtonVec;
	CAListView*												m_listView;
	CATextView*												m_feedBackTextView;
};


#endif /* defined(__HelloCpp__ViewController__) */
