
#ifndef __SessionsSearch__ViewController__
#define __SessionsSearch__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SessionsSearchViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource
{
    
public:

	SessionsSearchViewController(int type);
    
	virtual ~SessionsSearchViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void refreshTableByFormat(int track, int format);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
private:

	DSize													m_winSize;
    int                                                     m_type;
	CATextField*											m_searchTextField;

	CAView*													m_alertView;
	CAActivityIndicatorView*								m_pLoading;
	std::vector<sessionMsg>*								m_msg;
    std::vector<sessionMsg*>								m_msgFilter;
	std::vector<sessionMsg*>								m_msgSearchFilter;
	CATableView*											m_msgTableView;
	CAView*													m_filterView;
	CAView*													m_downView[2];
	std::vector<CAButton*>                                  m_trackButtonVec;
	std::vector<CAButton*>                                  m_formatButtonVec;


	int                                                     m_navTrackType;
	int                                                     m_navFormatType;
};


#endif /* defined(__HelloCpp__ViewController__) */
