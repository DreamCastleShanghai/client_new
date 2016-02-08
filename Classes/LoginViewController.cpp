
#include "LoginViewController.h"
#include "CommonHttpManager.h"
#include "utils/HttpConnect.h"
#include "RootWindow.h"
#include "ConstData/ConstFunc.h"
#include "FUserManager.h"

LoginViewController::LoginViewController()
: m_pAccount(NULL)
, m_pPassword(NULL)
{

}

LoginViewController::~LoginViewController()
{
//    CC_SAFE_DELETE(m_pAccount);
//    CC_SAFE_DELETE(m_pPassword);
}

void LoginViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
//	DSize winSize = this->getView()->getBounds().size;
    m_winSize = this->getView()->getBounds().size;
    int iw = 0, ih = 0;
    int offY = m_winSize.height / 2;
    int offW = _px(240);
    
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("login/login_bg.jpg"));
    if (imageView) {
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        imageView->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height));
        this->getView()->addSubview(imageView);
    }
	
    CAImageView* logoView = CAImageView::createWithCenter(DRect(m_winSize.width / 2, _px(200), _px(512), _px(151)));
    if (logoView) {
        logoView->setImage(CAImage::create("login/login_logo.png"));
        logoView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        this->getView()->addSubview(logoView);
    }
    
    
    CAView* loginView = CAView::createWithCenter(DRect(m_winSize.width / 2, offY, m_winSize.width - _px(80), offW));
    loginView->setColor(CAColor_white);
    
    iw = _px(50);
    ih = _px(50);
	imageView = CAImageView::createWithImage(CAImage::create("login/login_name.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	imageView->setFrame(DRect(_px(10), _px(10), iw, ih));
	loginView->addSubview(imageView);

	imageView = CAImageView::createWithImage(CAImage::create("login/login_passwd.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	imageView->setFrame(DRect(_px(10), _px(90), iw, ih));
	loginView->addSubview(imageView);

	m_pAccount = CATextField::createWithFrame(DRect(_px(80), _px(0), m_winSize.width - _px(170), _px(80)));
	m_pAccount->setTextColor(CAColor_black);
	m_pAccount->setPlaceHolder("Username");
    m_pAccount->setKeyboardType(KEY_BOARD_TYPE_ALPHABET);
    m_pAccount->setFontName(SAP_FONT_ARIAL);
    m_pAccount->setKeyboardReturnType(KEY_BOARD_RETURN_DONE);
    m_pAccount->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("common/white_bg.png")));
	loginView->addSubview(m_pAccount);

	m_pPassword = CATextField::createWithFrame(DRect(_px(80), _px(80), m_winSize.width - _px(170), _px(80)));
    m_pPassword->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("common/white_bg.png")));
	m_pPassword->setPlaceHolder("Password");
    m_pPassword->setFontName(SAP_FONT_ARIAL);
	m_pPassword->setInputType(KEY_BOARD_INPUT_PASSWORD);
    m_pPassword->setKeyboardReturnType(KEY_BOARD_RETURN_DONE);
    m_pPassword->setTextColor(CAColor_black);
	loginView->addSubview(m_pPassword);

    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect(_px(0), _px(80), m_winSize.width - _px(80), _px(1)));
    sView->setColor(ccc4(255, 255, 255, 128));
    loginView->addSubview(sView);
    
	CAButton* button = CAButton::createWithFrame(DRect(_px(0), _px(160), m_winSize.width - _px(80), _px(80)), CAButtonTypeCustom);
	button->setTitleForState(CAControlStateAll, "Log In");
	button->setTitleFontName(SAP_FONT_ARIAL);
	button->setTitleColorForState(CAControlStateAll, CAColor_white);
	button->setBackGroundViewForState(CAControlStateAll, CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png")));
	button->setTag(100);
	button->addTarget(this, CAControl_selector(LoginViewController::btnCallBack), CAControlEventTouchUpInSide);
	loginView->addSubview(button);
    
	this->getView()->addSubview(loginView);

    offY = m_winSize.height / 2 + _px(200);
    offW = _px(80);
    CALabel* label = CALabel::createWithFrame(DRect(m_winSize.width - _px(500), offY, m_winSize.width, offW));
    label->setText("Forget Registration Code?");
    label->setColor(CAColor_white);
    label->setFontName(SAP_FONT_ARIAL);
    label->setFontSize(_px(24));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setUnderLine(true);
    this->getView()->addSubview(label);
    
	button = CAButton::createWithFrame(DRect(m_winSize.width - _px(500), offY, m_winSize.width, offW), CAButtonTypeCustom);
	button->setTag(200);
	button->addTarget(this, CAControl_selector(LoginViewController::btnCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(button);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void LoginViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void LoginViewController::btnCallBack(CAControl* btn, DPoint point)
{
	//point = btn->convertToWorldSpace(point);
	//CAButton* button = (CAButton*)btn;

    if (btn->getTag() == 100)
    {
        string accout = m_pAccount->getText();
        string passwd = m_pPassword->getText();
        ConstFunc::trim(accout);
        ConstFunc::trim(passwd);
        if (accout.length() == 0 || passwd.length() == 0) {
            CAAlertView *alertView = CAAlertView::createWithText("Waining !", "Account or password cannot be null !", "OK", NULL);
            alertView->show();
            return;
        }
        
        std::map<std::string,std::string> key_value;
        key_value["tag"] = loginTag;
        key_value["usr"]= accout;
        key_value["pwd"]= passwd;
        //key_value["sign_method"]="md5";
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this,
                                                   CommonHttpJson_selector(LoginViewController::onRequestLoginFinished));
    }
    else if(btn->getTag() == 200)
    {
        CAAlertView *alertView = CAAlertView::createWithText("Nothing !", "Process later !", "OK", NULL);
        alertView->show();
    }
	
}

void LoginViewController::onRequestLoginFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        CSJson::FastWriter writer;
        string tempjson = writer.write(json);
        CCLog("receive json == %s",tempjson.c_str());
        
        if(RootWindow::getInstance()->getRootViewController() == this)
        {
            if(json["result"]["r"].asString() == "1")
            {
                FDataManager::getInstance()->setUserId(json["result"]["UserId"].asInt());
                RootWindow::getInstance()->setRootViewController(RootWindow::getInstance()->getRootNavigationController());
                
                FUser user;
                user.uid = json["result"]["UserId"].asInt();
                user.loginname = "Alex";//m_pAccount->getText();
                FUserManager::sharedFUserManager()->userLogin(user);
            }
            else
            {
                CAAlertView *alertView = CAAlertView::createWithText("Warning !", "Account or password wrong !", "OK", NULL);
                alertView->show();
            }
        }
    }
    else
    {
        CAAlertView *alertView = CAAlertView::createWithText("Warning !", "Network error !", "OK", NULL);
        alertView->show();
    }

}