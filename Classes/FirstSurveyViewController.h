
#ifndef __FirstSurvey__ViewController__
#define __FirstSurvey__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class FirstSurveyViewController :
	public CAViewController
{
    
public:

	FirstSurveyViewController();
    
	virtual ~FirstSurveyViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

	void buttonCallBack(CAControl* btn, DPoint point);

	void requestSubmit();

	void onRequestSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    
private:

	DSize                                                              m_winSize;

    CATextView*                                                 m_textView;
	CAActivityIndicatorView*								m_pLoading;
    CALabel*                                                        m_alertLabel;
	
};


#endif /* defined(__HelloCpp__ViewController__) */
