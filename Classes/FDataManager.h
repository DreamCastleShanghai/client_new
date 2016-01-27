#ifndef __FDataManager__
#define __FDataManager__

#include <iostream>
#include "CrossApp.h"
#include "CommonHttpManager.h"

//#define LOCALTEST

struct userInfo
{
    int         m_userId;
	std::string m_loginName;
    std::string m_userName;
	vector<int> m_demoVoteIdVec;
	vector<int> m_voiceVoteIdVec;
	bool		m_eggVoted;
    bool        m_greenAmb;
	std::string m_imageUrl;
    int         m_point;

    int         m_pointRank;
    int         m_lastPointRank;
    vector<int> m_hasPrizeId;
};

struct photoMsg
{
	int picId;
	int userId;
	int likeNum;
	bool liked;
	std::string m_imageUrl;
	std::string m_iconUrl;
	std::string name;
	std::string caterory;
	std::string comment;
};

struct surveyDetail
{
    std::string m_question1;
    std::string m_question2;
    std::vector<string> m_question_option1;
    std::vector<string> m_question_option2;
};

struct noticeMsg
{
	int noticeId;
	int noticeType;
	bool closed;
	std::string title;
	time_t startTime;
	time_t endTime;
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
	std::string m_tag;
	std::string m_imageUrl;

    int         m_titleId;
    std::string m_title;
};

struct sessionMsg
{
    int         m_sessionId;
    time_t      m_serverTime;
    std::string m_title;
    std::string m_track;
    std::string m_format;
    std::string m_location;   
	time_t      m_startTime;
	time_t      m_endTime;
	int         m_point;
	std::string m_imageUrl;
	bool		m_liked;
    int         m_likeNum;
    bool        m_stored;
	
    bool        m_done;
};

struct speakerMsg
{
	std::string name;
	std::string title;
	std::string iconUrl;
	std::string role;
};

struct sessionDetailMsg
{
	int         m_sessionId;
	std::string m_title;
	std::string m_track;
	std::string m_format;
	std::string m_location;
	time_t      m_startTime;
	time_t      m_endTime;
	int         m_point;
	std::string m_imageUrl;
	std::string m_detail;
	std::vector<speakerMsg> m_speaker;
};

struct demoJamMsg
{
    int         m_projectId;
    std::string m_teamName;
    std::string m_department;
    std::string m_teamDetail;

    time_t      m_playStartTime;
    time_t      m_playEndTime;
    std::string m_imageUrl;
};

struct voiceMsg
{
    int         m_projectId;
    std::string m_playerName;
    std::string m_projectName;
    std::string m_imageUrl;
    
    time_t      m_playStartTime;
    time_t      m_playEndTime;
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
