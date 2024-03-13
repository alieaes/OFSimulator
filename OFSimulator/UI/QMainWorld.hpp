#pragma once

#ifndef __HDR_OF_MAIN_WORLD__
#define __HDR_OF_MAIN_WORLD__

#include "ui_OFSimulator.h"

#include "def/OFWorlds.hpp"

enum eTileAlgorithm
{
    OF_TILE_ALGORITHM_V1 = 0,
    OF_TILE_ALGORITHM_V2 = 1
};

const int OF_DATA_OBJECT  = 10000;

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
    vec2DTiles                               makeMapTiles( int nX, int nY, eTileAlgorithm eAlgorithm );
    void                                     makeMapTilesV1( int nX, int nY, vec2DTiles& vec2DTiles );
    void                                     makeMapTilesV2( int nX, int nY, vec2DTiles& vec2DTiles );

    QColor                                   getTileColor( eTiles eTile );

    eTiles                                   getRecommendTile( eTiles eTile, QPoint pCurrent, QPoint pMax, int nRandom );

private:
    Ui::OFSimulatorClass                     ui;

    QGraphicsScene*                          _scene;
    QGraphicsPixmapItem*                     _pixmap;
    double                                   _scaleFactor;
    int                                      _moveSpeed;

    QPoint                                   _pLastPos;
};

#endif
