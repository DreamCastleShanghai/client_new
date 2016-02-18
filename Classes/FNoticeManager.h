
#ifndef __FNoticeManager__
#define __FNoticeManager__

#include <iostream>
#include "CrossApp.h"
#include "CommonHttpManager.h"

#define PRETIME (5 * 60)

typedef enum {
    notice_session=0,
    notice_news,
}NoticeType;

typedef struct
{
    int type;
    int sid;
    std::string title;
    bool readed;
    time_t startTime;
    time_t endTime;
    
} FNotice;

class FNoticeManager : public CAObject
{
public:
    
    FNoticeManager();
    
    virtual ~FNoticeManager();
    
    static FNoticeManager* sharedFNoticeManager();
    
    bool addNotice(int sid, int type, std::string &title, time_t start = 0, time_t end = 0, bool remote = false);
    bool deleteNotice(int sid);
    std::vector<FNotice>& getNotices();
    bool readNotice(int sid);

    void sendNoticeToken(unsigned char* token);
    
    void onSendNoticeToken(const HttpResponseStatus& status, const CSJson::Value& json);
    
protected:
    
    sqlite3_stmt *_sqlite_stmt_delete;
    sqlite3_stmt *_sqlite_stmt_add;
    sqlite3_stmt *_sqlite_stmt_get;
    sqlite3_stmt *_sqlite_stmt_read;

    std::vector<FNotice> m_notices;
    
};

#endif /* defined(__FNoticeManager__) */
