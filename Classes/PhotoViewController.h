
#ifndef __Photo__ViewController__
#define __Photo__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class PhotoViewController :
	public CAViewController,
    public CAScrollViewDelegate,
    public CAMediaDelegate
{
    
public:

	PhotoViewController(int type);
    
	virtual ~PhotoViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

    virtual void getSelectedImage(CAImage *image);
    
    void requestPhotoSubmit(std::string fullPath);
    
    void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

	DSize													m_winSize;
    int                                                         m_type;
    
    CAView*                                             m_basicView;
    CAView*                                             m_photoView;
    CAActivityIndicatorView*								p_pLoading;

    CADrawView* getStencil(const DSize& size, int index);
    
    
    CAView* renderImage;
    CAView* renderImage_mb;
    CAClippingView* m_clvImage;
    CAClippingView* m_clv;
    int dle_ren_index;
};


#endif /* defined(__HelloCpp__ViewController__) */
