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
#include "CDData.h"

int getTimeval(struct cc_timeval *tp, void *tzp);

time_t getTimeSecond();

std::string timeToString(time_t second);

void quickSort(std::vector<sessionMsg>* msg,int s,int t);

#endif /* HttpConnect_hpp */
