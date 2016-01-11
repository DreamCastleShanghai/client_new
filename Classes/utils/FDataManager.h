#ifndef __FDataManager__
#define __FDataManager__

#include <iostream>
#include "CrossApp.h"

class FDataManager
{
    
public:

	static FDataManager* getInstance();
    
	FDataManager();
    
	virtual ~FDataManager();

    int getUserId() { return m_userId; }
    void setUserId(int id) { m_userId = id; }

	bool isTimeNotice() { return m_timeNotice; }
	void setTimeNotice(bool notice) { m_timeNotice = notice; }

	bool isSessionNotice() { return m_sessionNotice; }
	void setSessionNotice(bool notcie) { m_sessionNotice = notcie; }

    void setDiffServerTime(time_t diff) { m_diffServerTime = diff;}
    time_t getDiffServerTime() { return m_diffServerTime; }
    
private:
    
    int                         m_userId;
	bool                        m_timeNotice;
	bool                        m_sessionNotice;
    time_t                      m_diffServerTime;
};


#endif /* defined(__FDataManager__) */
