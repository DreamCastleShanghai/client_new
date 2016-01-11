//
//  HttpConnect.cpp
//  SapSession
//
//  Created by csh on 15/12/10.
//
//

#include "FServerTime.h"
#include "FDataManager.h"

int getTimeval(struct cc_timeval *tp, void *tzp)
{
    if (tp)
    {
        time_t sT = FDataManager::getInstance()->getDiffServerTime();
        gettimeofday((struct timeval *)tp,  0);
        tp->tv_sec = sT + tp->tv_sec;
    }
    return 0;
}

time_t getTimeSecond()
{
    timeval tp;
    time_t sT = FDataManager::getInstance()->getDiffServerTime();
    gettimeofday(&tp,  0);
    return sT + tp.tv_sec;
}

std::string timeToString(time_t second)
{
    tm* time = localtime(&second);
    
    return crossapp_format_string("%02d:%02d", time->tm_hour, time->tm_min);
}

void quickSort(std::vector<sessionMsg>* msg,int s,int t)
{
    int i=s,j=t;
    
    if(i < j)
    {
        sessionMsg temp = msg->at(i);
        while(i < j)
        {
            while(i < j && msg->at(j).m_startTime >= temp.m_startTime)
                j--;
            if (i < j)
                msg->at(i++) = msg->at(j);
            
            while(i < j && msg->at(i).m_startTime < temp.m_startTime)
                i++;
            if (i < j)
                msg->at(j--) = msg->at(i);
        }
        msg->at(i) = temp;
        quickSort(msg, s, i-1);
        quickSort(msg, i+1, t);
    }
}