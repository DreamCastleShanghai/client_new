
#ifndef __Survey__ViewController__
#define __Survey__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SurveyViewController
:   public CAViewController
{
    
public:

	SurveyViewController();
    
	virtual ~SurveyViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

	DSize													m_winSize;

    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
