#include "stdafx.h"

#include "UI/QLoadingPage.hpp"

#include "Base/ExtConfig.hpp"

QLoadingPage::QLoadingPage( QWidget* parent, Ui::OFSimulatorClass _ui, eLoadingType type )
    : QWidget( parent )
{
    ui = _ui;
    _eLoadingType = type;

    _mvLoading = NULLPTR;
    Init();
}

QLoadingPage::~QLoadingPage()
{
}

void QLoadingPage::Init()
{
    _nPrevIndex = ui.ControlCentor->currentIndex();

    ui.ControlCentor->setCurrentIndex( 1 );

    if( _mvLoading != NULLPTR )
    {
        _mvLoading->stop();
        delete _mvLoading;
        _mvLoading = NULLPTR;
    }

    _mvLoading = new QMovie( ui.frLoading );

    ui.lblLoading->setMovie( _mvLoading );

    SetLoadingGIF( ":/OFSimulator/res/loading.gif" );

    _uCurrentPercent = 0;
}

void QLoadingPage::Start()
{
    //_thMainWorker = std::thread( [this] { mainWorker(); } );
    _mvLoading->start();
}

void QLoadingPage::Stop()
{
    if( _thMainWorker.joinable() == true )
        _thMainWorker.join();

    Init();
}

void QLoadingPage::SetLoadingGIF( const QString& sFileRes )
{
    _sLoadingGIF = sFileRes;

    if( _mvLoading != NULLPTR )
        _mvLoading->setFileName( sFileRes );
}

void QLoadingPage::SetMaximumPercent( unsigned uMax )
{
    _uMaximumPercent = uMax;
}

void QLoadingPage::mainWorker()
{
    while( _isStop == false )
    {
        if( _uCurrentPercent > _uMaximumPercent )
            break;

        Sleep( 10 );
    }
}

QString QLoadingPage::getTextNewGame()
{
    QStringList lst;

    lst << "게임을 생성합니다." << "세계를 설정 중입니다." << "인물을 설정 중입니다.";

    return lst[ 0 ];
}
