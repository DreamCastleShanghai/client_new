
#ifndef __Vote__ViewController__
#define __Vote__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class VoteViewController :
	public CAViewController,
    public CAPageViewDelegate
//,
//    public CCAccelerometerDelegate
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
    
    void setTitleDetailView(int type, int index);

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    virtual void pageViewDidBeginTurning(CAPageView* pageView);
    virtual void pageViewDidEndTurning(CAPageView* pageView);
    
    //virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
private:
    enum
    {
        SEG_PAGE = 3,
    };

    DSize                                       m_winSize;
    
    int                                         m_navType;
    std::vector<demoJamMsg>                     m_demoMsg;
    std::vector<voiceMsg>                       m_voiceMsg;
    std::vector<eggHikingMsg>                   m_hikMsg;
    
    CALabel*                                    m_titleLabel[SEG_PAGE];
    CALabel*                                    m_detailLabel[SEG_PAGE];
    CAView*                                     m_segView[SEG_PAGE];
    CAPageView*                                 m_pageView[SEG_PAGE];
    CAPageControl*								m_pageControl[SEG_PAGE];
    //CALabel*                                    m_titleView[SEG_PAGE];
    //CAView*                                     m_detailView[SEG_PAGE];
    CAImageView*                                m_segBgView[SEG_PAGE];
    int                                         m_currentPage[SEG_PAGE];
    
    int                                         m_imageHeight;
    
    bool                                        m_canVote;
    time_t                                      m_leaveTime;
    int                                         m_pageType;
    
    
    //CAListView*												m_detailListView;
    CALabel*                                    m_voteLabel;
    
    CAView*                                     p_alertView;
    CAActivityIndicatorView*					p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
