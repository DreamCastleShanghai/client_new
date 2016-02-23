
#include "LoginViewController.h"
#include "CommonHttpManager.h"
#include "utils/HttpConnect.h"
#include "RootWindow.h"
#include "ConstData/ConstFunc.h"
#include "FUserManager.h"
#include "FNoticeManager.h"

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
    int offW = (240);
    
    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("login/login_bg.jpg"));
    if (imageView) {
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        imageView->setFrame(DRect(0, 0, m_winSize.width, m_winSize.height));
        this->getView()->addSubview(imageView);
    }
	
    CAImageView* logoView = CAImageView::createWithCenter(DRect(m_winSize.width / 2, (200), (512), (151)));
    if (logoView) {
        logoView->setImage(CAImage::create("login/login_logo.png"));
        logoView->setImageViewScaleType(CAImageViewScaleTypeFitImageXY);
        this->getView()->addSubview(logoView);
    }
    
    
    CAView* loginView = CAView::createWithCenter(DRect(m_winSize.width / 2, offY, m_winSize.width - (80), offW));
    loginView->setColor(CAColor_white);
    
    iw = (50);
    ih = (50);
	imageView = CAImageView::createWithImage(CAImage::create("login/login_name.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	imageView->setFrame(DRect((10), (10), iw, ih));
	loginView->addSubview(imageView);

	imageView = CAImageView::createWithImage(CAImage::create("login/login_passwd.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	imageView->setFrame(DRect((10), (90), iw, ih));
	loginView->addSubview(imageView);

	m_pAccount = CATextField::createWithFrame(DRect((80), (0), m_winSize.width - (170), (80)));
	m_pAccount->setTextColor(CAColor_black);
    /*
	m_pAccount->setPlaceHolder("Username");
    m_pAccount->setKeyboardType(KEY_BOARD_TYPE_ALPHABET);
    m_pAccount->setFontName(SAP_FONT_ARIAL);
    m_pAccount->setKeyboardReturnType(KEY_BOARD_RETURN_DONE);
    m_pAccount->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("common/white_bg.png")));
	loginView->addSubview(m_pAccount);

	m_pPassword = CATextField::createWithFrame(DRect((80), (80), m_winSize.width - (170), (80)));
    m_pPassword->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("common/white_bg.png")));
	m_pPassword->setPlaceHolder("Password");
    m_pPassword->setFontName(SAP_FONT_ARIAL);
	m_pPassword->setInputType(KEY_BOARD_INPUT_PASSWORD);
    m_pPassword->setKeyboardReturnType(KEY_BOARD_RETURN_DONE);
     */
	m_pAccount->setPlaceHolderText("E-mail");
    //m_pAccount->setKeyboardType(CATextField::KeyboardTypeNumbersAndPunctuation);
    m_pAccount->setReturnType(CATextField::Next);
    m_pAccount->setBackgroundImage(CAImage::create("common/white_bg.png"));
	loginView->addSubview(m_pAccount);

	m_pPassword = CATextField::createWithFrame(DRect((80), (80), m_winSize.width - (170), (80)));
    //m_pPassword->setKeyboardType(CATextField::KeyboardTypeNumbersAndPunctuation);
    m_pPassword->setReturnType(CATextField::Next);
    m_pPassword->setBackgroundImage(CAImage::create("common/white_bg.png"));
    m_pPassword->setTextColor(CAColor_black);
    m_pPassword->setSecureTextEntry(true);
    m_pPassword->setPlaceHolderText("Confirmation number");
	loginView->addSubview(m_pPassword);

    CAScale9ImageView* sView = CAScale9ImageView::createWithImage(CAImage::create("common/gray_bg.png"));
    sView->setFrame(DRect((0), (80), m_winSize.width - (80), (1)));
    sView->setColor(ccc4(255, 255, 255, 128));
    loginView->addSubview(sView);
    
	CAButton* button = CAButton::createWithFrame(DRect((0), (160), m_winSize.width - (80), (80)), CAButtonTypeCustom);
	button->setTitleForState(CAControlStateAll, "Log In");
	button->setTitleFontName(SAP_FONT_ARIAL);
	button->setTitleColorForState(CAControlStateAll, CAColor_white);
	button->setBackgroundViewForState(CAControlStateAll, CAScale9ImageView::createWithImage(CAImage::create("common/sky_bg.png")));
	button->setTag(100);
	button->addTarget(this, CAControl_selector(LoginViewController::btnCallBack), CAControlEventTouchUpInSide);
	loginView->addSubview(button);
    
	this->getView()->addSubview(loginView);

    offY = m_winSize.height / 2 + (200);
    offW = (80);
    CALabel* label = CALabel::createWithFrame(DRect(m_winSize.width - (500), offY, m_winSize.width, offW));
    label->setText("Forgot confirmation number?");
    label->setColor(CAColor_white);
    label->setFontName(SAP_FONT_ARIAL);
    label->setFontSize((24));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setUnderLine(true);
    this->getView()->addSubview(label);
    
	button = CAButton::createWithFrame(DRect(m_winSize.width - (500), offY, m_winSize.width, offW), CAButtonTypeCustom);
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
        string account = m_pAccount->getText();
        string passwd = m_pPassword->getText();
        ConstFunc::trim(account);
        ConstFunc::trim(passwd);
        if (account.length() == 0 || passwd.length() == 0) {
            CAAlertView *alertView = CAAlertView::createWithText("Warning !", "Email adress or confirmation number cannot be null.", "OK", NULL);
            alertView->show();
            return;
        }
        m_loginName = account;
        std::map<std::string,std::string> key_value;
        key_value["tag"] = loginTag;
        key_value["usr"]= account;
        key_value["pwd"]= passwd;
        //key_value["sign_method"]="md5";
        //key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this,
                                                   CommonHttpJson_selector(LoginViewController::onRequestLoginFinished));
    }
    else if(btn->getTag() == 200)
    {
        CAAlertView *alertView = CAAlertView::createWithText("Check your badge", "Your confirmation number is printed on your badge.", "OK", NULL);
        alertView->show();
    }
}

void LoginViewController::onRequestLoginFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
//        CSJson::FastWriter writer;
//        string tempjson = writer.write(json);
//        CCLog("receive json == %s",tempjson.c_str());
        
        if(RootWindow::getInstance()->getRootViewController() == this)
        {
            if(json["result"]["r"].asString() == "1")
            {
                FDataManager::getInstance()->setUserId(json["result"]["UserId"].asInt());
                RootWindow::getInstance()->setRootViewController(RootWindow::getInstance()->getRootNavigationController());
                
                FUser user;
                user.uid = json["result"]["UserId"].asInt();
                user.loginname = m_loginName;
                FUserManager::sharedFUserManager()->userLogin(user);
                
                FNoticeManager::sharedFNoticeManager()->sendNoticeToken((unsigned char*)FDataManager::getInstance()->getUserToken().c_str());
            }
            else
            {
                CAAlertView *alertView = CAAlertView::createWithText("Warning !", "Invalid email address or confirmation number.", "OK", NULL);
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