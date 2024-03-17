#include "stdafx.h"

#include "Module/cCharacterModule.hpp"
#include "Module/EXModuleManager.hpp"

#include "UI/QMakeProfile.hpp"

#include "OFSimulator.h"
#include "Base/ExtConfig.hpp"

using namespace Ext;

QMakeProfile::QMakeProfile( QWidget* parent, Ui::OFSimulatorClass _ui )
    : QWidget( parent )
{
    ui = _ui;

    connect( ui.btnProfileCreate, SIGNAL( clicked() ), this, SLOT( OnBtnProfileCreateClicked() ) );
    connect( ui.btnProfileClose, SIGNAL( clicked() ), this, SLOT( OnBtnProfileCloseClicked() ) );

    ui.edtProfileStartYear->setValidator( new QIntValidator( 0, 1000, this ) );
    ui.edtProfileStartYear->setText( QString::number( ui.sdProfileYear->value() ) );

    connect( ui.edtProfileStartYear, &QLineEdit::textEdited, this, [this]
    {
        ui.sdProfileYear->setValue( ui.edtProfileStartYear->text().toInt() );
    } );

    connect( ui.sdProfileYear, &QSlider::sliderMoved, this, [this]
    {
        ui.edtProfileStartYear->setText( QString::number( ui.sdProfileYear->value() ) );
    } );
}

QMakeProfile::~QMakeProfile()
{
}

void QMakeProfile::Init()
{
    _nPrevIndex = ui.ControlCentor->currentIndex();
    ui.ControlCentor->setCurrentIndex( 2 );
}

void QMakeProfile::OnBtnProfileCreateClicked()
{
    if( ui.edtProfileName->text().isEmpty() == true )
    {
        QMessageBox::information( this, "OF", "캐릭터 이름은 빈칸일 수 없습니다." );
        return;
    }

    if( ui.edtProfileWorldName->text().isEmpty() == true )
    {
        QMessageBox::information( this, "OF", "월드 이름은 빈칸일 수 없습니다." );
        return;
    }

    if( ui.edtProfileStartYear->text().isEmpty() == true )
    {
        QMessageBox::information( this, "OF", "시작년도는 0~1000 이내의 숫자만 가능합니다." );
        return;
    }

    auto spCharacter = Module::GetModule< cCharacterModule >( L"CHARACTER" );

    qobject_cast< OFSimulator* >( parent() )->WorldStart();
    qobject_cast< OFSimulator* >( parent() )->CharacterStart();
}

void QMakeProfile::OnBtnProfileCloseClicked()
{
    ui.ControlCentor->setCurrentIndex( _nPrevIndex );
}
