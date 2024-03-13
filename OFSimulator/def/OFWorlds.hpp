﻿#pragma once

#ifndef __HDR_OF_WORLDS__
#define __HDR_OF_WORLDS__

#include "String/EXString.hpp"

enum eTiles
{
    OF_TILE_NONE = 0,

    OF_TILE_FLAT = 1,

    OF_TILE_GRASS = 2,
    OF_TILE_MOUNTAIN = 4,
    OF_TILE_CLIFF = 8,

    OF_TILE_COAST = 16,
    OF_TILE_LAKE = 32,

    OF_TILE_HILLS = 64,
    OF_TILE_WOODS = 128,
    OF_TILE_SNOW = 256,
    OF_TILE_VOLCANO = 512,
};

enum eOFObject
{
    OF_OBJECT_NONE    = 0,

    // 마을
    OF_OBJECT_VILLAGE = 1,

    // 문파
    OF_OBJECT_CLAN    = 2,

    // 기연을 위한?
    OF_TITLE_HOUSE    = 3
};

struct stOFTileInfo
{
    eTiles                     eTile              = OF_TILE_NONE;
    eOFObject                  eObject            = OF_OBJECT_NONE;

    QPoint                     pCoord             = QPoint( 0, 0 );

    // 영기
    int                        nEarthPower        = 0;

    stOFTileInfo()
    {
        Clear();
    }

    void Clear()
    {
        eTile = OF_TILE_NONE;
        pCoord = QPoint( 0, 0 );
        nEarthPower = 0;
    }
};

typedef QVector< QVector< stOFTileInfo > > vec2DTiles;

struct stWORLD_INFO
{
    XString                    sWorldName;

    unsigned int               uYears             = 0;
};

#endif