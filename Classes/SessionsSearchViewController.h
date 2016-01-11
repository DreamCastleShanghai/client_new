
#ifndef __SessionsSearch__ViewController__
#define __SessionsSearch__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SessionsSearchViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource,
	public CAScrollViewDelegate
{
    
public:

	SessionsSearchViewController();
    
	virtual ~SessionsSearchViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();
	void requestApendMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
private:

	DSize													m_winSize;

	CATextField*											m_searchTextField;
	CAView*													m_pageViewCell;
	CAView*													m_alertView;
	CAActivityIndicatorView*								m_pLoading;
	std::vector<newsMsg>									m_msg;
	CATableView*											m_msgTableView;
	int														m_section;
	
};


#endif /* defined(__HelloCpp__ViewController__) */
