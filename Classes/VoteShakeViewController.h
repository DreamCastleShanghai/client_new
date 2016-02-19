
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

//	VoteShakeViewController(demoJamMsg* dMsg, voiceMsg* vMsg);
    
    VoteShakeViewController(void* msg, int type);
    
	virtual ~VoteShakeViewController();
    
    enum
    {
        TYPE_DJ,
        TYPE_VOICE,
        TYPE_EH,
        TYPE_MAX
    };
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
        
        SHAKE_CNT = 20,
    };
	DSize                                       m_winSize;
    void*                                       m_msg;
    int                                         m_tye;
    CAView*                                     m_view;
    CAView*                                     m_shakeView;
    CAView*                                     m_shakeEndView;
//	demoJamMsg*                                 m_demoMsg;
//    voiceMsg*                                   m_voiceMsg;
    bool                                        m_canVote;
    bool                                        m_voted;
    int                                         m_voteStatus;
    int                                         m_shakeNum;
    CAView*                                     p_alertView;
    CAActivityIndicatorView*                    p_pLoading;
    CALabel*                                    m_shakeNumLabel;
    int                                         m_projectType;
};


#endif /* defined(__HelloCpp__ViewController__) */
