#pragma once

#ifndef __HDR_OF_CHARACTER_MODULE__
#define __HDR_OF_CHARACTER_MODULE__

#include "OFSimulator.h"
#include "Module/EXModuleBase.hpp"

#include "def/OFCharacter.hpp"

class cCharacterModule : public QObject, public Ext::Module::cModuleBase
{
    Q_OBJECT

public:
    cCharacterModule( QObject* parent );
    ~cCharacterModule();

    bool                                     NotifyModule( const XString& sNotifyJobs ) override;

    bool                                     Save();
    bool                                     Load();

    void                                     MakeCharacter( stOFCharacter info, int nX, int nY );
    void                                     MakeCharacter( int nX, int nY );
    void                                     MakeCharacter();

    void                                     SetCharacterCoord( const QString& sUUID, QPoint pCoord );
    void                                     SetCharacterColor( const QString& sUUID, const QString& sColor );

    stOFCharacter*                           GetCharacter( const QString& sUUID );
    OFSimulator*                             GetParent() { return _parent; }

    bool                                     IsExistCharacter( const QString& sUUID );

    void                                     RefreshHP( const QString& sUUID );
    void                                     RefreshSP( const QString& sUUID );

protected:
    bool                                     moduleInit() override;
    bool                                     moduleStart() override;
    bool                                     moduleStop() override;
    bool                                     moduleFinal() override;

private:
    XString                                  createCharacterName( const eOFGender& eGender, const XString& sFirstName = "" );

    XString                                  createFirstName( const eOFGender& eGender );
    XString                                  createSecondName( const eOFGender& eGender );

    void                                     makeCharacterConcept( stOFCharacter& info );

private:
    QReadWriteLock                           _lckCharacter;

    OFSimulator*                             _parent;

    QHash< QString, stOFCharacter >          _char;

};


#endif