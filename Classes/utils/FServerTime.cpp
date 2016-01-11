//
//  HttpConnect.cpp
//  SapSession
//
//  Created by csh on 15/12/10.
//
//

#include "FServerTime.h"
#include "FDataManager.h"

int gettimeofdayCrossApp(struct cc_timeval *tp, void *tzp)
{
    if (tp)
    {
        time_t sT = FDataManager::getInstance()->getDiffServerTime();
        gettimeofday((struct timeval *)tp,  0);
        tp->tv_sec = sT + tp->tv_sec;
    }
    return 0;
}

std::string timeToString(time_t second)
{
    tm* time = localtime(&second);
    
    return crossapp_format_string("%02d:%02d", time->tm_hour, time->tm_min);
}