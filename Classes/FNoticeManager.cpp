// create by caoshuai

#include "FNoticeManager.h"
#include "FServerTime.h"
#include "RootWindow.h"

static FNoticeManager* _noticeManager = NULL;

FNoticeManager* FNoticeManager::sharedFNoticeManager()
{
    if (_noticeManager == NULL)
    {
        _noticeManager = new FNoticeManager();
    }
    return _noticeManager;
}

FNoticeManager::FNoticeManager()
{
    
    int ret = 0;

    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS notice(id int auto_increment, sid int, title VARCHAR(256), readed int, start_time int64, end_time int64, type int);";
	sqlite3_stmt *stmt;
    int ok=sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLog("Error in CREATE Notice TABLE\n");

    const char *sql_delete = "DELETE FROM notice WHERE sid = ? ;";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_delete, -1, &_sqlite_stmt_delete, NULL);
    
    const char *sql_stmt_add = "INSERT INTO notice (sid, title, readed, start_time, end_time, type) VALUES (?,?,?,?,?,?);";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_add, -1, &_sqlite_stmt_add, NULL);
    
    const char *sql_stmt_read = "UPDATE notice SET readed = 1 WHERE sid = ?;";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_read, -1, &_sqlite_stmt_read, NULL);
    
    const char* sql_stmt_get =" SELECT sid, title, readed ,start_time, end_time, type FROM notice ORDER BY start_time DESC;";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_get, -1, &_sqlite_stmt_get, NULL);
}

FNoticeManager::~FNoticeManager()
{

    sqlite3_finalize(_sqlite_stmt_delete);
    sqlite3_finalize(_sqlite_stmt_add);
    sqlite3_finalize(_sqlite_stmt_read);
    sqlite3_finalize(_sqlite_stmt_get);
}

bool FNoticeManager::addNotice(int sid, int type, std::string &title, time_t start, time_t end)
{
    deleteNotice(sid);
    
    CADevice::sendLocalNotification("session", title.c_str(), start - getTimeSecond() - PRETIME, crossapp_format_string("%d", sid).c_str());

    int ok = sqlite3_bind_text(_sqlite_stmt_add, 1, crossapp_format_string("%d", sid).c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_add, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_add, 3, crossapp_format_string("%d", 0).c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_add, 4, crossapp_format_string("%ld", start).c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_add, 5, crossapp_format_string("%ld", end).c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_add, 6, crossapp_format_string("%d", type).c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_add);
    ok |= sqlite3_reset(_sqlite_stmt_add);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        CCLog("Error in FNoticeManager::addNotice");
        return false;
    }
    
    return true;
}

bool FNoticeManager::readNotice(int sid)
{
    CADevice::cancelLocalNotification(crossapp_format_string("%d", sid).c_str());
    
    int ok = sqlite3_bind_text(_sqlite_stmt_read, 1, crossapp_format_string("%d", sid).c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_step(_sqlite_stmt_read);
    ok |= sqlite3_reset(_sqlite_stmt_read);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
    {
        CCLog("Error in FNoticeManager::readNotice");
        return false;
    }
    return true;
}

std::vector<FNotice>& FNoticeManager::getNotices()
{
    int ok = SQLITE_OK;
    m_notices.clear();
    FNotice notice;
    while(sqlite3_step(_sqlite_stmt_get) == SQLITE_ROW)
    {
        notice.sid = sqlite3_column_int(_sqlite_stmt_get, 0);
        notice.title = crossapp_format_string("%s", sqlite3_column_text(_sqlite_stmt_get, 1));
        notice.readed = sqlite3_column_int(_sqlite_stmt_get, 2);
        notice.startTime = sqlite3_column_int64(_sqlite_stmt_get, 3);
        notice.endTime = sqlite3_column_int64(_sqlite_stmt_get, 4);
        notice.type = sqlite3_column_int(_sqlite_stmt_get, 5);
        m_notices.push_back(notice);
    }
    ok |= sqlite3_reset(_sqlite_stmt_get);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        CCLog("Error in FNoticeManager::getNotices");
    
    return m_notices;
}


bool FNoticeManager::deleteNotice(int sid)
{
    CADevice::cancelLocalNotification(crossapp_format_string("%d", sid).c_str());
    
    int ok = sqlite3_bind_text(_sqlite_stmt_delete, 1, crossapp_format_string("%d", sid).c_str(), -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_delete);
	
	ok |= sqlite3_reset(_sqlite_stmt_delete);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
    {
		CCLog("Error in FNoticeManager::_delete\n");
        return false;
    }
    
    return true;
}


