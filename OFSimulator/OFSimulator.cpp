﻿#include "stdafx.h"
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

    auto spModule = Module::GetModule< cCharacterModule >( L"CHARACTER" );
}

OFSimulator::~OFSimulator()
{}

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