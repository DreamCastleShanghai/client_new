#include "FDataManager.h"

static FDataManager* _fDataManager = NULL;

FDataManager* FDataManager::getInstance()
{
    if (_fDataManager == NULL)
    {
        _fDataManager = new FDataManager();
    }
    return _fDataManager;
}

FDataManager::FDataManager()
:   m_timeNotice(false)
, m_sessionNotice(false)
, m_diffServerTime(0)
, m_userDirty(true)
, m_rankDirty(true)
, m_photoDirty(true)
{
    m_sessionMsgs.clear();
//    m_userId = 1;
//    memset((void*)&m_userInfo, 0, sizeof(userInfo));
//    m_userInfo.m_userId = 1;
//    m_sessionMsgs.clear();
}

FDataManager::~FDataManager()
{

}