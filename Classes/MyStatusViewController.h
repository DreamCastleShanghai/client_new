
#ifndef __MyStatus__ViewController__
#define __MyStatus__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class MyStatusViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource,
	public CAScrollViewDelegate
{
    
public:

	MyStatusViewController();
    
	virtual ~MyStatusViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();
    
    void requestRankMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

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
    
    CAButton*                                               m_searchButton;
    CAButton*                                               m_pointButton;
    CAView*                                                 m_segView[2];
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
	CATableView*											m_msgTableView;
    int                                                     m_navType;
    bool                                                    m_canSwitchSeg;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
