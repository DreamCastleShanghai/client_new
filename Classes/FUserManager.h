
#ifndef __FUserManager__
#define __FUserManager__

#include <iostream>
#include "CrossApp.h"

typedef enum {
    offline=0,
    online
}UserStatus;

class FUser
{
public:
    
    
    int uid;
    
    std::string loginname;
    
    std::string photo;
    
    int status;
    
};

class FUserManager
{
public:
    
    FUserManager();
    
    virtual ~FUserManager();
    
    static FUserManager* sharedFUserManager();
    
    void _delete(int uid);
    
    //登陆
    bool userLogin(const FUser &user);
    //退出
    bool userLogout();
    //当前登陆的用户
    FUser onLineUser();

protected:

    sqlite3_stmt *_sqlite_stmt_delete;

    //登陆
    sqlite3_stmt *_sqlite_stmt_login;
    //退出
    sqlite3_stmt *_sqlite_stmt_logout;
    //查找登录状态的用户
    sqlite3_stmt *_sqlite_stmt_select_online;
 
    
};

#endif /* defined(__FUserManager__) */
