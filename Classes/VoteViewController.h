
#ifndef __Vote__ViewController__
#define __Vote__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class VoteViewController :
	public CAViewController,
    public CAPageViewDelegate,
    public CCAccelerometerDelegate
{
    
public:

	VoteViewController();
    
	virtual ~VoteViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();
    
    void refreshPageView(int index);
    
    void setDetailView(int type, int index);

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    virtual void pageViewDidBeginTurning(CAPageView* pageView);
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
private:

	DSize													m_winSize;

    bool                                                    m_canVote;
    time_t                                                  m_leaveTime;
    int                                                     m_pageType;
    int                                                     m_currentPage[2];
    
    std::vector<voteMsg>                                    m_msg;
    std::vector<voteMsg>									m_demoMsg;
    std::vector<voteMsg>									m_voiceMsg;
    CAPageView*												m_pageView;
    CAPageControl*											m_pageControl[2];
    CAView*                                                 m_detailView;
    
    //CAListView*												m_detailListView;
    CALabel*                                                m_voteLabel;
    CAImageView*                                            m_segBgView[2];
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
