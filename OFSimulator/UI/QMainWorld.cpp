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
    ui.gvMap->setSceneRect( 0, 0, 200, 200 );

    //ui.gvMap->setFocusPolicy( Qt::StrongFocus );
    ui.gvMap->installEventFilter( this );
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
            ui.gvMap->scroll( 10, 0 );
            break;
            case Qt::Key_Right:
            ui.gvMap->scroll( -10, 0 );
            break;
            case Qt::Key_Up:
            ui.gvMap->scroll( 0, 10 );
            break;
            case Qt::Key_Down:
            ui.gvMap->scroll( 0, -10 );
            break;
            default:
            break;
        }
    }
    else if( eventType == QEvent::Wheel )
    {
        QWheelEvent* wheelEvent = static_cast< QWheelEvent* >( event );

        if( wheelEvent->angleDelta().y() > 0 )
        {
            _scene->setSceneRect( _scene->sceneRect().x(), _scene->sceneRect().y(),
                                  _scene->sceneRect().width() * 1.1, _scene->sceneRect().height() * 1.1 );
            ui.gvMap->scale( 1.1, 1.1 );
        }
        else
        {
            ui.gvMap->scale( 0.9, 0.9 );
            _scene->setSceneRect( _scene->sceneRect().x(), _scene->sceneRect().y(),
                                  _scene->sceneRect().width() * 0.9, _scene->sceneRect().height() * 0.9 );
        }
    }

    return QWidget::eventFilter( watched, event );
}

