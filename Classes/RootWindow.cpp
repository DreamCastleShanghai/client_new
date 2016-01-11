

#include "RootWindow.h"
#include "LoginViewController.h"
#include "MainViewController.h"
#include "SessionsViewController.h"
#include "MyStatusViewController.h"

static RootWindow* _window = NULL;

RootWindow* RootWindow::getInstance()
{
    if (_window == NULL)
    {
        _window = new RootWindow();
        _window->init();
        _window->autorelease();
    }
    return _window;
}

RootWindow::RootWindow()
: m_pRootNavigationController(NULL),
m_timeNotice(false),
m_sessionNotice(false),
m_diffServerTime(0)
{
    CAApplication::getApplication()->getKeypadDispatcher()->addDelegate(this);
}

RootWindow::~RootWindow()
{
	CAApplication::getApplication()->getKeypadDispatcher()->removeDelegate(this);
}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    CAApplication::getApplication()->setNotificationView(CAView::createWithFrame(this->getBounds(), CAColor_green));

    CAVector<CAViewController*> controllerVec;
	MainViewController* mainController = new MainViewController();
    mainController->setTabBarItem(CATabBarItem::create(unicode_to_utf8(mainBar[0]), CAImage::create("main/bar_0.png"),CAImage::create("main/bar_pre_0.png")));
	mainController->init();
	//mainController->autorelease();
    controllerVec.pushBack(mainController);
    
    SessionsViewController* sessionsController = new SessionsViewController();
    sessionsController->init();
    sessionsController->autorelease();
    sessionsController->setTabBarItem(CATabBarItem::create(unicode_to_utf8(mainBar[1]), CAImage::create("main/bar_1.png"),CAImage::create("main/bar_pre_1.png")));
    controllerVec.pushBack(sessionsController);
    
    MyStatusViewController* myController = new MyStatusViewController();
    myController->setTabBarItem(CATabBarItem::create(unicode_to_utf8(mainBar[2]), CAImage::create("main/bar_2.png"),CAImage::create("main/bar_pre_2.png")));
    myController->init();
    myController->autorelease();
    controllerVec.pushBack(myController);
    
    
    CATabBarController* tabBarController = new CATabBarController();
    tabBarController->initWithViewControllers(controllerVec);
    tabBarController->setTabBarSelectedBackGroundColor(CAColor_clear);
    //tabBarController->setTabBarBackGroundImage(CAImage::create("common/sky_bg.png"));
    CANavigationBarItem* temp_nav = CANavigationBarItem::create("");
    temp_nav->setNagigationBarHidden(true);
    tabBarController->setNavigationBarItem(temp_nav);
    tabBarController->setTabBarTitleColorForSelected(CAColor_white);
    tabBarController->autorelease();

	m_pRootNavigationController = new CANavigationController();
	m_pRootNavigationController->initWithRootViewController(tabBarController);
	m_pRootNavigationController->setNavigationBarBackGroundImage(CAImage::create("common/sky_bg.png"));
	m_pRootNavigationController->setNavigationBarHidden(true, false);
    m_pRootNavigationController->setScrollEnabled(false);

	if (0)
	{
		LoginViewController* _viewController = new LoginViewController();
		_viewController->init();
		this->setRootViewController(_viewController);
		_viewController->release();
	}
	else
	{
		this->setRootViewController(m_pRootNavigationController);
	}

	CAApplication::getApplication()->setNotificationView(NULL);
    
    return true;
}

void RootWindow::keyBackClicked()
{
	CC_RETURN_IF(CAAlertView::hideWithDisplayed());

	if (this->getModalViewController())
	{
		this->dismissModalViewController(true);
	}
	else if (this->getRootNavigationController()->getViewControllerCount() > 1)
	{
		this->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else{
		CAApplication::getApplication()->end();
	}
}

void RootWindow::visit()
{
	CAView::visit();
}

void RootWindow::didReceiveRemoteNotification()
{
    
}

void RootWindow::didReceiveLocalNotification()
{
    
}