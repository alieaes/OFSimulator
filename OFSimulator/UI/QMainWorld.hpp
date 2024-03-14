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

const int OF_DATA_TILE_INFO    = 10000;
const int OF_DATA_OBJECT       = 10001;

const int OF_VILLAGE_MIN       = 100;
const int OF_VILLAGE_MAX       = 80;

const int OF_HOUSE_MIN         = 400;
const int OF_HOUSE_MAX         = 300;

const int OF_CLAN_MIN          = 150;
const int OF_CLAN_MAX          = 100;

class QMainWorld : public QWidget
{
    Q_OBJECT
public:
    QMainWorld( QWidget* parent, Ui::OFSimulatorClass _ui );
    ~QMainWorld();

    void                                     Init();

    QSize                                    GetPixmapSize();
    QGraphicsPixmapItem*                     GetPixmap();
    QGraphicsScene*                          GetScene();

public slots:
    bool                                     eventFilter( QObject* watched, QEvent* event ) override;

protected:

private:
    vec2DTiles                               makeMapTiles( int nX, int nY, eTileAlgorithm eAlgorithm );
    void                                     makeMapTilesV1( int nX, int nY, vec2DTiles& vec2DTiles );
    void                                     makeMapTilesV2( int nX, int nY, vec2DTiles& vec2DTiles );

    void                                     makeObject( vec2DTiles& vec2DTiles );
    void                                     makeObjectImpl( vec2DTiles& vec2DTiles, eOFObject eObject, int nMakeObjectToLine );

    int                                      getObjectPercentage( eOFObject eObject, eTiles eTile );
    QColor                                   getTileColor( eTiles eTile );
    QString                                  getTileName( eTiles eTile );

    eTiles                                   getRecommendTile( eTiles eTile, QPoint pCurrent, QPoint pMax, int nRandom );

private:
    Ui::OFSimulatorClass                     ui;

    vec2DTiles                               _vecTiles;

    QGraphicsScene*                          _scene;
    QGraphicsPixmapItem*                     _pixmap;
    double                                   _scaleFactor;
    int                                      _moveSpeed;

    QPoint                                   _pLastPos;
};

#endif
