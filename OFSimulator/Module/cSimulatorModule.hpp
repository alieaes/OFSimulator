#pragma once

#ifndef __HDR_OF_SIMULATOR_MODULE__
#define __HDR_OF_SIMULATOR_MODULE__

#include <ExDataStruct.hpp>

#include "OFSimulator.h"
#include "Module/EXModuleBase.hpp"

#include "UI/QMainWorld.hpp"
#include "UI/QCharacter.hpp"
#include "UI/QLoadingPage.hpp"
#include "UI/QMakeProfile.hpp"
#include "UI/QTimeController.hpp"

struct stDualInfo
{
    QString     sUUID;
    QString     sTargetUUID;
};

typedef std::shared_ptr< std::thread > spThread;
class cSimulatorModule : public QObject, public Ext::Module::cModuleBase
{
    Q_OBJECT

public:
    cSimulatorModule( QObject* parent );
    ~cSimulatorModule();

    bool                                     NotifyModule( const XString& sNotifyJobs ) override;

protected:
    bool                                     moduleInit() override;
    bool                                     moduleStart() override;
    bool                                     moduleStop() override;
    bool                                     moduleFinal() override;

private:
    void                                     workerTime();
    void                                     workerCharacter();
    void                                     workerDual();

private:
    QVector< spThread >                      _thCharacterPool;
    std::thread                              _thTime;
    Ext::Queue::eventQueue< QString >        _queue;

};


#endif