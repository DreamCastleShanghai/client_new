
#ifndef __FNoticeManager__
#define __FNoticeManager__

#include <iostream>
#include "CrossApp.h"
#include "CommonHttpManager.h"

#define PRETIME (5 * 60)

typedef enum {
    notice_session=0,
    notice_news,
    notice_prize,
}NoticeType;

typedef struct
{
    int type;
    int sid;
    std::string title;
    std::string detail;
    bool readed;
    time_t startTime;
    time_t endTime;
    
} FNotice;


typedef void (CAObject::*SEL_FNoticeEvent)();
#define FNotice_selector(_SELECTOR) (SEL_FNoticeEvent)(&_SELECTOR)

class FNoticeManager : public CAObject
{
public:
    
    FNoticeManager();
    
    virtual ~FNoticeManager();
    
    static FNoticeManager* sharedFNoticeManager();
    
    void setTarget(CAObject* target, SEL_FNoticeEvent selector);
    
    bool addNotice(int sid, int type, std::string &title, std::string &detail, time_t start = 0, time_t end = 0, bool remote = false);
    bool deleteNotice(int sid);
    std::vector<FNotice>& getNotices();
    bool readNotice(int sid);

    void sendNoticeToken(unsigned char* token);
    
    void onSendNoticeToken(const HttpResponseStatus& status, const CSJson::Value& json);
    
    void playNoticeVoice();
    
    void refreshMessage();
    bool hasNewMessage() { return m_hasNewMessage; }
    
protected:
    
    sqlite3_stmt *_sqlite_stmt_delete;
    sqlite3_stmt *_sqlite_stmt_add;
    sqlite3_stmt *_sqlite_stmt_get;
    sqlite3_stmt *_sqlite_stmt_read;
    sqlite3_stmt *_sqlite_stmt_getunread;

    std::vector<FNotice> m_notices;
    
    CAObject*           m_pFNoticeTarget;
    SEL_FNoticeEvent    m_pFNoticeEvent;
    bool m_hasNewMessage;
};

#endif /* defined(__FNoticeManager__) */
