#include "stdafx.h"

#include "UI/QMainWorld.hpp"

#include "Base/ExtConfig.hpp"
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
    ui.ControlCentor->setCurrentIndex( 3 );

    QGraphicsScene* scene = new QGraphicsScene( this );
    scene->setSceneRect( 0, 0, ui.gvMap->width() * 3, ui.gvMap->height() * 3 );
    _scene = scene;

    ui.gvMap->setBackgroundBrush( QBrush( "#302f2b" ) );
    ui.gvMap->setScene( scene );
    ui.gvMap->installEventFilter( this );
    ui.gvMap->setMouseTracking( true );
    ui.gvMap->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    ui.gvMap->setCacheMode( QGraphicsView::CacheBackground );
    //ui.gvMap->setDragMode( QGraphicsView::ScrollHandDrag );

    _scaleFactor = 1.0f;

    QPixmap pix( _scene->width(), _scene->height() );
    pix.fill( QColor( "#1e1e1e" ) );
    _pixmap = scene->addPixmap( pix );
    _pixmap->setPos( 0, 0 );

    qreal rect_size = 60; // 네모의 크기

    qDebug() << "width=" << _scene->width() << "|height=" << _scene->height();

    int nRectXCount = _scene->width() / rect_size;
    int nRectYCount = _scene->height() / rect_size;
    int nRectFullCount = nRectXCount * nRectYCount;

    vec2DTiles vecTiles = makeMapTiles( nRectXCount, nRectYCount, OF_TILE_ALGORITHM_V1 );

    int nOffsetX = 0;

    for( int idx = 0; idx < nRectXCount; idx++, nOffsetX += rect_size )
    {
        for( int idx2 = 0; idx2 < nRectYCount; idx2++ )
        {
            qreal rectX = rect_size * idx;
            qreal rectY = rect_size * idx2;

            QColor tileColor = getTileColor( vecTiles[ idx ][ idx2 ].eTile );

            QGraphicsRectItem* rectItem = scene->addRect( rectX, rectY, rect_size, rect_size, QPen( Qt::white ), QBrush( tileColor ) );
            rectItem->setParentItem( _pixmap );

            if( vecTiles[ idx ][ idx2 ].eObject == OF_OBJECT_VILLAGE )
                rectItem->setData( OF_DATA_OBJECT, OF_OBJECT_VILLAGE );

            QPixmap pix( 32, 32 );
            pix.load( ":/OFSimulator/res/village.png" );
            //pix.load( ":/OFSimulator/res/house.png" );

            QGraphicsPixmapItem* village = scene->addPixmap( pix );
            village->setScale( 0.5 );
            village->setPos( rectX + rect_size / 2 - pix.width() / 4 , rectY + rect_size / 2 - pix.height() / 4 );
            village->setParentItem( rectItem );
            village->setZValue( 10 );
        }
    }

    //QGraphicsEllipseItem* dot = scene->addEllipse( 0, 0, 5, 5, QPen(), QBrush( Qt::red ) );
    //dot->setPos( 0, 0 ); // Position of the dot
    //dot->setParentItem( _pixmap );

    //QGraphicsRectItem* squareDot = scene->addRect( 0, 0, 1000, 1000, QPen(), QBrush( "#1e1e1e" ) );
    //squareDot->setPos( 10, 10 ); // Position of the square dot

    /*
    QGraphicsScene* scene = new QGraphicsScene( this );
    scene->setSceneRect( 0, 0, 1000, 1000 );

    _scene = scene;

    QGraphicsEllipseItem* dot = scene->addEllipse( 0, 0, 5, 5, QPen(), QBrush( Qt::red ) );
    dot->setPos( 100, 100 ); // Position of the dot

    QGraphicsRectItem* squareDot = scene->addRect( 50, 50, 5, 5, QPen(), QBrush( Qt::blue ) );
    squareDot->setPos( 100, 100 ); // Position of the square dot

    ui.gvMap->setBackgroundBrush( Qt::yellow );
    ui.gvMap->setScene( scene );

    ui.gvMap->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    ui.gvMap->setRenderHint( QPainter::Antialiasing );
    ui.gvMap->setCacheMode( QGraphicsView::CacheBackground );
    ui.gvMap->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );

    ui.gvMap->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setSceneRect( 0, 0, 1000, 1000 );

    //ui.gvMap->setFocusPolicy( Qt::StrongFocus );
    ui.gvMap->installEventFilter( this );
    ui.gvMap->viewport()->setMouseTracking( true );
    ui.gvMap->viewport()->installEventFilter( this );
    */
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
            case Qt::Key_Left:
            case Qt::Key_A:
            _pixmap->moveBy( 50, 0 );
            break;
            case Qt::Key_D:
            case Qt::Key_Right:
            _pixmap->moveBy( -50, 0 );
            break;
            case Qt::Key_W:
            case Qt::Key_Up:
            _pixmap->moveBy( 0, 50 );
            break;
            case Qt::Key_S:
            case Qt::Key_Down:
            _pixmap->moveBy( 0, -50 );
            break;
            case Qt::Key_Control:
            ui.gvMap->setDragMode( QGraphicsView::ScrollHandDrag );
            break;
            default:
            break;
        }
    }
    else if( eventType == QEvent::KeyRelease )
    {
        QKeyEvent* keyEvent = static_cast< QKeyEvent* >( event );

        switch( keyEvent->key() )
        {
            case Qt::Key_Control:
            ui.gvMap->setDragMode( QGraphicsView::NoDrag );
            break;
            default:
            break;
        }
    }
    else if( eventType == QEvent::Wheel )
    {
        QWheelEvent* wheelEvent = static_cast< QWheelEvent* >( event );
        ui.gvMap->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

        double scaleFactor = 0.25;

        double dT = scaleFactor + _scaleFactor;

        if( wheelEvent->angleDelta().y() > 0 )
        {
            if( dT >= 3.0f )
                return false;

            _scaleFactor += scaleFactor;
            qDebug() << "Zoom in" << _scaleFactor;
            _pixmap->setScale( _scaleFactor );

            return false;
        }
        else
        {
            if( dT <= 1.0f )
                return false;

            _scaleFactor -= scaleFactor;
            qDebug() << "Zoom out" << _scaleFactor;
            _pixmap->setScale( _scaleFactor );

            return false;
        }
    }
    else if( eventType == QEvent::MouseButtonPress )
    {
        QMouseEvent* mouseEvent = static_cast< QMouseEvent* >( event );

        if( mouseEvent->button() == Qt::LeftButton )
        {
            auto mapPoint = _pixmap->mapFromScene( mouseEvent->pos() );

            QGraphicsItem* item = _scene->itemAt( mapPoint, _pixmap->transform() );

            auto pItem = dynamic_cast< QGraphicsRectItem* >( item );

            if( pItem != NULLPTR )
            {
                int a = 0;
                //a = pItem->data( OF_DATA_MAP ).toInt();
            }
        }
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
    int nVillageMin;
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