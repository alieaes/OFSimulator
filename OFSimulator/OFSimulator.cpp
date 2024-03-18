#include "stdafx.h"
#include "OFSimulator.h"

#include "Module/EXModuleManager.hpp"

#include "Module/cCharacterModule.hpp"

#include "UI/QMakeProfile.hpp"
//#include "UI/QLoadingPage.hpp"

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
}
