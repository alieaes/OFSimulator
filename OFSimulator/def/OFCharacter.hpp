#pragma once

#ifndef __HDR_OF_CHARACTER__
#define __HDR_OF_CHARACTER__

#include "String/EXString.hpp"
#include <QDateTime>

struct stSecretStatus
{
    // 기분
    short                   rTension               = 0;

    // 친화력(Friendly)
    short                   rFRD                   = 0;

    // 성장치(Bless)
    short                   rBLS                   = 0;

    // 명성
    unsigned int            uFame                  = 0;

    // 신의
    unsigned int            uBelief                = 0;

    // 성향
    unsigned int            uNature                = 0;

    stSecretStatus()
    {
        ClearSecretStatus();
    }

    void ClearSecretStatus()
    {
        rTension = rFRD = rBLS = uFame = uBelief = uNature = 0;
    }
};

// 속성과 내성을 같이 담당함.
struct stcharacterAttr
{
    unsigned int            uFire                  = 0;
    unsigned int            uWater                 = 0;
    unsigned int            uIce                   = 0;
    unsigned int            uElec                  = 0;
    unsigned int            uPoison                = 0;
    unsigned int            uEarth                 = 0;
    unsigned int            uWind                  = 0;

    stcharacterAttr()
    {
        ClearAttr();
    }

    void ClearAttr()
    {
        uFire = uWater = uIce = uElec = uPoison = uEarth = uWind = 0;
    }
};

struct stCharacterQuality
{
    // 검법 자질
    unsigned int             uBlade                = 0;

    // 도법 자질
    unsigned int             uSword                = 0;

    // 창법 자질
    unsigned int             uSpear                = 0;

    // 봉법 자질
    unsigned int             uPole                 = 0;

    // 장법 자질
    unsigned int             uPalm                 = 0;

    // 권법 자질
    unsigned int             uFist                 = 0;

    // 지법 자질
    unsigned int             uFinger               = 0;

    // 암기(투척) 자질
    unsigned int             uProjectile           = 0;

    stCharacterQuality()
    {
        ClearQuality();
    }

    void ClearQuality()
    {
        uBlade = uSword = uSpear = uPole = uPalm = uFist = 0;
        uFinger = uProjectile = 0;
    }
};

struct stInjury
{
    bool                      isTrauma             = false;
    unsigned short            rTraumaRecovery      = 0;

    bool                      isInjury             = false;
    unsigned short            rInjuryRecovery      = 0;

    bool                      isControlLose        = false;
    unsigned short            rControlLoseRecovery = 0;

    bool                      isBlooding           = false;
    unsigned short            rBloodingRecovery    = 0;

    bool                      isBurn               = false;
    unsigned short            rBurnRecovery        = 0;

    bool                      isFrostbite          = false;
    unsigned short            rFrostbiteRecovery   = 0;

    bool                      isShork              = false;
    unsigned short            rShorkRecovery       = 0;

    stInjury()
    {
        ClearInjury();
    }

    void ClearInjury()
    {
        isTrauma = isInjury = isControlLose = isBlooding = isBurn = isFrostbite = isShork = false;
        rTraumaRecovery = rInjuryRecovery = rControlLoseRecovery = rBloodingRecovery = 0;
        rBurnRecovery = rFrostbiteRecovery = rShorkRecovery = 0;
    }
};

enum eOFPurity
{
    OF_PURITY_NONE   = 0,

    // 평범
    OF_PURITY_NORMAL = 1,

    // 순수한 내공
    OF_PURITY_PURE   = 2,

    // 파괴적
    OF_PURITY_POWER  = 4,

    // 불순한 내공
    OF_PURITY_IMPURE = 8,

    // 사파?
    OF_PURITY_BAD    = 16,

    // 마공
    OF_PURITY_DARK   = 32
};

EXT_ENUM_START( eOFPurity )
EXT_ENUM_ADD( OF_PURITY_NONE, "없음" )
EXT_ENUM_ADD( OF_PURITY_NONE, "평범" )
EXT_ENUM_ADD( OF_PURITY_NONE, "순수" )
EXT_ENUM_ADD( OF_PURITY_NONE, "파괴" )
EXT_ENUM_ADD( OF_PURITY_NONE, "불순" )
EXT_ENUM_ADD( OF_PURITY_NONE, "오염" )
EXT_ENUM_ADD( OF_PURITY_NONE, "마공" )
EXT_ENUM_END( eOFPurity )

struct stDefStatus
{
    unsigned int              uCurrentHP           = 0;
    unsigned int              uMaximumHP           = 0;

    unsigned int              uCurrentSP           = 0;
    unsigned int              uMaximumSP           = 0;

    // 내공
    unsigned int              uOFPower             = 0;
    eOFPurity                 ePurity              = OF_PURITY_NONE;

    // 인물 기본 공격력과 방어력, 실제 대미지는 계산식으로.
    unsigned int              uDamage              = 0;
    unsigned int              uDefence             = 0;

    // 힘 : 공격력
    unsigned int              uSTR                 = 0;

    // 민첩 : 공격속도
    unsigned int              uDEX                 = 0;

    // 민첩 : 회피
    unsigned int              uAGI                 = 0;
    
    // 민첩 : 이동속도
    unsigned int              uMOV                 = 0;

    // 지능 : 내공(SP)
    unsigned int              uINT                 = 0;

    // 체력 : 체력(HP)
    unsigned int              uVIT                 = 0;

    // 운 : 행운
    unsigned int              uLUK                 = 0;

    // 오성
    unsigned int              uWIZ                 = 0;

    // 정신력
    unsigned int              uWILL                = 0;

    // 건강
    unsigned int              uCON                 = 0;

    // 매력
    unsigned int              uCHA                 = 0;

    // 기감(시야)
    unsigned int              uSight               = 0;

    stDefStatus()
    {
        ClearDefStatus();
    }

    void ClearDefStatus()
    {
        uCurrentHP = uCurrentSP = 0;

        uMaximumHP = uMaximumSP = 0;

        uOFPower = 0;
        ePurity = OF_PURITY_NONE;

        uDamage = uDefence = 0;

        uSTR = uDEX = uAGI = uMOV = uINT = uVIT = 0;
        uLUK = uWIZ = uWILL = uCON = uCHA = uSight = 0;
    }
};

enum eOFEventLevel
{
    // 세계에서 중복될 수 있음
    OF_EVENT_NORMAL    = 0,
    // 세계에서 딱 하나의 이벤트
    OF_EVENT_UNIQUE    = 1,

    // 상기 이벤트에 대한 등급
    OF_EVENT_COMMON    = 2,
    OF_EVENT_RARE      = 4,
    OF_EVENT_EPIC      = 8,
    OF_EVENT_LEGENDARY = 16,
    OF_EVENT_MYTHIC    = 32,
};

struct stCharacterHistory
{
    QDateTime                 dtEvent;
    XString                   sEvent;
    eOFEventLevel             eLevel;
};

typedef QVector< stCharacterHistory > vecCharactorHistory;

enum eOFGender
{
    OF_GENDER_NONE     = 0,
    OF_GENDER_MALE     = 1,
    OF_GENDER_FEMALE   = 2,
    OF_GENDER_ASEXUAL  = 3
};

EXT_ENUM_START( eOFGender )
EXT_ENUM_ADD( OF_GENDER_NONE, "없음" )
EXT_ENUM_ADD( OF_GENDER_MALE, "남자" )
EXT_ENUM_ADD( OF_GENDER_FEMALE, "여자" )
EXT_ENUM_ADD( OF_GENDER_ASEXUAL, "무성" )
EXT_ENUM_END( eOFGender )

struct stCharacterInfo : stCharacterHistory, stDefStatus, stInjury, stcharacterAttr, stCharacterQuality, stSecretStatus
{
    XString                    sUUID;

    XString                    sFirstName;
    XString                    sSecondName;

    XString                    sAlias;

    eOFGender                  eGender              = OF_GENDER_NONE;

    short                      rAge                 = 0;
    short                      rLifespan            = 0;

    vecCharactorHistory        vecHistory;

    unsigned int               uMoney               = 0;

    stCharacterInfo()
    {
        ClearChracterInfo();
    }

    void ClearChracterInfo()
    {
        sUUID.clear();
        sFirstName.clear();
        sSecondName.clear();
        sAlias.clear();

        eGender = OF_GENDER_NONE;

        rAge = 0;
        rLifespan = 0;

        vecHistory.clear();

        uMoney = 0;
    }

    void ClearAll()
    {
        ClearChracterInfo();
        ClearDefStatus();
        ClearInjury();
        ClearAttr();
        ClearQuality();
        ClearSecretStatus();
    }
};

struct stOFCharacter
{
    stCharacterInfo            stInfo;

    QPoint                     pCoord = QPoint( 0, 0 );

    QString                    sColor;
};

const int OF_CHARACTER_DATA_UUID         = 20000;

const QSize OF_CHARACTER_LEVEL_1               = QSize( 7, 7 );
const QSize OF_CHARACTER_LEVEL_2               = QSize( 8, 8 );
const QSize OF_CHARACTER_LEVEL_3               = QSize( 9, 9 );
const QSize OF_CHARACTER_LEVEL_4               = QSize( 10, 10 );
const QSize OF_CHARACTER_LEVEL_5               = QSize( 11, 11 );
const QSize OF_CHARACTER_LEVEL_6               = QSize( 12, 12 );

#endif