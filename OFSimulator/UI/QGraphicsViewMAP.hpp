#pragma once

#ifndef __HDR_OF_QGRAPHICS_VIEW_MAP__
#define __HDR_OF_QGRAPHICS_VIEW_MAP__


class QGraphicsViewMAP : public QGraphicsView
{
    Q_OBJECT
public:
    QGraphicsViewMAP( QWidget* parent = nullptr );
    ~QGraphicsViewMAP() override;

    void                                     SetMainWorld();

public slots:
    bool                                     eventFilter( QObject* object, QEvent* event ) override;
    void                                     wheelEvent( QWheelEvent* event ) override;

};

#endif
