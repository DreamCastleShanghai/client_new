

#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CrossApp.h"
#include "CDData.h"

USING_NS_CC;

class RootWindow: public CAWindow, public CAKeypadDelegate
{
    
public:

	static RootWindow* getInstance();
    
	RootWindow();
    
	virtual ~RootWindow();
    
    virtual bool init();
    
	virtual void visit();

	CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
    
	virtual void keyBackClicked();

    int getUserId() { return m_userId; }
    void setUserId(int id) { m_userId = id; }

	bool isTimeNotice() { return m_timeNotice; }
	void setTimeNotice(bool notice) { m_timeNotice = notice; }

	bool isSessionNotice() { return m_sessionNotice; }
	void setSessionNotice(bool notcie) { m_sessionNotice = notcie; }
    
    void didReceiveRemoteNotification();
    
    void didReceiveLocalNotification();
    
    void setDiffServerTime(time_t diff) { m_diffServerTime = diff;}
    time_t getDiffServerTime() { return m_diffServerTime; }
    time_t getServerTime();
    
private:
    
    int                         m_userId;
	bool                        m_timeNotice;
	bool                        m_sessionNotice;
    time_t                      m_diffServerTime;
};


#endif /* defined(__HelloCpp__ViewController__) */
