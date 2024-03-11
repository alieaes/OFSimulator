#include "stdafx.h"

#include "Module/cCharacterModule.hpp"

#include "Base/ExtConfig.hpp"

cCharacterModule::cCharacterModule( QObject* parent )
{
}

cCharacterModule::~cCharacterModule()
{
}

bool cCharacterModule::NotifyModule( const XString& sNotifyJobs )
{
    return true;
}

void cCharacterModule::SetName( const XString& sName )
{
    _info.sCharacterName = sName;
}

bool cCharacterModule::Save()
{
    bool isSuccess = false;

    return isSuccess;
}

bool cCharacterModule::Load()
{
    bool isSuccess = false;

    return isSuccess;
}

bool cCharacterModule::moduleInit()
{
    return true;
}

bool cCharacterModule::moduleStart()
{
    return true;
}

bool cCharacterModule::moduleStop()
{
    return true;
}

bool cCharacterModule::moduleFinal()
{
    return true;
}
