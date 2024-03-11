#include "stdafx.h"

#include "UI/QMainWorld.hpp"

#include "Base/ExtConfig.hpp"

QMainWorld::QMainWorld( QWidget* parent, Ui::OFSimulatorClass _ui )
    : QWidget( parent )
{
    ui = _ui;
}

QMainWorld::~QMainWorld()
{
}

void QMainWorld::Init()
{
    QGraphicsScene* scene = new QGraphicsScene( this );
    scene->setSceneRect( 0, 0, 1000, 1000 );

    QGraphicsEllipseItem* dot = scene->addEllipse( 0, 0, 5, 5, QPen(), QBrush( Qt::red ) );
    dot->setPos( 100, 100 ); // Position of the dot

    ui.gvMap->setScene( scene );

    ui.gvMap->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    ui.gvMap->setRenderHint( QPainter::Antialiasing );
    ui.gvMap->setCacheMode( QGraphicsView::CacheBackground );
    ui.gvMap->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );

    ui.gvMap->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui.gvMap->setSceneRect( 0, 0, 200, 200 );

    ui.gvMap->setFocusPolicy( Qt::StrongFocus );
}