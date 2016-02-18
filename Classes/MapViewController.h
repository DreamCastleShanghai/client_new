
#ifndef __Map__ViewController__
#define __Map__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"
#include "FSegmentView.h"

USING_NS_CC;

class MapViewController :
	public CAViewController
{
    
public:

	MapViewController();
    
	virtual ~MapViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
//    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

//	void showAlert();

	void requestInsideguideMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

    enum
    {
        ID_BACK = 0,
        ID_OUTSIDE,
        ID_INSIDE,
    };
	DSize													m_winSize;
    //std::vector<newsPage>                                   m_maps;
    CAScrollView*                                           m_bodyScrollView;
//    std::vector<sessionMsg>									m_msg;
    
//    CAView*													p_alertView;
    CAActivityIndicatorView*								m_pLoading;
    
//    FSegmentView*                                           m_segView;
//    CAButton*                                               m_leftBtn;
//    CAButton*                                               m_rightBtn;
    
//    CAScrollView*                                           m_pageOne;
//    CAScrollView*                                           m_pageTwo;
    
//    CALabel*                                                m_pageOneAddressLabel;
//    CommonUrlImageView*                                     m_pageOneImage;
    
//    CommonUrlImageView*                                     m_pageTwoImage;
    
};


#endif /* defined(__HelloCpp__ViewController__) */
