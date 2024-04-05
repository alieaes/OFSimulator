#include "stdafx.h"

#include "Module/cSimulatorModule.hpp"
#include "Module/EXModuleManager.hpp"

#include "cCharacterModule.hpp"
#include "cUIManagerModule.hpp"
#include "Base/ExtConfig.hpp"
#include "def/OFDefines.hpp"

cSimulatorModule::cSimulatorModule( QObject* parent )
    : QObject( parent )
{
    _thTime = std::thread( [this]
    {
        workerTime();
    } );

    // ThreadPool 관리 방안 필요함
    for( int idx = 0; idx < 10; idx++ )
    {
        _thCharacterPool.push_back( std::make_shared<std::thread>( std::thread( [this] { workerCharacter(); } ) ) );
    }
}

cSimulatorModule::~cSimulatorModule()
{
}

bool cSimulatorModule::NotifyModule( const XString& sNotifyJobs )
{
    return true;
}

bool cSimulatorModule::moduleInit()
{
    return true;
}

bool cSimulatorModule::moduleStart()
{
    return true;
}

bool cSimulatorModule::moduleStop()
{
    return true;
}

bool cSimulatorModule::moduleFinal()
{
    return true;
}

void cSimulatorModule::workerTime()
{
    auto spCharacter = Ext::Module::GetModule< cCharacterModule >( OF_MODULE_CHARACTER );
    auto spUIManager = Ext::Module::GetModule< cUIManagerModule >( OF_MODULE_UI_MANAGER );

    while( IsStop() == false )
    {
        cWorldDateTime* worldTime = spUIManager->GetMainWorld()->GetWorldTime();
        worldTime->SpendMinute();

        auto setUUID = spCharacter->GetAllUUID();

        for( const QString& sUUID : setUUID )
            _queue.EnQueue( sUUID );

        while( _queue.Size() > 0 )
            Sleep( 10 );

        Sleep( 2000 );
    }
}

void cSimulatorModule::workerCharacter()
{
    auto spCharacter = Ext::Module::GetModule< cCharacterModule >( OF_MODULE_CHARACTER );

    while( IsStop() == false )
    {
        QString sUUID = _queue.DeQueue();

        stOFCharacter* info = spCharacter->GetCharacter( sUUID );

        if( info == NULLPTR )
            continue;

        unsigned int uSpeed = info->stInfo.uMOV;

        Ext::Util::cRandom< unsigned int > rand( 0, uSpeed );

        int nX = rand.Generate();

        info->gpItem->moveBy( nX, uSpeed - nX );
    }
}

void cSimulatorModule::workerDual()
{
}
