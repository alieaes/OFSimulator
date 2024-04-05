#pragma once

#ifndef __HDR_OF_UI_MANAGER_MODULE__
#define __HDR_OF_UI_MANAGER_MODULE__

#include "OFSimulator.h"
#include "Module/EXModuleBase.hpp"

#include "UI/QMainWorld.hpp"
#include "UI/QCharacter.hpp"
#include "UI/QLoadingPage.hpp"
#include "UI/QMakeProfile.hpp"
#include "UI/QTimeController.hpp"

class cUIManagerModule : public QObject, public Ext::Module::cModuleBase
{
    Q_OBJECT

public:
    cUIManagerModule( QObject* parent );
    ~cUIManagerModule();

    void                                     SetMainWindow( OFSimulator* of ) { _parent = of; }
    void                                     SetWorld( QMainWorld* world ) { _world = world; }
    void                                     SetLoadingPage( QLoadingPage* page ) { _loadingPage = page; }
    void                                     SetProfilePage( QMakeProfile* profile ) { _profile = profile; }
    void                                     SetCharacter( QCharacter* character ) { _character = character; }
    void                                     SetTimeController( QTimeController* tm ) { _tmController = tm; }

    OFSimulator*                             GetMainWindow() const { return _parent; }
    QMainWorld*                              GetMainWorld() const { return _world; }
    QLoadingPage*                            GetLoadingPage() const { return _loadingPage; }
    QMakeProfile*                            GetProfilePage() const { return _profile; }
    QCharacter*                              GetCharacter() const { return _character; }
    QTimeController*                         GetTimeController() const { return _tmController; }

    bool                                     NotifyModule( const XString& sNotifyJobs ) override;

protected:
    bool                                     moduleInit() override;
    bool                                     moduleStart() override;
    bool                                     moduleStop() override;
    bool                                     moduleFinal() override;

private:
    OFSimulator*                             _parent       = NULLPTR;

    QMainWorld*                              _world        = NULLPTR;

    QLoadingPage*                            _loadingPage  = NULLPTR;

    QMakeProfile*                            _profile      = NULLPTR;

    QCharacter*                              _character    = NULLPTR;

    QTimeController*                         _tmController = NULLPTR;
};


#endif