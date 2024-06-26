﻿#pragma once

#ifndef __HDR_OF_MAIN_WORLD__
#define __HDR_OF_MAIN_WORLD__

#include "QGraphicsViewMAP.hpp"
#include "ui_OFSimulator.h"

#include "def/OFWorlds.hpp"

enum eTileAlgorithm
{
    OF_TILE_ALGORITHM_V1 = 0,
    OF_TILE_ALGORITHM_V2 = 1
};

const int OF_MAP_DATA_TILE_INFO          = 10000;
const int OF_MAP_DATA_OBJECT             = 10001;
const int OF_MAP_DATA_PARENT_TILE        = 10002;

const int OF_VILLAGE_MIN                 = 100;
const int OF_VILLAGE_MAX                 = 80;

const int OF_HOUSE_MIN                   = 400;
const int OF_HOUSE_MAX                   = 300;

const int OF_CLAN_MIN                    = 150;
const int OF_CLAN_MAX                    = 100;

const qreal OF_RECT_SIZE                 = 60;

const int OF_PIXMAP_WIDTH                = 3600;
const int OF_PIXMAP_HEIGHT               = 3600;

class cWorldDateTime;

class QMainWorld : public QWidget
{
    Q_OBJECT
public:
    QMainWorld( QWidget* parent, Ui::OFSimulatorClass _ui );
    ~QMainWorld();

    void                                     Init();
    void                                     InitOld();

    QSize                                    GetPixmapSize();
    QPoint                                   GetMovealeSize();

    QGraphicsPixmapItem*                     GetPixmap();
    QGraphicsScene*                          GetScene();
    stWORLD_INFO                             GetWorldInfo();
    cWorldDateTime*                          GetWorldTime() const { return _worldTime; }

    void                                     ShowClickedItem( QMouseEvent* mouseEvent );
    void                                     WheelEvent( QWheelEvent* wheelEvent );

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
    QString                                  getObjectName( eOFObject eObject );

    eTiles                                   getRecommendTile( eTiles eTile, QPoint pCurrent, QPoint pMax, int nRandom );

private:
    Ui::OFSimulatorClass                     ui;

    vec2DTiles                               _vecTiles;

    QGraphicsViewMAP*                        _gvMap           = NULLPTR;
    QGraphicsScene*                          _scene           = NULLPTR;
    QGraphicsPixmapItem*                     _pixmap          = NULLPTR;
    double                                   _scaleFactor     = 1.0f;
    int                                      _moveSpeed       = 0;

    QPoint                                   _pLastPos;

    stWORLD_INFO                             _worldInfo;

    cWorldDateTime*                          _worldTime       = NULLPTR;
};


class cWorldDateTime : public QWidget
{
    Q_OBJECT

public:
    cWorldDateTime( Ui::OFSimulatorClass _ui );
    ~cWorldDateTime();

    void                                     SpendYear();
    void                                     SpendMonth();
    void                                     SpendDay();

    void                                     SpendHour();
    void                                     SpendMinute();
    void                                     SpendSecond();

private:
    void                                     RefreshLabel();

    Ui::OFSimulatorClass                     ui;

    short                                    _rYears      = 0;
    short                                    _rMonth      = 1;
    short                                    _rDays       = 1;

    short                                    _rHour       = 0;
    short                                    _rMinute     = 0;
    short                                    _rSecond     = 0;
};

#endif
