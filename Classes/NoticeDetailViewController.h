
#ifndef __NoticeDetail__ViewController__
#define __NoticeDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class NoticeDetailViewController
:   public CAViewController
{
    
public:

    NoticeDetailViewController(std::string &title, std::string &detail);
    
	virtual ~NoticeDetailViewController();
    
    void buttonCallBack(CAControl* btn, DPoint point);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

private:

    DSize m_winSize;
    std::string m_detail;
    std::string m_title;
};


#endif /* defined(__HelloCpp__ViewController__) */
