﻿#include "stdafx.h"

#include "UI/QMainWorld.hpp"

#include "Base/ExtConfig.hpp"
#include "def/OFCharacter.hpp"
#include "Module/cCharacterModule.hpp"
#include "Module/EXModuleManager.hpp"

#include "String/EXFormat.hpp"

#include "Util/EXUtil.hpp"

QMainWorld::QMainWorld( QWidget* parent, Ui::OFSimulatorClass _ui )
    : QWidget( parent )
{
    ui = _ui;

    connect( ui.gvMap, SIGNAL( keyPressEvent() ), this, SLOT( keyPressEvent() ) );
}

QMainWorld::~QMainWorld()
{

}

void QMainWorld::Init()
{
    QColor SCENE_BACKGROUND_COLOR( "#ff9200" );
    QColor GV_BACKGROUND_COLOR( Qt::black );

    qDebug() << "gvMap Size : " << ui.gvMap->size();

    QGraphicsScene* scene = new QGraphicsScene( ui.gvMap );
    scene->setBackgroundBrush( QBrush( SCENE_BACKGROUND_COLOR ) );
    _scene = scene;
    scene->installEventFilter( this );

    ui.gvMap->setMouseTracking( true );
    ui.gvMap->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setScene( scene );

    QPixmap pixMap( OF_PIXMAP_WIDTH, OF_PIXMAP_HEIGHT );
    _pixmap = scene->addPixmap( pixMap );

    int nRectXCount = OF_PIXMAP_WIDTH / OF_RECT_SIZE;
    int nRectYCount = OF_PIXMAP_HEIGHT / OF_RECT_SIZE;

    _vecTiles = makeMapTiles( nRectXCount, nRectYCount, OF_TILE_ALGORITHM_V1 );

    for( int idx = 0; idx < nRectYCount; idx++ )
    {
        for( int idx2 = 0; idx2 < nRectXCount; idx2++ )
        {
            qreal rectX = OF_RECT_SIZE * idx2;
            qreal rectY = OF_RECT_SIZE * idx;

            auto& item = _vecTiles[ idx2 ][ idx ];
            QColor tileColor = getTileColor( item.eTile );

            item.pCoord = QPoint( rectX, rectY );

            QGraphicsRectItem* rectItem = scene->addRect( rectX, rectY, OF_RECT_SIZE, OF_RECT_SIZE, QPen( Qt::white ), QBrush( tileColor ) );

            rectItem->setParentItem( _pixmap );

            QVariant var;
            var.setValue( item );
            rectItem->setData( OF_MAP_DATA_TILE_INFO, var );

            if( item.eObject == OF_OBJECT_VILLAGE )
            {
                QPixmap pix( 32, 32 );
                pix.load( ":/OFSimulator/res/village.png" );

                QGraphicsPixmapItem* village = scene->addPixmap( pix );
                village->setScale( 0.5 );
                village->setPos( rectX + OF_RECT_SIZE / 2 - pix.width() / 4, rectY + OF_RECT_SIZE / 2 - pix.height() / 4 );
                village->setParentItem( rectItem );
                village->setZValue( 10 );
                village->setData( OF_MAP_DATA_PARENT_TILE, true );
            }
            else if( item.eObject == OF_OBJECT_HOUSE )
            {
                QPixmap pix( 32, 32 );
                pix.load( ":/OFSimulator/res/house.png" );

                QGraphicsPixmapItem* village = scene->addPixmap( pix );
                village->setScale( 0.5 );
                village->setPos( rectX + OF_RECT_SIZE / 2 - pix.width() / 4, rectY + OF_RECT_SIZE / 2 - pix.height() / 4 );
                village->setParentItem( rectItem );
                village->setZValue( 10 );
                village->setData( OF_MAP_DATA_PARENT_TILE, true );
            }
            else if( item.eObject == OF_OBJECT_CLAN )
            {
                QPixmap pix( 32, 32 );
                pix.load( ":/OFSimulator/res/banner.png" );

                QGraphicsPixmapItem* village = scene->addPixmap( pix );
                village->setScale( 0.5 );
                village->setPos( rectX + OF_RECT_SIZE / 2 - pix.width() / 4, rectY + OF_RECT_SIZE / 2 - pix.height() / 4 );
                village->setParentItem( rectItem );
                village->setZValue( 10 );
                village->setData( OF_MAP_DATA_PARENT_TILE, true );
            }
        }
    }

    _worldInfo.sWorldName = ui.edtProfileWorldName->text();
    _worldTime = new cWorldDateTime( ui );

    _worldInfo.rRighteous = 120;
    _worldInfo.rEvil = 60;
    _worldInfo.rCult = 20;
}

void QMainWorld::InitOld()
{
    qreal rSceneWidth = ui.gvMap->width() * 4;
    qreal rSceneHeight = (double)ui.gvMap->height() * 5.5;

    qreal rPixmapWidth = ui.gvMap->width() * 4;
    qreal rPixmapHeight = ui.gvMap->height() * 4;

    QGraphicsScene* scene = new QGraphicsScene( this );
    scene->setSceneRect( 0, 0, rSceneWidth, rSceneHeight );
    _scene = scene;

    ui.gvMap->setBackgroundBrush( QBrush( "#302f2b" ) );
    ui.gvMap->setScene( scene );
    ui.gvMap->installEventFilter( this );
    ui.gvMap->setMouseTracking( true );
    ui.gvMap->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    ui.gvMap->setCacheMode( QGraphicsView::CacheBackground );
    //ui.gvMap->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
    ui.gvMap->setRenderHint( QPainter::Antialiasing );
    ui.gvMap->setTransformationAnchor( QGraphicsView::NoAnchor );

    int nRectXCount = rPixmapWidth / OF_RECT_SIZE;
    int nRectXRemain = ( int )rPixmapWidth % ( int )OF_RECT_SIZE;

    int nRectYCount = rPixmapHeight / OF_RECT_SIZE;
    int nRectYRemain = ( int )rPixmapHeight % ( int )OF_RECT_SIZE;

    QPixmap pixMap( rPixmapWidth - nRectXRemain, rPixmapHeight - nRectYRemain );
    pixMap.fill( QColor( "#1e1e1e" ) );
    qreal rect_size = 60; // 네모의 크기

    int nRectFullCount = rPixmapHeight * nRectYCount;

    _pixmap = scene->addPixmap( pixMap );
    _pixmap->setPos( 0, 0 );
    //_scene->installEventFilter( this );
    ui.gvMap->update();

    _vecTiles = makeMapTiles( nRectXCount, nRectYCount, OF_TILE_ALGORITHM_V1 );

    int nWriteX = 0;
    int nWriteY = 0;

    for( int idx = 0; idx < nRectYCount; idx++ )
    {
        for( int idx2 = 0; idx2 < nRectXCount; idx2++ )
        {
            qreal rectX = OF_RECT_SIZE * idx2;
            qreal rectY = OF_RECT_SIZE * idx;

            auto& item = _vecTiles[ idx2 ][ idx ];
            QColor tileColor = getTileColor( item.eTile );

            item.pCoord = QPoint( rectX, rectY );

            QGraphicsRectItem* rectItem = scene->addRect( rectX, rectY, OF_RECT_SIZE, OF_RECT_SIZE, QPen( Qt::white ), QBrush( tileColor ) );

            nWriteX = rectItem->rect().width();
            nWriteY = rectItem->rect().height();

            rectItem->setParentItem( _pixmap );

            QVariant var;
            var.setValue( item );
            rectItem->setData( OF_MAP_DATA_TILE_INFO, var );

            if( item.eObject == OF_OBJECT_VILLAGE )
            {
                QPixmap pix( 32, 32 );
                pix.load( ":/OFSimulator/res/village.png" );

                QGraphicsPixmapItem* village = scene->addPixmap( pix );
                village->setScale( 0.5 );
                village->setPos( rectX + OF_RECT_SIZE / 2 - pix.width() / 4, rectY + OF_RECT_SIZE / 2 - pix.height() / 4 );
                village->setParentItem( rectItem );
                village->setZValue( 10 );
                village->setData( OF_MAP_DATA_PARENT_TILE, true );
            }
            else if( item.eObject == OF_OBJECT_HOUSE )
            {
                QPixmap pix( 32, 32 );
                pix.load( ":/OFSimulator/res/house.png" );

                QGraphicsPixmapItem* village = scene->addPixmap( pix );
                village->setScale( 0.5 );
                village->setPos( rectX + OF_RECT_SIZE / 2 - pix.width() / 4, rectY + OF_RECT_SIZE / 2 - pix.height() / 4 );
                village->setParentItem( rectItem );
                village->setZValue( 10 );
                village->setData( OF_MAP_DATA_PARENT_TILE, true );
            }
            else if( item.eObject == OF_OBJECT_CLAN )
            {
                QPixmap pix( 32, 32 );
                pix.load( ":/OFSimulator/res/banner.png" );

                QGraphicsPixmapItem* village = scene->addPixmap( pix );
                village->setScale( 0.5 );
                village->setPos( rectX + OF_RECT_SIZE / 2 - pix.width() / 4, rectY + OF_RECT_SIZE / 2 - pix.height() / 4 );
                village->setParentItem( rectItem );
                village->setZValue( 10 );
                village->setData( OF_MAP_DATA_PARENT_TILE, true );
            }
        }
    }

    qDebug() << "width=" << _scene->width() << "|height=" << _scene->height();
    qDebug() << "width=" << nRectXCount * OF_RECT_SIZE << "|height=" << nRectYCount * OF_RECT_SIZE;
    qDebug() << "width=" << nWriteX << "|height=" << nWriteY;

    _worldInfo.sWorldName = ui.edtProfileWorldName->text();
    _worldTime = new cWorldDateTime( ui );

    _worldInfo.rRighteous = 120;
    _worldInfo.rEvil      = 60;
    _worldInfo.rCult      = 20;
}

QSize QMainWorld::GetPixmapSize()
{
    QSize size( 0, 0 );
    if( _pixmap != NULLPTR )
        size = _pixmap->pixmap().size();

    return size;
}

QPoint QMainWorld::GetMovealeSize()
{
    QPoint p( 0, 0 );
    if( _pixmap != NULLPTR )
    {
        p.setX( _pixmap->pixmap().size().width() - 10 );
        p.setY( _pixmap->pixmap().size().height() - 10 );
    }

    return p;
}

QGraphicsPixmapItem* QMainWorld::GetPixmap()
{
    return _pixmap;
}

QGraphicsScene* QMainWorld::GetScene()
{
    return _scene;
}

stWORLD_INFO QMainWorld::GetWorldInfo()
{
    return _worldInfo;
}

void QMainWorld::ShowClickedItem( QMouseEvent* mouseEvent )
{

    QPoint p = mouseEvent->pos();

    p.setX( p.x() + ui.gvMap->horizontalScrollBar()->value() );
    p.setY( p.y() + ui.gvMap->verticalScrollBar()->value() );

    //auto mapPoint = ui.gvMap->mapFromScene( mouseEven t->pos() );
    QGraphicsItem* item = _scene->itemAt( p, _pixmap->sceneTransform() );
    auto lstItem = _scene->items( p, Qt::IntersectsItemShape, Qt::DescendingOrder, _pixmap->sceneTransform() );

    QGraphicsRectItem* pItem = NULLPTR;

    for( auto* itemTmp : lstItem )
    {
        pItem = dynamic_cast< QGraphicsRectItem* >( itemTmp );

        if( pItem != NULLPTR )
            break;
    }

    ui.lblMapPoint->setText( QString( "-, -" ) );
    ui.lblMapTile->setText( QString( "-" ) );
    ui.lblMapObject->setText( QString( "-" ) );
    ui.lstCharacter->clear();

    do
    {
        if( pItem == NULLPTR )
        {
            auto pItemChild = dynamic_cast< QGraphicsPixmapItem* >( item );

            if( pItemChild == NULLPTR )
            {
                auto pCharacter = dynamic_cast< QGraphicsEllipseItem* >( item );

                if( pCharacter == NULLPTR )
                    break;
            }

            if( pItemChild->data( OF_MAP_DATA_PARENT_TILE ).toBool() == false )
                break;

            pItem = dynamic_cast< QGraphicsRectItem* >( pItemChild->parentItem() );

            if( pItem == NULLPTR )
                break;
        }

        QVariant var = pItem->data( OF_MAP_DATA_TILE_INFO );
        stOFTileInfo data = var.value<stOFTileInfo>();

        ui.lblMapPoint->setText( QString( "%1, %2" ).arg( data.pCoord.x() ).arg( data.pCoord.y() ) );
        ui.lblMapTile->setText( QString( "%1" ).arg( getTileName( data.eTile ) ) );
        ui.lblMapObject->setText( QString( "%1" ).arg( getObjectName( data.eObject ) ) );

        QRect rect( QPoint( data.pCoord.x(), data.pCoord.y() ), QPoint( data.pCoord.x() + OF_RECT_SIZE - 1, data.pCoord.y() + OF_RECT_SIZE - 1 ) );
        auto lstCharacter = _scene->items( rect, Qt::IntersectsItemShape, Qt::DescendingOrder, _pixmap->sceneTransform() );

        for( auto ofCharacter : lstCharacter )
        {
            auto pCharacter = dynamic_cast< QGraphicsEllipseItem* >( ofCharacter );

            if( pCharacter == NULLPTR )
                continue;

            QString sUUID = ofCharacter->data( OF_CHARACTER_DATA_UUID ).toString();

            if( sUUID.isEmpty() == true )
                continue;

            auto spCharacterModule = Ext::Module::GetModule< cCharacterModule >( L"CHARACTER" );
            stOFCharacter* OFCharacter = spCharacterModule->GetCharacter( sUUID );

            if( OFCharacter == NULLPTR )
                continue;

            QString sName = OFCharacter->stInfo.sFirstName + " " + OFCharacter->stInfo.sSecondName;

            QListWidgetItem* item = new QListWidgetItem( ui.lstCharacter );
            item->setText( sName );
            item->setData( OF_CHARACTER_DATA_UUID, sUUID );
        }

    } while( false );
}

void QMainWorld::WheelEvent( QWheelEvent* wheelEvent )
{
    double scaleFactor = 0.25;

    if( wheelEvent->angleDelta().y() > 0 )
    {
        // Zoom in
        double newScaleFactor = _scaleFactor + scaleFactor;
        if( newScaleFactor >= 3.0 )
            return;

        _scaleFactor = newScaleFactor;
        qDebug() << "Zoom in" << _scaleFactor;
    }
    else
    {
        // Zoom out
        double newScaleFactor = _scaleFactor - scaleFactor;
        if( newScaleFactor < 0.5 )
            return;

        _scaleFactor = newScaleFactor;
        qDebug() << "Zoom out" << _scaleFactor;
    }

    // Scale the pixmap
    _pixmap->setScale( _scaleFactor );

    int maximumWidth = static_cast< int >( 3600 * _scaleFactor );
    ui.gvMap->horizontalScrollBar()->setMaximum( maximumWidth );

    int maximumHeight = static_cast< int >( 3600 * _scaleFactor );
    ui.gvMap->verticalScrollBar()->setMaximum( maximumHeight );

    ui.gvMap->horizontalScrollBar()->setValue( 0 );
    ui.gvMap->verticalScrollBar()->setValue( 0 );

    QSize scaledSize = _pixmap->boundingRect().size().toSize() * _scaleFactor;
    ui.gvMap->scene()->setSceneRect( QRectF( QPointF(), scaledSize ) );
}

bool QMainWorld::eventFilter( QObject* watched, QEvent* event )
{
    QString objName = watched->objectName();
    QKeyEvent* keyEvent = static_cast< QKeyEvent* >( event );
    QEvent::Type eventType = keyEvent->type();

    if( eventType == QEvent::KeyPress )
    {
        QKeyEvent* keyEvent = static_cast< QKeyEvent* >( event );

        switch( keyEvent->key() )
        {
            /*
            case Qt::Key_Left:
            case Qt::Key_A:
            _pixmap->moveBy( 100, 0 );
            break;
            case Qt::Key_D:
            case Qt::Key_Right:
            _pixmap->moveBy( -100, 0 );
            break;
            case Qt::Key_W:
            case Qt::Key_Up:
            _pixmap->moveBy( 0, 100 );
            break;
            case Qt::Key_S:
            case Qt::Key_Down:
            _pixmap->moveBy( 0, -100 );
            break;
            */
            default:
            break;
        }
    }
    else if( eventType == QEvent::KeyRelease )
    {
        QKeyEvent* keyEvent = static_cast< QKeyEvent* >( event );
    }
    else if( eventType == QEvent::GraphicsSceneDragEnter )
    {
        qDebug() << "drag";
    }
    else if( eventType == QEvent::Wheel )
    {
    }
    else if( event->type() == QEvent::MouseMove || event->type() == QEvent::DragMove )
    {
        QMouseEvent* mouseEvent = static_cast< QMouseEvent* >( event );
        qDebug() << "MOVE";

        if( mouseEvent->buttons() == Qt::LeftButton )
        {
        }
    }
    else if( event->type() == QEvent::GraphicsSceneDragMove )
    {
        qDebug() << "scene drag move";
    }
    else if( event->type() == QEvent::GraphicsSceneHoverMove )
    {
        qDebug() << "scene hover move";
    }
    else if( event->type() == QEvent::GraphicsSceneMousePress )
    {
        QGraphicsSceneMouseEvent* mouseEvent = static_cast< QGraphicsSceneMouseEvent* >( event );
        if( mouseEvent->button() == Qt::RightButton )
        {
        }
    }
    else if( event->type() == QEvent::GraphicsSceneMouseRelease )
    {
        QGraphicsSceneMouseEvent* mouseEvent = static_cast< QGraphicsSceneMouseEvent* >( event );
        if( mouseEvent->button() == Qt::RightButton )
        {
            qDebug() << "우클릭 해제";

            ui.gvMap->setDragMode( QGraphicsView::NoDrag );
        }
    }
    else if( event->type() == QEvent::GraphicsSceneMouseMove )
    {
        QGraphicsSceneMouseEvent* mouseEvent = static_cast< QGraphicsSceneMouseEvent* >( event );
    }
    else if( eventType == QEvent::MouseButtonPress )
    {
        QMouseEvent* mouseEvent = static_cast< QMouseEvent* >( event );

        if( mouseEvent->button() == Qt::LeftButton )
        {
            qDebug() << "LeftButton clicked";
        }
        else if( mouseEvent->button() == Qt::RightButton )
        {

        }
    }
    else if( event->type() == QEvent::MouseButtonRelease )
    {

    }

    return QWidget::eventFilter( watched, event );
}

vec2DTiles QMainWorld::makeMapTiles( int nX, int nY, eTileAlgorithm eAlgorithm )
{
    vec2DTiles vec2DTiles( nX, QVector< stOFTileInfo >( nY, stOFTileInfo() ) );

    QPoint pCenter( ( int )nX / 2, ( int )nY / 2 );
    QPoint pMax( nX, nY );
    QPoint pMin( 0, 0 );

    qDebug() << "X=" << pCenter.x() << "|Y=" << pCenter.y();

    for( auto vecX : vec2DTiles )
    {
        QString s;
        for( auto data : vecX )
            s += QString::number( ( int )data.eTile ) + ", ";

        s.chop( 2 );
        qDebug() << s;
    }

    // 센터는 평지로
    vec2DTiles[ pCenter.x() ][ pCenter.y() ].eTile = OF_TILE_FLAT;
    
    switch( eAlgorithm )
    {
        case OF_TILE_ALGORITHM_V1: { makeMapTilesV1( nX, nY, vec2DTiles ); } break;
        case OF_TILE_ALGORITHM_V2: { makeMapTilesV2( nX, nY, vec2DTiles ); } break;
        default: { makeMapTilesV1( nX, nY, vec2DTiles ); } break;
    }

    makeObject( vec2DTiles );

    return vec2DTiles;
}

void QMainWorld::makeMapTilesV1( int nX, int nY, vec2DTiles& vec2DTiles )
{
    /*
     * 1. 중앙 Y라인을 랜덤으로 다 채움 ( FLAT / GRASS / MOUNTAIN )
     * 2. 그 후 첫 X라인부터 채움
     */

    QPoint pCenter( nX / 2, nY / 2 );
    QPoint pMax( nX, nY );
    QPoint pMin( 0, 0 );

    auto rand = Ext::Util::cRandom< int >( 0, 11000 );

    eTiles Tile = OF_TILE_FLAT;

    for( int idx = pCenter.y(); idx < pMax.y(); idx++ )
    {
        if( vec2DTiles[ pCenter.x() ][ idx ].eTile != OF_TILE_NONE )
            continue;

        Tile = getRecommendTile( OF_TILE_FLAT, QPoint( pCenter.x(), idx ), pMax, rand.Generate() );
        vec2DTiles[ pCenter.x() ][ idx ].eTile = Tile;
    }

    for( int idx = 0; idx < pCenter.y(); idx++ )
    {
        if( vec2DTiles[ pCenter.x() ][ idx ].eTile != OF_TILE_NONE )
            continue;

        Tile = getRecommendTile( OF_TILE_FLAT, QPoint( pCenter.x(), idx ), pMax, rand.Generate() );
        vec2DTiles[ pCenter.x() ][ idx ].eTile = Tile;
    }

    // X 우측 하단
    for( int idx = pCenter.x(); idx < pMax.x(); idx++ )
    {
        for( int idx2 = pCenter.y(); idx2 < pMax.y(); idx2++ )
        {
            if( vec2DTiles[ idx ][ idx2 ].eTile != OF_TILE_NONE )
            {
                Tile = vec2DTiles[ idx ][ idx2 ].eTile;
                continue;
            }

            Tile = getRecommendTile( Tile, QPoint( idx, idx2 ), pMax, rand.Generate() );
            vec2DTiles[ idx ][ idx2 ].eTile = Tile;
        }
    }

    // X 우측 상단
    for( int idx = pCenter.x(); idx < pMax.x(); idx++ )
    {
        for( int idx2 = 0; idx2 < pCenter.y(); idx2++ )
        {
            if( vec2DTiles[ idx ][ idx2 ].eTile != OF_TILE_NONE )
            {
                Tile = vec2DTiles[ idx ][ idx2 ].eTile;
                continue;
            }

            Tile = getRecommendTile( Tile, QPoint( idx, idx2 ), pMax, rand.Generate() );
            vec2DTiles[ idx ][ idx2 ].eTile = Tile;
        }
    }

    // X 우측 하단
    for( int idx = 0; idx < pCenter.x(); idx++ )
    {
        for( int idx2 = pCenter.y(); idx2 < pMax.y(); idx2++ )
        {
            if( vec2DTiles[ idx ][ idx2 ].eTile != OF_TILE_NONE )
            {
                Tile = vec2DTiles[ idx ][ idx2 ].eTile;
                continue;
            }

            Tile = getRecommendTile( Tile, QPoint( idx, idx2 ), pMax, rand.Generate() );
            vec2DTiles[ idx ][ idx2 ].eTile = Tile;
        }
    }

    // X 우측 상단
    for( int idx = 0; idx < pCenter.x(); idx++ )
    {
        for( int idx2 = 0; idx2 < pCenter.y(); idx2++ )
        {
            if( vec2DTiles[ idx ][ idx2 ].eTile != OF_TILE_NONE )
            {
                Tile = vec2DTiles[ idx ][ idx2 ].eTile;
                continue;
            }

            Tile = getRecommendTile( Tile, QPoint( idx, idx2 ), pMax, rand.Generate() );
            vec2DTiles[ idx ][ idx2 ].eTile = Tile;
        }
    }

}

void QMainWorld::makeMapTilesV2( int nX, int nY, vec2DTiles& vec2DTiles )
{
    // 중앙 한 칸부터 확률에 따라 이어가고, 확률에 실패하면 다음 타일을 이어가는 형식
}

void QMainWorld::makeObject( vec2DTiles& vec2DTiles )
{
    int nTotalCount = 0;

    for( auto vecX : vec2DTiles )
    {
        for( auto vecY : vecX )
        {
            nTotalCount++;
        }
    }

    // 마을 개수
    int nVillageMin = nTotalCount / OF_VILLAGE_MIN;
    int nVillageMax = nTotalCount / OF_VILLAGE_MAX;
    int nVillageCnt = Ext::Util::cRandom< int >( nVillageMin, nVillageMax ).Generate();

    int nHouseMin = nTotalCount / OF_HOUSE_MIN;
    int nHouseMax = nTotalCount / OF_HOUSE_MAX;
    int nHouseCnt = Ext::Util::cRandom< int >( nHouseMin, nHouseMax ).Generate();

    int nClanMin = nTotalCount / OF_CLAN_MIN;
    int nClanMax = nTotalCount / OF_CLAN_MAX;
    int nClanCnt = Ext::Util::cRandom< int >( nClanMin, nClanMax ).Generate();

    int nXMax = vec2DTiles.count();

    int nLineToVillage   = 0;
    int nLineToHouse     = 0;
    int nLineClan        = 0;

    if( nXMax > nVillageCnt )
    {
        nLineToVillage = ceil( ( float )nXMax / nVillageCnt );
        nLineToHouse = ceil( ( float )nXMax / nHouseCnt );
        nLineClan = ceil( ( float )nXMax / nClanCnt );
    }
    else
    {
        nLineToVillage = ceil( ( float )nVillageCnt / nXMax );
        nLineToHouse = ceil( ( float )nHouseCnt / nXMax );
        nLineClan = ceil( ( float )nClanCnt / nXMax );
    }

    makeObjectImpl( vec2DTiles, OF_OBJECT_VILLAGE, nLineToVillage );
    makeObjectImpl( vec2DTiles, OF_OBJECT_CLAN, nLineClan );
    makeObjectImpl( vec2DTiles, OF_OBJECT_HOUSE, nLineToHouse );
}

void QMainWorld::makeObjectImpl( vec2DTiles& vec2DTiles, eOFObject eObject, int nMakeObjectToLine )
{
    int nRemainVillage = 0;
    int nRemainHouse = 0;
    int nRemainClan = 0;

    QSet< int > setDuplVillageX;
    QSet< int > setDuplVillageY;

    QSet< int > setDuplHouseX;
    QSet< int > setDuplHouseY;

    QSet< int > setClanX;
    QSet< int > setClanY;

    int nRemainCnt = 0;
    int nMaxX = 0;

    Ext::Util::cRandom< int > cRandomObject = Ext::Util::cRandom< int >( 0, 10000 );

    for( auto vecX : vec2DTiles )
    {
        int nInstallCnt = nMakeObjectToLine;

        nMaxX = vecX.count();

        Ext::Util::cRandom< int > cRandomY = Ext::Util::cRandom< int >( 0, ( vecX.count() - 1 ) );
        Ext::Util::cRandom< int > cRandomX = Ext::Util::cRandom< int >( 0, ( vec2DTiles.count() - 1 ) );

        // 3회만 랜덤하도록 함. 안 되면 Remain으로 남김
        for( int idx = 0; idx < 3; idx++ )
        {
            if( nInstallCnt <= 0 )
                break;

            int nY = cRandomY.Generate();

            auto item = vecX[ nY ];

            if( item.eObject != OF_OBJECT_NONE )
            {
                nRemainCnt++;
                continue;
            }

            int nCompare = getObjectPercentage( eObject, item.eTile );

            if( cRandomObject.Generate() >= nCompare )
            {
                item.eObject = eObject;
                nInstallCnt--;
            }
            else
            {
                nRemainCnt++;
            }
        }
    }

    int nXRandom = vec2DTiles.count();

    Ext::Util::cRandom< int > cRandomY = Ext::Util::cRandom< int >( 0, ( nMaxX - 1 ) );
    Ext::Util::cRandom< int > cRandomX = Ext::Util::cRandom< int >( 0, ( vec2DTiles.count() - 1 ) );

    while( nRemainCnt > 0 )
    {
        int nX = cRandomX.Generate();
        int nY = cRandomY.Generate();

        auto& item = vec2DTiles[ nX ][ nY ];

        if( item.eObject != OF_OBJECT_NONE )
            continue;

        int nCompare = getObjectPercentage( eObject, item.eTile );

        if( cRandomObject.Generate() >= nCompare )
        {
            item.eObject = eObject;
            nRemainCnt--;
        }
    }
}

int QMainWorld::getObjectPercentage( eOFObject eObject, eTiles eTile )
{
    int nCompare = 10000;

    if( eObject == OF_OBJECT_VILLAGE )
    {
        if( eTile == OF_TILE_COAST || eTile == OF_TILE_LAKE )
            nCompare -= 8000;
        else if( eTile == OF_TILE_SNOW || eTile == OF_TILE_VOLCANO )
            nCompare -= 9000;
        else if( eTile == OF_TILE_CLIFF )
            nCompare -= 9500;
        else if( eTile == OF_TILE_MOUNTAIN )
            nCompare -= 6000;
        else
            nCompare -= 3000;
    }
    else if( eObject == OF_OBJECT_HOUSE )
    {
        if( eTile == OF_TILE_COAST || eTile == OF_TILE_LAKE )
            nCompare -= 10000;
        else if( eTile == OF_TILE_SNOW || eTile == OF_TILE_VOLCANO )
            nCompare -= 9000;
        else if( eTile == OF_TILE_CLIFF )
            nCompare -= 8000;
        else if( eTile == OF_TILE_MOUNTAIN )
            nCompare -= 5000;
        else
            nCompare -= 3000;
    }
    else if( eObject == OF_OBJECT_CLAN )
    {
        if( eTile == OF_TILE_COAST || eTile == OF_TILE_LAKE )
            nCompare -= 5000;
        else if( eTile == OF_TILE_SNOW || eTile == OF_TILE_VOLCANO )
            nCompare -= 6000;
        else if( eTile == OF_TILE_CLIFF )
            nCompare -= 9000;
        else
            nCompare -= 3000;
    }

    return nCompare;
}

QColor QMainWorld::getTileColor( eTiles eTile )
{
    QString sColor;

    switch( eTile )
    {
        case OF_TILE_NONE: { sColor = "#000000"; } break;
        case OF_TILE_FLAT: { sColor = "#c2a17a"; } break;
        case OF_TILE_GRASS: { sColor = "#8dba68"; } break;
        case OF_TILE_MOUNTAIN: { sColor = "#679343"; } break;
        case OF_TILE_CLIFF: { sColor = "#6f593e"; } break;
        case OF_TILE_COAST: { sColor = "#86a3c7"; } break;
        case OF_TILE_LAKE: { sColor = "#7596bf"; } break;
        case OF_TILE_HILLS: { sColor = "#80b256"; } break;
        case OF_TILE_WOODS: { sColor = "#73a44b"; } break;
        case OF_TILE_SNOW: { sColor = "#5096e7"; } break;
        case OF_TILE_VOLCANO: { sColor = "#ba6c68"; } break;
        default: { sColor = "#ffffff"; }break;
    }

    QColor color( sColor );
    return color;
}

QString QMainWorld::getTileName( eTiles eTile )
{
    QString sTileName;

    switch( eTile )
    {
        case OF_TILE_NONE: { sTileName = "없음"; } break;
        case OF_TILE_FLAT: { sTileName = "평지"; } break;
        case OF_TILE_GRASS: { sTileName = "초원"; } break;
        case OF_TILE_MOUNTAIN: { sTileName = "산"; } break;
        case OF_TILE_CLIFF: { sTileName = "절벽"; } break;
        case OF_TILE_COAST: { sTileName = "호수"; } break;
        case OF_TILE_LAKE: { sTileName = "해안"; } break;
        case OF_TILE_HILLS: { sTileName = "언덕"; } break;
        case OF_TILE_WOODS: { sTileName = "숲"; } break;
        case OF_TILE_SNOW: { sTileName = "빙하"; } break;
        case OF_TILE_VOLCANO: { sTileName = "화산"; } break;
        default: { sTileName = "#없음"; }break;
    }

    return sTileName;
}

QString QMainWorld::getObjectName( eOFObject eObject )
{
    QString sObjectName;

    switch (eObject) {
        case OF_OBJECT_NONE: { sObjectName = "-"; } break;
        case OF_OBJECT_VILLAGE: { sObjectName = "마을"; } break;
        case OF_OBJECT_CLAN: { sObjectName = "문파"; } break;
        case OF_OBJECT_HOUSE: { sObjectName = "이름 없는 집"; } break;
        default: { sObjectName = "-"; } break;
    }

    return sObjectName;
}

eTiles QMainWorld::getRecommendTile( eTiles eTile, QPoint pCurrent, QPoint pMax, int nRandom )
{
    /*
     * 1. X나 Y가 MAX에 가까울수록 바다가 나올 확률이 높아짐 ( 0~4 / MAX-4~MAX 부터 확률 증가 )
     * 2. 현재 지형이 산이면 절벽이 생성될 수 있음
     */

    QPoint pCenter( pMax.x() / 2, pMax.y() / 2 );

    eTiles ret = OF_TILE_NONE;
    int nPercentage = 0;

    int nMaximumPercent = 11000;
    int nCurrentLeft = nMaximumPercent;
    int nCurrent = 0;

    int nLAKE = 0;

    // 바다 확률은 고정
    if( pCurrent.x() == 0 || pCurrent.y() == 0 )
        nLAKE = 9000;
    else if( pCurrent.x() >= 1 && pCurrent.x() <= 4 )
        nLAKE = 6000 - 1000 * pCurrent.x();
    else if( pCurrent.y() >= 1 && pCurrent.y() <= 4 )
        nLAKE = 6000 - 1000 * pCurrent.y();
    else
        nLAKE = 1000;

    nCurrentLeft -= nLAKE;

    // 9000 / 1000 = 9 = 0.1%
    // 9000 / 100 = 90 = 1%
    // 9000 / 10 = 900 = 10% 
    int nLeftd = nCurrentLeft / 1000;

    switch( eTile )
    {
        case OF_TILE_NONE: {} break;
        case OF_TILE_FLAT:
        {
            /*
             * FLAT 0.7 / GRASS 0.1 / HILLS 0.1 / WOODS 0.1
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 700 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            nCurrent += nLeftd * 700;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_HILLS;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_WOODS;
                break;
            }

            ret = OF_TILE_FLAT;
        } break;
        case OF_TILE_GRASS:
        {
            /*
             * GRASS 0.4 / FLAT 0.2 / HILLS 0.1 / COAST 0.1 / WOODS 0.2
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_HILLS;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_COAST;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_WOODS;
                break;
            }

            ret = OF_TILE_FLAT;
        } break;
        case OF_TILE_MOUNTAIN:
        {
            /*
             * MOUNTAIN 0.4 / HILLS 0.2 / CLIFF 0.1 / SNOW 0.05 / VOLCANO 0.05 / GRASS 0.1 / FLAT 0.1
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_MOUNTAIN;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_HILLS;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_CLIFF;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 50 )
            {
                ret = OF_TILE_SNOW;
                break;
            }

            nCurrent += nLeftd * 50;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 50 )
            {
                ret = OF_TILE_VOLCANO;
                break;
            }

            nCurrent += nLeftd * 50;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            ret = OF_TILE_MOUNTAIN;

        } break;
        case OF_TILE_CLIFF:
        {
            /*
             * MOUNTAIN 0.4 / CLIFF 0.4 / HILLS 0.1 / VOLCANO 0.1
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_MOUNTAIN;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_CLIFF;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_HILLS;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_VOLCANO;
                break;
            }

            ret = OF_TILE_CLIFF;

        } break;
        case OF_TILE_COAST:
        {
            /*
             * COAST 0.1 / FLAT 0.4 / GRASS 0.45 / SNOW 0.05
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_COAST;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 300 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            nCurrent += nLeftd * 300;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 450 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 450;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 50 )
            {
                ret = OF_TILE_SNOW;
                break;
            }

            ret = OF_TILE_COAST;

        } break;
        case OF_TILE_LAKE:
        {
            /*
             * 어차피 LAKE 확률은 위에서 보정하기 때문에, 적게 넣도록 함
             * COAST 0.2 / FLAT 0.5 / GRASS 0.25 / SNOW 0.05
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_COAST;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 500 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            nCurrent += nLeftd * 500;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 250 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 250;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 50 )
            {
                ret = OF_TILE_SNOW;
                break;
            }

            ret = OF_TILE_LAKE;

        } break;
        case OF_TILE_HILLS:
        {
            /*
             * HILLS 0.4 / MOUNTAIN 0.2 / WOODS 0.2 / GRASS 0.15 / VOLCANO 0.05
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_HILLS;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_MOUNTAIN;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_WOODS;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 150 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 150;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 50 )
            {
                ret = OF_TILE_VOLCANO;
                break;
            }

            ret = OF_TILE_HILLS;
        } break;
        case OF_TILE_WOODS:
        {
            /*
             * WOODS 0.4 / GRASS 0.3 / MOUNTAIN 0.2 / FLAT 0.1
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_WOODS;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 300 )
            {
                ret = OF_TILE_GRASS;
                break;
            }

            nCurrent += nLeftd * 300;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_MOUNTAIN;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            ret = OF_TILE_WOODS;

        } break;
        case OF_TILE_SNOW:
            {
            /*
             * SNOW 0.1 / COAST 0.1 / MOUNTAIN 0.4 / FLAT 0.4
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_SNOW;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 100 )
            {
                ret = OF_TILE_COAST;
                break;
            }

            nCurrent += nLeftd * 100;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_MOUNTAIN;
                break;
            }

            nCurrent += nLeftd * 400;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 400 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            ret = OF_TILE_SNOW;
            } break;
        case OF_TILE_VOLCANO:
            {

            /*
             * VOLCANO 0.3 / MOUNTAIN 0.2 / FLAT 0.3 / HILLS 0.2
             */

            if( nRandom >= 0 && nRandom <= nLAKE )
            {
                ret = OF_TILE_LAKE;
                break;
            }

            nCurrent += nLAKE;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 300 )
            {
                ret = OF_TILE_VOLCANO;
                break;
            }

            nCurrent += nLeftd * 300;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 200 )
            {
                ret = OF_TILE_MOUNTAIN;
                break;
            }

            nCurrent += nLeftd * 200;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 300 )
            {
                ret = OF_TILE_FLAT;
                break;
            }

            nCurrent += nLeftd * 300;
            if( nRandom > nCurrent && nRandom <= nCurrent + nLeftd * 300 )
            {
                ret = OF_TILE_HILLS;
                break;
            }

            ret = OF_TILE_VOLCANO;
        } break;
        default: {} break;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////

cWorldDateTime::cWorldDateTime( Ui::OFSimulatorClass _ui )
    : ui( _ui )
{
    RefreshLabel();
}

cWorldDateTime::~cWorldDateTime()
{
}

void cWorldDateTime::SpendYear()
{
    _rYears++;

    _rMonth = 1;
    _rDays = 1;
    _rHour = 0;
    _rMinute = 0;
    _rSecond = 0;

    RefreshLabel();
}

void cWorldDateTime::SpendMonth()
{
    _rMonth++;

    if( _rMonth == 12 )
    {
        _rMonth = 1;
        SpendYear();
    }
    else
    {
        RefreshLabel();
    }
}

void cWorldDateTime::SpendDay()
{
    _rDays++;

    if( _rDays == 30 )
    {
        _rDays = 1;
        SpendMonth();
    }
    else
    {
        RefreshLabel();
    }
}

void cWorldDateTime::SpendHour()
{
    _rHour++;

    if( _rHour == 24 )
    {
        _rHour = 0;
        SpendDay();
    }
    else
    {
        RefreshLabel();
    }
}

void cWorldDateTime::SpendMinute()
{
    _rMinute++;

    if( _rMinute == 60 )
    {
        _rMinute = 0;
        SpendHour();
    }
    else
    {
        RefreshLabel();
    }
}

void cWorldDateTime::SpendSecond()
{
    _rSecond++;

    if( _rSecond == 60 )
    {
        _rSecond = 0;
        SpendMinute();
    }
    else
    {
        RefreshLabel();
    }
}

void cWorldDateTime::RefreshLabel()
{
    QString sWorldTime = Ext::Format::Format( "{}년 {}월 {}일 {}시 {}분 {}초", _rYears, _rMonth, _rDays, _rHour, _rMinute, _rSecond );
    ui.lblWorldTime->setText( sWorldTime );
}

/*
    switch( eTile )
    {
        case OF_TILE_NONE: {} break;
        case OF_TILE_FLAT: {} break;
        case OF_TILE_GRASS: {} break;
        case OF_TILE_MOUNTAIN: {} break;
        case OF_TILE_CLIFF: {} break;
        case OF_TILE_COAST: {} break;
        case OF_TILE_LAKE: {} break;
        case OF_TILE_HILLS: {} break;
        case OF_TILE_WOODS: {} break;
        case OF_TILE_SNOW: {} break;
        case OF_TILE_VOLCANO: {} break;
        case OF_TILE_VILLAGE: {} break;
        case OF_TILE_CLAN: {} break;
        case OF_TILE_HOUSE: {} break;
        default: {} break;
    }
 */