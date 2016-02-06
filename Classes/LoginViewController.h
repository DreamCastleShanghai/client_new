
#ifndef __Login__ViewController__
#define __Login__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "CommonHttpManager.h"

USING_NS_CC;

class LoginViewController: public CAViewController
{
    
public:

	LoginViewController();
    
	virtual ~LoginViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
	void btnCallBack(CAControl* btn, DPoint point);
    
    void onRequestLoginFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    
private:

    DSize           m_winSize;
	CATextField*    m_pAccount;
	CATextField*    m_pPassword;
};


#endif /* defined(__Login__ViewController__) */
