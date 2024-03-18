#include "stdafx.h"
#include "OFSimulator.h"

#include "Module/EXModuleManager.hpp"

#include "Module/cCharacterModule.hpp"

#include "UI/QMakeProfile.hpp"
//#include "UI/QLoadingPage.hpp"

#include "def/OFCharacter.hpp"

#include "def/OFDefines.hpp"

using namespace Ext;

OFSimulator::OFSimulator( QWidget* parent )
    : QMainWindow( parent )
{
    ui.setupUi( this );
    ui.ControlCentor->setCurrentIndex( 0 );

    auto tdModuleManager = Module::tdStModuleManager::GetInstance();
    tdModuleManager->RegisterModule( L"CHARACTER", L"CHARACTER", new cCharacterModule( this ) );

    auto spCharacterModule = Module::GetModule< cCharacterModule >( L"CHARACTER" );

    QObject::connect( ui.lstCharacter, &QListWidget::itemDoubleClicked, this, &OFSimulator::CharacterViewDoubleClicked );
}

OFSimulator::~OFSimulator()
{}

void OFSimulator::WorldStart()
{
    if( _World == NULLPTR )
        _World = new QMainWorld( this, ui );

    _World->Init();
}

void OFSimulator::CharacterStart()
{
    if( _Character == NULLPTR )
        _Character = new QCharacter( this, ui );

    auto spCharacterModule = Module::GetModule< cCharacterModule >( L"CHARACTER" );
    spCharacterModule->MakeCharacter();
}

void OFSimulator::on_btnNewGame_clicked()
{
    if( _QProfile == NULLPTR )
        _QProfile = new QMakeProfile( this, ui );

    _QProfile->Init();
}

void OFSimulator::on_btnContinue_clicked()
{
}

void OFSimulator::on_btnExit_clicked()
{
    exit( -1 );
}

void OFSimulator::CharacterViewDoubleClicked( const QListWidgetItem* item )
{
    QString sUUID = item->data( OF_CHARACTER_DATA_UUID ).toString();

    if( sUUID == NULLPTR )
        return;

    auto spCharacterModule = Module::GetModule< cCharacterModule >( L"CHARACTER" );
    auto info = spCharacterModule->GetCharacter( sUUID );

    ui.wdgDetail->setCurrentIndex( 1 );

    ui.lblCName->setText( info->stInfo.sFirstName + " " + info->stInfo.sSecondName );
    ui.lblCNickName->setText( info->stInfo.sAlias );
    ui.lblCGender->setText( "" );
    EXT_ENUM_GET( eOFGender, info->stInfo.eGender );
}

void OFSimulator::CharacterViewClear()
{
    ui.lblCName->clear();
    ui.lblCNickName->clear();
    ui.lblCGender->clear();
    ui.lblCAge->clear();
    ui.lblCMoney->clear();
    ui.lblCHP->clear();
    ui.lblCSP->clear();

    ui.lblCDamage->clear();
    ui.lblCDefence->clear();
    ui.lblCOFPower->clear();
    ui.lblCOFPowerDetail->clear();

    ui.lblCSTR->clear();
    ui.lblCDEX->clear();
    ui.lblCAGI->clear();
    ui.lblCMOV->clear();
    ui.lblCINT->clear();
    ui.lblCVIT->clear();
    ui.lblCLUK->clear();
    ui.lblCWIZ->clear();
    ui.lblCWILL->clear();
    ui.lblCCON->clear();
    ui.lblCCHA->clear();
    ui.lblCSIGHT->clear();

    ui.lblCBlade->clear();
    ui.lblCSword->clear();
    ui.lblCSpear->clear();
    ui.lblCPole->clear();
    ui.lblCPalm->clear();
    ui.lblCFinger->clear();
    ui.lblCFist->clear();
    ui.lblCProjectile->clear();

    ui.lblCFire->clear();
    ui.lblCWater->clear();
    ui.lblCIce->clear();
    ui.lblCElec->clear();
    ui.lblCPoison->clear();
    ui.lblCEarth->clear();
    ui.lblCWind->clear();
}
