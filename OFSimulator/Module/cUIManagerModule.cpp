#include "stdafx.h"

#include "Module/cUIManagerModule.hpp"

#include "Base/ExtConfig.hpp"

cUIManagerModule::cUIManagerModule( QObject* parent )
    : QObject( parent )
{
}

cUIManagerModule::~cUIManagerModule()
{
}

bool cUIManagerModule::NotifyModule( const XString& sNotifyJobs )
{
    return true;
}

bool cUIManagerModule::moduleInit()
{
    return true;
}

bool cUIManagerModule::moduleStart()
{
    return true;
}

bool cUIManagerModule::moduleStop()
{
    return true;
}

bool cUIManagerModule::moduleFinal()
{
    return true;
}
