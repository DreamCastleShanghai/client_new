
#ifndef __Main__ViewController__
#define __Main__ViewController__

#include "RootWindow.h"

class MainViewController :
public CAViewController,
public CATableViewDelegate,
public CATableViewDataSource,
public CAPageViewDelegate,
public CACollectionViewDelegate,
public CAScrollViewDelegate,
public CADatePickerViewDelegate
{
    
public:

	MainViewController();
    
	virtual ~MainViewController();
    
public:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();
    
    void update(float dt);
    
//	void initMsgTableView();

//	void initPageView();
    
    void requestSessionMsg();
    
    void initPageView();
    
//    void requestIconMsg();

	void showAlert();

	void refreshBtnCallBack(CAControl* btn, DPoint point);

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	//void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	void buttonCallBack(CAControl* btn, DPoint point);
    
    void updatePageView(float dt);
    
    void refreshTable();
    
    void reloadTableData();

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void pageViewDidBeginTurning(CAPageView* pageView);
	virtual void pageViewDidEndTurning(CAPageView* pageView);
    virtual void pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    
	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
private:
    
	std::vector<sessionMsg>*								m_msg;
    std::vector<sessionMsg*>								m_filterMsg;
	std::vector<newsPage>									m_page;

    CAImageView*                                            m_timeNoticeImageView;
    CAImageView*                                            m_sessionNoticeImageView;
//    CAPageView*                                             m_totalView;
//    CAVector<CAView*>                                       m_totalViewList;
	CATableView*											m_msgTableView;
	CAPageView*												m_pageView;
	CAPageControl*											m_pageControl;
    CAView*                                                 m_pageControlBG;
	CALabel*												m_pageViewTitle;
	CAView*													p_alertView;
	CAActivityIndicatorView*								p_pLoading;
//    int														m_pastSection;
//	int														m_nextSection;
    time_t                                                  m_timeForPageView;

	DSize													m_winSize;
    CAViewController*                                       m_sustainbilitySurvey;
    CAViewController*                                       m_monent;
    CAViewController*                                       m_vote;
    CAViewController*                                       m_map;
    CAViewController*                                       m_session;
    CAViewController*                                       m_my;
    
    CAPullToRefreshView *                               m_headRefreshView;
    CAPullToRefreshView *                               m_rootRefreshView;
};


#endif /* defined(__Login__ViewController__) */
