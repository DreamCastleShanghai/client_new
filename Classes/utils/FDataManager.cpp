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
:   m_timeNotice(false),
    m_sessionNotice(false),
    m_diffServerTime(0)
{

}

FDataManager::~FDataManager()
{

}