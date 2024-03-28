#include "stdafx.h"

#include "UI/QGraphicsViewMAP.hpp"

QGraphicsViewMAP::QGraphicsViewMAP( QWidget* parent )
    : QGraphicsView( parent )
{
    this->installEventFilter( this );
    QGraphicsView* parentView = static_cast< QGraphicsView* >( parent );
    setMinimumSize( QSize( 800, 600 ) );
}

QGraphicsViewMAP::~QGraphicsViewMAP()
{
}

bool QGraphicsViewMAP::eventFilter( QObject* object, QEvent* event )
{
    if( event->type() == QEvent::MouseButtonPress )
    {
        qDebug() << "MouseButtonPress";

        QMouseEvent* mouse_event = static_cast< QMouseEvent* >( event );
        // Enter here any button you like
        if( mouse_event->button() == Qt::RightButton )
        {
            // temporarly enable dragging mode
            this->setDragMode( QGraphicsView::DragMode::ScrollHandDrag );
            // emit a left mouse click (the default button for the drag mode)
            QMouseEvent* pressEvent = new QMouseEvent( QEvent::GraphicsSceneMousePress,
                                                       mouse_event->pos(), Qt::MouseButton::LeftButton,
                                                       Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier );

            this->mousePressEvent( pressEvent );
        }
        else if( event->type() == QEvent::MouseButtonRelease )
        {
            qDebug() << "MouseButtonRelease";

            // disable drag mode if dragging is finished
            this->setDragMode( QGraphicsView::DragMode::NoDrag );
        }

        Q_UNUSED( object );
        return false;
    }
    return QGraphicsView::eventFilter( object, event );
}
