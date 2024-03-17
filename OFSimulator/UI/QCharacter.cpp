#include "stdafx.h"

#include "UI/QCharacter.hpp"
#include "UI/QMainWorld.hpp"

#include "Base/ExtConfig.hpp"
#include "Util/EXUtil.hpp"

#include "Module/cCharacterModule.hpp"
#include "Module/EXModuleManager.hpp"

#include "OFSimulator.h"
#include "def/OFCharacter.hpp"


QCharacter::QCharacter( QWidget* parent, Ui::OFSimulatorClass _ui )
    : QWidget( parent )
{
    ui = _ui;
}

QCharacter::~QCharacter()
{
}

QString QCharacter::MakeComfortableColor()
{
    QString sHexColor = "";
    Ext::Util::cRandom< int > rand = Ext::Util::cRandom< int >( 0, 255 );

    while( true )
    {
        sHexColor = "";

        for( int idx = 0; idx < 3; idx++ )
        {
            int nRand = rand.Generate();

            if( nRand < 16 )
                sHexColor += "0";

            sHexColor += QString::number( rand.Generate(), 16 );
        }

        if( sHexColor.toInt( nullptr, 16 ) < 0x333333 )
            continue;

        // 예시: 너무 밝은 색상일 경우
        if( sHexColor.toInt( nullptr, 16 ) > 0xCCCCCC )
            continue;

        // 눈에 불편하지 않은 색상일 경우
        break;
    }

    return sHexColor;
}

void QCharacter::WriteCharacter( QString sUUID )
{
    OFSimulator* parent = dynamic_cast< OFSimulator* >( this->parent() );

    int nX = 0;
    int nY = 0;

    QSize characterSize = OF_CHARACTER_LEVEL_1;

    QGraphicsScene* scene = parent->GetWorld()->GetScene();
    QString sColor = "#" + MakeComfortableColor();
    QGraphicsEllipseItem* ellipseItem = scene->addEllipse( nX, nY, characterSize.width(), characterSize.height(), QPen( Qt::black ), QBrush( QColor( sColor ) ) );
    ellipseItem->setData( OF_CHARACTER_DATA_UUID, sUUID );
    ellipseItem->setParentItem( parent->GetWorld()->GetPixmap() );

    auto spCharacterModule = Ext::Module::GetModule< cCharacterModule >( L"CHARACTER" );
    spCharacterModule->SetCharacterCoord( sUUID, QPoint( nX, nY ) );
    spCharacterModule->SetCharacterColor( sUUID, sColor );
}
