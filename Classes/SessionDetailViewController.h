
#ifndef __SessionDetail__ViewController__
#define __SessionDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SessionDetailViewController :
	public CAViewController
{
    
public:

	SessionDetailViewController(newsMsg& msg);
    
	virtual ~SessionDetailViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void initMsgTableView();
    
	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void storeBtnCallBack(CAControl* btn, DPoint point);
    
    void onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

	DSize													m_winSize;

    newsMsg*                                                m_msg;
    bool                                                    m_canStore;
    bool                                                    m_isStore;
    CAImageView*                                            m_storeBtnImage;
};


#endif /* defined(__HelloCpp__ViewController__) */
