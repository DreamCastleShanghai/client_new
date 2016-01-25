
#ifndef __VoteDetail__ViewController__
#define __VoteDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class VoteDetailViewController
: public CAViewController
{
    
public:

	VoteDetailViewController();
    
	virtual ~VoteDetailViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

private:

	DSize													m_winSize;
	demoJamMsg*                                  m_demoMsg;
    voiceMsg*                                            m_voiceMsg;

    int                                                         m_voteType;
    
    CAView*                                             p_alertView;
    CAActivityIndicatorView*                p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
