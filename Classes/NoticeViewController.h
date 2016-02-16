
#ifndef __Notice__ViewController__
#define __Notice__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class NoticeViewController
:   public CAViewController,
    public CATableViewDelegate,
    public CATableViewDataSource
{
    
public:

	NoticeViewController();
    
	virtual ~NoticeViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void viewDidAppear();

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
    std::vector<LocalNoticeMsg>         m_msgList;
	std::vector<sessionMsg>*								m_msg;
	CATableView*											m_msgTableView;

    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
