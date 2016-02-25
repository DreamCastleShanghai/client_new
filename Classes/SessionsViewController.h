
#ifndef __Sessions__ViewController__
#define __Sessions__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

#define m_timeFilterNum 15

typedef struct
{
	CALabel* lab;
	CAImageView* imgView;
} TabItemView;

class SessionsViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource,
	public CAScrollViewDelegate,
    public CAListViewDelegate,
    public CAListViewDataSource
{
    
public:

	SessionsViewController();
    
	virtual ~SessionsViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void viewDidAppear();
    
    virtual void viewDidDisappear();

    void initMsgTableView();

	void refreshTableData();
    
    void refreshTableByTime(int index);
    
    void refreshTableByFormat(int track, int format);
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    
    virtual void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index);
    virtual void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index);
    virtual unsigned int numberOfIndex(CAListView *listView);
    virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index);
    virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index);
    
    
	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
private:

	DSize													m_winSize;
    
    std::vector<sessionMsg>*								m_msg;
    std::vector<sessionMsg*>								m_msgFilter;
    
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
    CAImageView*                                            m_sessionNoticeImageView;
	CATableView*											m_msgTableView;
    CAListView*                                             m_listView;
    CAView*													m_filterView;
	CAView*													m_downView[2];
    CAButton*                                               m_filterBtn[2];
	std::vector<CAView*>									m_filterViewVec;

	std::vector<CAButton*>                                  m_trackButtonVec;
	std::vector<CAButton*>                                  m_formatButtonVec;

    int                                                     m_navType;
    int                                                     m_navTimeType;

	int                                                     m_navTrackType;
    int                                                     m_navFormatType;
    
    int                                                     m_timeTblId;
};


#endif /* defined(__HelloCpp__ViewController__) */
