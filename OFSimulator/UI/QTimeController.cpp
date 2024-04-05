#include "stdafx.h"

#include "UI/QTimeController.hpp"

#include "OFSimulator.h"
#include "Base/ExtConfig.hpp"

using namespace Ext;

const QString          OF_CONTROLLER_PLAY_NO_EXT_RES  = ":/OFSimulator/res/imgStart";
const QString          OF_CONTROLLER_ADD_NO_EXT_RES   = ":/OFSimulator/res/imgFastForward";
const QString          OF_CONTROLLER_SUB_NO_EXT_RES   = ":/OFSimulator/res/imgRewind";
const QString          OF_CONTROLLER_PAUSE_NO_EXT_RES = ":/OFSimulator/res/imgPause";

const QString          OF_CONTROLLER_POSTFIX          = ".png";
const QString          OF_CONTROLLER_HOVER_POSTFIX    = "-hover.png";
const QString          OF_CONTROLLER_CLICK_POSTFIX    = "-click.png";

const unsigned short   OF_SPEED_MIN                   = 0;
const unsigned short   OF_SPEED_MAX                   = 16;

QTimeController::QTimeController( QWidget* parent, Ui::OFSimulatorClass _ui )
    : QWidget( parent )
{
    ui = _ui;

    RefreshSpeed();

    SetIcon( ui.btnPlay, ":/OFSimulator/res/imgStart.png" );
    SetIcon( ui.btnAddSpeed, ":/OFSimulator/res/imgFastForward.png" );
    SetIcon( ui.btnSubSpeed, ":/OFSimulator/res/imgRewind.png" );

    ui.btnPlay->installEventFilter( this );
    ui.btnAddSpeed->installEventFilter( this );
    ui.btnSubSpeed->installEventFilter( this );
}

QTimeController::~QTimeController()
{
}

void QTimeController::SetIcon( QPushButton* btn, const QString& sResFile )
{
    if( btn == NULLPTR )
        return;

    btn->setIcon( QIcon( QPixmap( sResFile ) ) );
    btn->setIconSize( QSize( 20, 20 ) );
    btn->setStyleSheet( "border:0px; margin:0px;" );
}

QString QTimeController::GetSpeedText()
{
    return GetSpeedText( _rSpeed );
}

QString QTimeController::GetSpeedText( unsigned short rSpeed )
{
    return QString( "%1x" ).arg( rSpeed );
}

bool QTimeController::eventFilter( QObject* watched, QEvent* event )
{
    QPushButton* btn = qobject_cast< QPushButton* >( watched );

    if( event->type() == QEvent::MouseButtonPress )
    {
        if( btn == ui.btnPlay )
        {
            if( _isPause == true )
            {
                SetIcon( btn, OF_CONTROLLER_PAUSE_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
                _isPause = false;
                RefreshSpeed( 1 );
            }
            else
            {
                SetIcon( btn, OF_CONTROLLER_PLAY_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
                _isPause = true;
                RefreshSpeed( 0 );
            }
        }
        else if( btn == ui.btnAddSpeed )
        {
            SetIcon( btn, OF_CONTROLLER_ADD_NO_EXT_RES + OF_CONTROLLER_CLICK_POSTFIX );

            if( _isPause == false )
            {
                if( _rSpeed < OF_SPEED_MAX )
                    RefreshSpeed( _rSpeed * 2 );
            }
        }
        else if( btn == ui.btnSubSpeed )
        {
            SetIcon( btn, OF_CONTROLLER_SUB_NO_EXT_RES + OF_CONTROLLER_CLICK_POSTFIX );

            if( _isPause == false )
            {
                if( _rSpeed > OF_SPEED_MIN )
                    RefreshSpeed( _rSpeed / 2 );
            }
        }
    }
    else if( event->type() == QEvent::Enter )
    {
        if( btn == ui.btnPlay )
        {
            if( _isPause == true )
                SetIcon( btn, OF_CONTROLLER_PLAY_NO_EXT_RES + OF_CONTROLLER_HOVER_POSTFIX );
            else
                SetIcon( btn, OF_CONTROLLER_PAUSE_NO_EXT_RES + OF_CONTROLLER_HOVER_POSTFIX );
        }
        else if( btn == ui.btnAddSpeed )
            SetIcon( btn, OF_CONTROLLER_ADD_NO_EXT_RES + OF_CONTROLLER_HOVER_POSTFIX );
        else if( btn == ui.btnSubSpeed )
            SetIcon( btn, OF_CONTROLLER_SUB_NO_EXT_RES + OF_CONTROLLER_HOVER_POSTFIX );
    }
    else if( event->type() == QEvent::Leave )
    {
        if( btn == ui.btnPlay )
        {
            if( _isPause == true )
                SetIcon( btn, OF_CONTROLLER_PLAY_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
            else
                SetIcon( btn, OF_CONTROLLER_PAUSE_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
        }
        else if( btn == ui.btnAddSpeed )
            SetIcon( btn, OF_CONTROLLER_ADD_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
        else if( btn == ui.btnSubSpeed )
            SetIcon( btn, OF_CONTROLLER_SUB_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
    }
    else if( event->type() == QEvent::MouseButtonRelease )
    {
        if( btn == ui.btnPlay )
        {
            if( _isPause == true )
                SetIcon( btn, OF_CONTROLLER_PLAY_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
            else
                SetIcon( btn, OF_CONTROLLER_PAUSE_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
        }
        else if( btn == ui.btnAddSpeed )
            SetIcon( btn, OF_CONTROLLER_ADD_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
        else if( btn == ui.btnSubSpeed )
            SetIcon( btn, OF_CONTROLLER_SUB_NO_EXT_RES + OF_CONTROLLER_POSTFIX );
    }

    return QWidget::eventFilter( watched, event );
}

void QTimeController::RefreshSpeed()
{
    RefreshSpeed( _rSpeed );
}

void QTimeController::RefreshSpeed( unsigned short rSpeed )
{
    ui.edtSpeed->setText( GetSpeedText( rSpeed ) );
    _rSpeed = rSpeed;
}
