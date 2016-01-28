//
//  ConstRes.cpp
//  SapSession
//
//  Created by bbknightzm on 16/1/25.
//
//

#include "ConstRes.h"

ConstRes::ConstRes()
{
    
}

ConstRes::~ConstRes()
{
    
}

// Common
const string& ConstRes::getResCommonWhiteBG()
{
    return "common/white_bg.png";
}

const string& ConstRes::getResCommonGreyBG()
{
    return "common/gray_bg.png";
}

const string& ConstRes::getResCommonBlueBG()
{
    return "common/sky_bg.png";
}


// Login
const std::string& ConstRes::getResLoginBG()
{
    return "login/login_bg.jpg";
}

const string& ConstRes::getResLoginUserBG()
{
    return "login/login_name.png";
}

const string& ConstRes::getResLoginPwdBG()
{
    return "login/login_passwd.png";
}

