
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

	PhotoViewController();
    
	virtual ~PhotoViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    virtual void getSelectedImage(CAImage *image);
    
    
private:

	DSize													m_winSize;

    
    void scheduleFuck(float dt);
    CADrawView* getStencil(const DSize& size, int index);
    
    
    CAView* renderImage;
    CAView* renderImage_mb;
    CAClippingView* m_clvImage;
    CAClippingView* m_clv;
    CAButton* render_btn;
    int dle_ren_index;
};


#endif /* defined(__HelloCpp__ViewController__) */
