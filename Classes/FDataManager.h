#ifndef __FDataManager__
#define __FDataManager__

#include <iostream>
#include "CrossApp.h"
#include "CommonHttpManager.h"

struct userInfo
{
    int         m_userId;
    std::string m_userName;
    int         m_point;
    int         m_pointRank;
    int         m_lastPointRank;
    vector<int> m_hasPrizeId;
    int         m_voteProjectId;
    std::string m_imageUrl;
};

struct surveyDetail
{
    int m_surveyId;
    int m_itemId;
    std::string m_itemDetail;
    std::vector<string> m_itemSelection;
};

struct pointMsg
{
    int         m_userId;
    int         m_point;
    int         m_pointRank;
    vector<int> m_hasPrizeId;
    int         m_rankLimit;
    int         m_miLimit;
};

struct newsPage
{
    int         m_titleId;
    std::string m_title;
    std::string m_imageUrl;
};

struct sessionMsg
{
    int         m_sessionId;
    time_t      m_serverTime;
    std::string m_title;
    std::string m_detail;
    std::string m_lecturer;
    std::string m_lecturerEmail;
    std::string m_location;
    std::string m_track;
    std::string m_format;
    time_t      m_startTime;
    time_t      m_endTime;
    int         m_likeNum;
    std::string m_imageUrl;
    bool        m_stored;
    bool        m_done;
    int         m_point;
};

struct voteMsg
{
    int         m_projectId;
    int         m_projectType;
    std::string m_playerName;
    std::string m_playerTeam;
    time_t      m_playStartTime;
    time_t      m_playEndTime;
    std::string m_projectLoctaion;
    std::string m_projectDetail;
    std::string m_imageUrl;
};

struct newsImage
{
    std::string m_title;
    std::vector<std::string > m_imageUrl;
    std::vector<std::string > m_imageDesc;
};

struct rankMsg
{
    int         m_userId;
    std::string m_imageUrl;
};

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
    
    vector<sessionMsg>* getSessionMsgs() { return &m_sessionMsgs; }
    userInfo* getUserInfo() { return &m_userInfo; }
    void setUserInfo(userInfo& ui) { m_userInfo = ui; }
    
private:
    
    int                         m_userId;
	bool                        m_timeNotice;
	bool                        m_sessionNotice;
    time_t                      m_diffServerTime;
    vector<sessionMsg>          m_sessionMsgs;
    userInfo                    m_userInfo;
};


#endif /* defined(__FDataManager__) */
