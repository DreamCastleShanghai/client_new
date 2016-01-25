
#ifndef __VoteShake__ViewController__
#define __VoteShake__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class VoteShakeViewController
: public CAViewController,
  public CCAccelerometerDelegate
{
    
public:

	VoteShakeViewController(demoJamMsg* dMsg, voiceMsg* vMsg);
    
	virtual ~VoteShakeViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void initView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void requestMsg(int type);

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void onRequestVoteFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
private:

    enum
    {
        Vote_NotStart = 0,
        Vote_Start,
        Vote_End,
    };
	DSize													m_winSize;
    CAView*                                             m_view;
    CAView*                                             m_shakeView;
    CAView*                                             m_shakeEndView;
	demoJamMsg*                                  m_demoMsg;
    voiceMsg*                                            m_voiceMsg;
    bool                                                        m_canVote;
    bool                                                         m_voted;
    int                                                         m_voteStatus;
    int                                                         m_shakeNum;
    CAView*                                             p_alertView;
    CAActivityIndicatorView*                p_pLoading;
    CALabel*                                        m_shakeNumLabel;
    int             m_projectType;
};


#endif /* defined(__HelloCpp__ViewController__) */
