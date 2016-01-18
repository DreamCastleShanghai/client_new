
#ifndef __Survey__ViewController__
#define __Survey__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SurveyViewController
: public CAViewController,
  public CAListViewDelegate,
  public CAListViewDataSource
{
    
public:

	SurveyViewController();
    
	virtual ~SurveyViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
	virtual void listViewDidSelectCellAtIndex(CAListView *listView, unsigned int index);
	virtual void listViewDidDeselectCellAtIndex(CAListView *listView, unsigned int index);
	virtual unsigned int numberOfIndex(CAListView *listView);
	virtual unsigned int listViewHeightForIndex(CAListView *listView, unsigned int index);
	virtual CAListViewCell* listViewCellAtIndex(CAListView *listView, const DSize& cellSize, unsigned int index);

private:

	DSize													m_winSize;
	std::vector<surveyDetail>								m_msg;

    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;

	CAListView*												m_listView;
};


#endif /* defined(__HelloCpp__ViewController__) */
