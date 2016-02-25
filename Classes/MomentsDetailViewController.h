
#ifndef __MomentsDetail__ViewController__
#define __MomentsDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class MomentsDetailViewController
: public CAViewController,
 public CommonUrlImageViewDelegate
{
    
public:

	MomentsDetailViewController(photoMsg& msg, int type);
    
	virtual ~MomentsDetailViewController();
    
    void setParent(CAViewController* pt)    { m_parent = pt; }
    
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
    int                                     m_type;
    DSize                                   m_winSize;
    CAView*                                 m_view;
    CALabel*                                m_likeNumLabel;
    CAImageView*                            m_likeBtnImage;
    CAScrollView*                           m_scrollView;
    CommonUrlImageView*                     m_urlImageVIew;
//    bool                                    m_canLike;
    CAActivityIndicatorView*                p_pLoading;
    
    CAViewController*                       m_parent;
};


#endif /* defined(__HelloCpp__ViewController__) */
