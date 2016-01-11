//
//  HttpConnect.hpp
//  SapSession
//
//  Created by csh on 15/12/10.
//
//

#ifndef __FServerTime__
#define __FServerTime__

#include <iostream>
#include "CrossApp.h"

int gettimeofdayCrossApp(struct cc_timeval *tp, void *tzp);
std::string timeToString(time_t second);

#endif /* HttpConnect_hpp */
