#pragma once

#ifndef __HDR_OF_CHARACTER_MODULE__
#define __HDR_OF_CHARACTER_MODULE__

#include "Module/EXModuleBase.hpp"

#include "def/OFCharacter.hpp"

class cCharacterModule : public QObject, public Ext::Module::cModuleBase
{
    Q_OBJECT

public:
    cCharacterModule( QObject* parent );
    ~cCharacterModule();

    bool                                     NotifyModule( const XString& sNotifyJobs ) override;

    void                                     SetName( const XString& sName );

    bool                                     Save();
    bool                                     Load();

protected:
    bool                                     moduleInit() override;
    bool                                     moduleStart() override;
    bool                                     moduleStop() override;
    bool                                     moduleFinal() override;

private:
    stCharacterInfo                          _info;
};


#endif