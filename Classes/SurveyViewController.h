
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
    enum
    {
        STAR_CNT = 5,
        OPTN_CNT = 4,
    };

	SurveyViewController(int sessionId);
    
	virtual ~SurveyViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void requestMsg();
    
    void getSurveyInfo();
    
    void onAlert(int bid);

    void onSurveyInfoFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

private:

	DSize													m_winSize;
	struct surveyDetail                                            m_surveyDetail;
//	int														m_score;
	int														m_sessionId;
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
	CALabel*												m_alertLabel;
	std::vector<CAButton*>									m_scoreButtonVec;
    CAImage*                                                m_stars[STAR_CNT];
	CAListView*												m_listView;
	CATextView*												m_feedBackTextView;
    
    CALabel*                                                m_question1;
    CALabel*                                                m_question2;
    
    CALabel*                                                m_option1[OPTN_CNT];
    CALabel*                                                m_option2[OPTN_CNT];
    
    CAButton*                                               m_optionBtn1[OPTN_CNT];
    CAButton*                                               m_optionBtn2[OPTN_CNT];
    
    int                                                     m_choose1;
    int                                                     m_choose2;
    int                                                     m_chooseStar;
};


#endif /* defined(__HelloCpp__ViewController__) */
