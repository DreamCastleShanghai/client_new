// create by caoshuai

#include "FUserManager.h"
#include "RootWindow.h"

static FUserManager* _userManager = NULL;

FUserManager* FUserManager::sharedFUserManager()
{
    if (_userManager == NULL)
    {
        _userManager = new FUserManager();
    }
    return _userManager;
}

FUserManager::FUserManager()
{
    
    int ret = 0;
    const char *sql_createtable = "CREATE TABLE IF NOT EXISTS user(id int auto_increment, uid int, loginname VARCHAR(256), photo VARCHAR(256) , status int);";
	sqlite3_stmt *stmt;
    int ok=sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_createtable, -1, &stmt, NULL);
	ok |= sqlite3_step(stmt);
	ok |= sqlite3_finalize(stmt);
	
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLog("Error in CREATE USER TABLE\n");
    
    //DELETE
    const char *sql_delete = "DELETE FROM user WHERE uid = ? ;";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_delete, -1, &_sqlite_stmt_delete, NULL);
    
    const char *sql_stmt_login = "INSERT INTO user (uid, loginname, photo,status) VALUES (?,?,?,?);";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_login, -1, &_sqlite_stmt_login, NULL);
    
    const char *sql_stmt_logout = "UPDATE user SET status = 0 WHERE uid = (SELECT uid FROM user WHERE status = 1 LIMIT 1);";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_logout, -1, &_sqlite_stmt_logout, NULL);
    
    const char* sql_stmt_select_online =" SELECT uid, loginname, photo ,status FROM user WHERE status = 1 LIMIT 1; ";
    ret |= sqlite3_prepare_v2(RootWindow::getInstance()->getSqlite3(), sql_stmt_select_online, -1, &_sqlite_stmt_select_online, NULL);
}

FUserManager::~FUserManager()
{

    sqlite3_finalize(_sqlite_stmt_delete);

    sqlite3_finalize(_sqlite_stmt_login);
    sqlite3_finalize(_sqlite_stmt_logout);
    sqlite3_finalize(_sqlite_stmt_select_online);
}


//登陆
bool FUserManager::userLogin(const FUser &user)
{
    
    _delete(user.uid);
    
    int ok = sqlite3_bind_text(_sqlite_stmt_login, 1, crossapp_format_string("%d", user.uid).c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_login, 2, user.loginname.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_login, 3, user.photo.c_str(), -1, SQLITE_TRANSIENT);
    ok |= sqlite3_bind_text(_sqlite_stmt_login, 4, crossapp_format_string("%d", 1).c_str(), -1, SQLITE_TRANSIENT);
    
    ok |= sqlite3_step(_sqlite_stmt_login);
    ok |= sqlite3_reset(_sqlite_stmt_login);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        CCLog("Error in FUserManager::userLogin");
        return false;
    }
    
    return true;
}

//退出
bool FUserManager::userLogout()
{
    int ok = sqlite3_step(_sqlite_stmt_logout);
    ok |= sqlite3_reset(_sqlite_stmt_logout);
    
    if( ok != SQLITE_OK && ok != SQLITE_DONE){
        CCLog("Error in FUserManager::userLogout");
        return false;
    }
    return true;
}
//当前登陆的用户
FUser FUserManager::onLineUser()
{
    int ok = SQLITE_OK;
    FUser user;
    if (sqlite3_step(_sqlite_stmt_select_online)== SQLITE_ROW){
        int uid = sqlite3_column_int(_sqlite_stmt_select_online, 0);
        const unsigned char *loginname=sqlite3_column_text(_sqlite_stmt_select_online, 1);
        const unsigned char *photo=sqlite3_column_text(_sqlite_stmt_select_online, 2);
        int status = sqlite3_column_int(_sqlite_stmt_select_online, 3);
        user.uid = uid;
        user.loginname = (const char*)loginname;
        user.photo = (const char*)photo;
        user.status = status;
    }
    ok |= sqlite3_reset(_sqlite_stmt_select_online);
    if( ok != SQLITE_OK && ok != SQLITE_DONE)
        CCLog("Error in FUserManager::onLineUser");
    
    return user;
}


void FUserManager::_delete(int uid)
{
    int ok = sqlite3_bind_text(_sqlite_stmt_login, 1, crossapp_format_string("%d", uid).c_str(), -1, SQLITE_TRANSIENT);
	
	ok |= sqlite3_step(_sqlite_stmt_delete);
	
	ok |= sqlite3_reset(_sqlite_stmt_delete);
    
	if( ok != SQLITE_OK && ok != SQLITE_DONE)
		CCLog("Error in FUserManager::_delete\n");
}


