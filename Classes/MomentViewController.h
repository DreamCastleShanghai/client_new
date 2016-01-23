
#ifndef __Moment__ViewController__
#define __Moment__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class MomentViewController
:   public CAViewController,
    public CATableViewDelegate,
	public CATableViewDataSource,
	public CACollectionViewDelegate,
	public CACollectionViewDataSource,
	public CAScrollViewDelegate
{
    
public:

	MomentViewController();
    
	virtual ~MomentViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg(int type);

	void onRequestAllFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
	void onRequestMyFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int numberOfSections(CATableView *table);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
	virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);

	virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const DSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
	virtual unsigned int numberOfSections(CACollectionView *collectionView);
	virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
	virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
	virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
private:
	enum
	{
		Type_all = 0,
		Type_me,
	};

	DSize													m_winSize;
	int														m_segType;

	std::vector<photoMsg>                                   m_allMsg;
	std::vector<photoMsg>                                   m_myMsg;
	int														m_currentAllNum;
	int														m_currentMyNum;
    
	CAScale9ImageView*										m_browView;
	CAView*													m_segView[2];
    CATableView*											m_msgTableView;
	CACollectionView*										m_myCollectionView;
    CAView*													p_alertView;
    CAActivityIndicatorView*								p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
