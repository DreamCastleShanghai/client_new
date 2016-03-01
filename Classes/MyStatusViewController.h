
#ifndef __MyStatus__ViewController__
#define __MyStatus__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

struct secVec
{
    int rowNum;
    int hour;
};

class MyStatusViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource,
	public CAScrollViewDelegate
{
    
public:

	MyStatusViewController();
    
	virtual ~MyStatusViewController();
    
    void refreshTable();
    
    void refreshUserInfo();

protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void viewDidAppear();
    
    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();
    
    void showLoading();

//	void requestMsg();
    
    //	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void requestRankMsg();
    
    void onRequestRankFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void requestScoreHistoryMsg();

    void onRequestScoreHistoryFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    void switchNavType();//int index);
    
    void requestUserMsg();
    
    void onRequestUserMsgFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section);
    
    unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    
    virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    
private:
    enum {
        MY_CALANDER_VIEW = 1,
        MY_INFO_VIEW = 2,
        MY_INFO_SCORE_HISTORY = 1,
        MY_INFO_RANK = 2,
    };

	DSize													m_winSize;

    std::vector<sessionMsg>*								m_msg;
    std::vector<sessionMsg*>								m_filterMsg;
    std::vector<userInfo>                                   m_rankMsg;
    std::vector<scoreHistory>                               m_shMsg;
    
    class FSegmentView*                                     m_navSegmentView;
    class FSegmentView*                                     m_pointSegmentView;
    
    CAButton*                                               m_searchButton;
    CAButton*                                               m_pointButton;
    CommonUrlImageView*                                     m_urlImageView;
    CAImageView*                                            m_greenAmbIcon;
    CALabel*                                                m_nameLabel;
    CALabel*                                                m_pointLabel[2];
    CALabel*                                                m_rankLabel[2];
    CAView*                                                 m_pointView;
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
	CATableView*											m_msgTableView;
    CAPullToRefreshView*                                    m_headPullView;
    CATableView*                                            m_myCalanderView;
    int                                                     m_navType;
    int                                                     m_pointType;
//    bool                                                    m_canSwitchSeg;
//    bool                                                    m_canSwitchPoint;
    int                                                     m_startTimeFlag;
    std::vector<secVec>                                     m_rowNumOfSection;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
