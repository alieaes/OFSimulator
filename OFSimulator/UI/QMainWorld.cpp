#include "stdafx.h"

#include "UI/QMainWorld.hpp"

#include "Base/ExtConfig.hpp"

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
    scene->setSceneRect( 0, 0, ui.gvMap->width() * 2, ui.gvMap->height() * 2 );
    _scene = scene;

    ui.gvMap->setBackgroundBrush( QBrush( "#302f2b" ) );
    ui.gvMap->setScene( scene );
    ui.gvMap->installEventFilter( this );
    ui.gvMap->setMouseTracking( true );
    ui.gvMap->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    ui.gvMap->setCacheMode( QGraphicsView::CacheBackground );

    _scaleFactor = 1.0f;

    QPixmap pix( _scene->width(), _scene->height() );
    pix.fill( QColor( "#1e1e1e" ) );
    _pixmap = scene->addPixmap( pix );
    _pixmap->setPos( 0, 0 );

    qreal rect_size = 50; // 네모의 크기

    qDebug() << "width=" << _scene->width() << "|height=" << _scene->height();

    int nRectXCount = _scene->width() / rect_size;
    int nRectYCount = _scene->height() / rect_size;
    int nRectFullCount = nRectXCount * nRectYCount;

    makeMapTiles( nRectXCount, nRectYCount );

    int nOffsetX = 0;

    for( int idx = 0; idx < nRectXCount; idx++, nOffsetX += rect_size )
    {
        for( int idx2 = 0; idx2 < nRectYCount; idx2++ )
        {
            qreal rectX = rect_size * idx;
            qreal rectY = rect_size * idx2;
            QGraphicsRectItem* rectItem = scene->addRect( rectX, rectY, rect_size, rect_size, QPen( Qt::white ), QBrush( "#ba9568" ) );
            rectItem->setParentItem( _pixmap );
        }
    }

    /*// 시계 방향으로 네모를 그리기
    for( int i = 0; i < 8; ++i )
    {
        // 네모를 그리고 Scene에 추가
        
        rectItem->setParentItem( _pixmap );

        // 네모를 시계 방향으로 회전시킴

        // 다음 네모의 위치를 계산
        center_x += rect_size;
    }*/

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

bool QMainWorld::eventFilter(QObject* watched, QEvent* event)
{
    QString objName = watched->objectName();
    qDebug() << objName;
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
                return QWidget::eventFilter( watched, event );

            _scaleFactor += scaleFactor;
            qDebug() << "Zoom in" << _scaleFactor;
            _pixmap->setScale( _scaleFactor );
        }
        else
        {
            if( dT <= 1.0f )
                return QWidget::eventFilter( watched, event );

            _scaleFactor -= scaleFactor;
            qDebug() << "Zoom out" << _scaleFactor;
            _pixmap->setScale( _scaleFactor );
            //ui.gvMap->scale( 1 / _scaleFactor, 1 / _scaleFactor );
        }

        /*
        double scaleFactorDelta = 0.1;
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        double numSteps = numDegrees / 15.0; // 15 degrees per step

        double scaleFactorMultiplier = 1.0 + numSteps * scaleFactorDelta;

        double dT = scaleFactor * scaleFactorMultiplier;

        if( dT > 3.0f )
        {
            scaleFactor = 3.0f;
            return QWidget::eventFilter( watched, event );
        }

        if( dT < 1.0f )
        {
            scaleFactor = 1.0f;
            return QWidget::eventFilter( watched, event );
        }

        if( scaleFactorMultiplier > 0 )
        {
            scaleFactor *= scaleFactorMultiplier;
            _scene->setSceneRect( _scene->sceneRect().adjusted(
                - _scene->sceneRect().width() * scaleFactorMultiplier,
                - _scene->sceneRect().height() * scaleFactorMultiplier,
                _scene->sceneRect().width() * scaleFactorMultiplier,
                _scene->sceneRect().height() * scaleFactorMultiplier ) );

            //ui.gvMap->scale( scaleFactorMultiplier, scaleFactorMultiplier );
        }
        */

        /*
        QPointF target_viewport_pos = wheelEvent->position();
        QPointF target_scene_pos = ui.gvMap->mapToScene( wheelEvent->position().toPoint() );
        ui.gvMap->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

        if( wheelEvent->angleDelta().y() > 0 )
        {
            ui.gvMap->scale( 1.1, 1.1 );
        }
        else
        {
            ui.gvMap->scale( 0.9, 0.9 );
        }
        */
    }

    return QWidget::eventFilter( watched, event );
}

vec2DTiles QMainWorld::makeMapTiles( int nX, int nY )
{
    vec2DTiles vec2DTiles( nX, QVector< stOFTileInfo >( nY, stOFTileInfo() ) );

    QPoint pCenter( nX / 2, nY / 2 );
    QPoint pMax( nX, nY );
    QPoint pMin( 0, 0 );

    qDebug() << "X=" << nX << "|Y=" << nY;

    for( auto vecX : vec2DTiles )
    {
        QString s;
        for( auto data : vecX )
            s += QString::number( ( int )data.eTile ) + ", ";

        s.chop( 2 );
        qDebug() << s;
    }

    /*
     * 나중에 옵션으로 넣어도 되고??
     */

    // Flat -> Grass 변환 확률
    int nFlatToGrass = 2000;

    // 센터는 평지로
    //vec2DTiles[ pCenter.x() ][ pCenter.y() ] = OF_TILE_FLAT;

    // 센터 상하좌우 채우기


    return vec2DTiles;
}

