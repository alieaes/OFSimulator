#include "stdafx.h"

#include "UI/QGraphicsViewMAP.hpp"

#include "OFSimulator.h"
#include "Module/cCharacterModule.hpp"
#include "Module/EXModuleManager.hpp"

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
        QMouseEvent* mouseEvent = static_cast< QMouseEvent* >( event );

        if( mouseEvent->button() == Qt::LeftButton )
        {
            QPoint p = mouseEvent->pos();

            p.setX( p.x() + horizontalScrollBar()->value() );
            p.setY( p.y() + verticalScrollBar()->value() );

            auto spCharacterModule = Ext::Module::GetModule< cCharacterModule >( L"CHARACTER" );
            OFSimulator* pOF = spCharacterModule->GetParent();

            if( pOF == NULLPTR )
                return true;

            pOF->GetWorld()->ShowClickedItem( mouseEvent );
        }
        else if( mouseEvent->button() == Qt::RightButton )
        {
            // temporarly enable dragging mode
            this->setDragMode( QGraphicsView::DragMode::ScrollHandDrag );
            // emit a left mouse click (the default button for the drag mode)
            QMouseEvent* pressEvent = new QMouseEvent( QEvent::GraphicsSceneMousePress,
                                                       mouseEvent->pos(), Qt::MouseButton::LeftButton,
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
    else if( event->type() == QEvent::Wheel )
    {
        event->ignore();
        qDebug() << "Wheel";
        QMouseEvent* mouseEvent = static_cast< QMouseEvent* >( event );
        return false;
    }

    return QGraphicsView::eventFilter( object, event );
}

void QGraphicsViewMAP::wheelEvent( QWheelEvent* event )
{
    qDebug() << "Wheel2";

    auto spCharacterModule = Ext::Module::GetModule< cCharacterModule >( L"CHARACTER" );
    OFSimulator* pOF = spCharacterModule->GetParent();

    if( pOF == NULLPTR )
        return;

    pOF->GetWorld()->WheelEvent( event );

    //QGraphicsView::wheelEvent( event );
}
