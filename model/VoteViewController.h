
#ifndef __Sessions__ViewController__
#define __Sessions__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

typedef struct
{
	CALabel* lab;
	CAImageView* imgView;
} TabItemView;

class SessionsViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource,
	public CAScrollViewDelegate
{
    
public:

	SessionsViewController();
    
	virtual ~SessionsViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
private:

	DSize													m_winSize;

    std::vector<newsMsg>									m_msg;
    
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
    CAImageView*                                            m_timeNoticeImageView;
    CAImageView*                                            m_sessionNoticeImageView;
	CATableView*											m_msgTableView;
};


#endif /* defined(__HelloCpp__ViewController__) */
