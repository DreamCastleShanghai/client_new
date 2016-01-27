
#ifndef __PointDetail__ViewController__
#define __PointDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class PointDetailViewController
: public CAViewController
{
    
public:

	PointDetailViewController(photoMsg& msg, int type);
    
	virtual ~PointDetailViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void initView();
    
    virtual void imageViewRequestFinished(CommonUrlImageView* imageView);
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void requestLikeSubmit();

	void onRequestLikeSubmitFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

    
    photoMsg*                               m_msg;
    int                                             m_type;
    DSize                                                  m_winSize;
    CAView*                                             m_view;
    CALabel*                                           m_likeNumLabel;
    CAImageView*                                 m_likeBtnImage;
    CAScrollView*                                   m_scrollView;
    CommonUrlImageView*                  m_urlImageVIew;
    bool                                                     m_canLike;
    CAActivityIndicatorView*                  p_pLoading;
};


#endif /* defined(__HelloCpp__ViewController__) */
