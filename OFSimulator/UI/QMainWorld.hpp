#pragma once

#ifndef __HDR_OF_MAIN_WORLD__
#define __HDR_OF_MAIN_WORLD__

#include "ui_OFSimulator.h"

#include "def/OFWorlds.hpp"


class QMainWorld : public QWidget
{
    Q_OBJECT
public:
    QMainWorld( QWidget* parent, Ui::OFSimulatorClass _ui );
    ~QMainWorld();

    void                                     Init();

public slots:
    bool                                     eventFilter( QObject* watched, QEvent* event ) override;

protected:

private:
    vec2DTiles                               makeMapTiles( int nX, int nY );

private:
    Ui::OFSimulatorClass                     ui;

    QGraphicsScene*                          _scene;
    QGraphicsPixmapItem*                     _pixmap;
    double                                   _scaleFactor;
    int                                      _moveSpeed;
};

#endif