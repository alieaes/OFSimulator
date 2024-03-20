#include "stdafx.h"

#include "Module/cCharacterModule.hpp"

#include "Base/ExtConfig.hpp"

// NPC가 아닌 플레이어가 설정한 캐릭터. 따로 모듈로 두도록 함.

cCharacterModule::cCharacterModule( QObject* parent )
{
    _parent = dynamic_cast< OFSimulator* >( parent );
}

cCharacterModule::~cCharacterModule()
{
}

bool cCharacterModule::NotifyModule( const XString& sNotifyJobs )
{
    return true;
}

bool cCharacterModule::Save()
{
    bool isSuccess = false;

    return isSuccess;
}

bool cCharacterModule::Load()
{
    bool isSuccess = false;

    return isSuccess;
}

void cCharacterModule::MakeCharacter()
{
    auto rand = Ext::Util::cRandom< int >( 0, 10000 );

    stOFCharacter info;

    info.stInfo.sUUID = Ext::Util::CreateGUID( Ext::CASE_TYPE_UPPER );

    int nGenderRand = rand.Generate();

    if( nGenderRand < 4500 )
        info.stInfo.eGender = OF_GENDER_MALE;
    else if( nGenderRand >= 4500 && nGenderRand < 9000 )
        info.stInfo.eGender = OF_GENDER_FEMALE;
    else if( nGenderRand >= 9000 )
        info.stInfo.eGender = OF_GENDER_ASEXUAL;

    info.stInfo.sFirstName = createFirstName( info.stInfo.eGender );
    info.stInfo.sSecondName = createSecondName( info.stInfo.eGender );

    info.stInfo.uINT = 10;
    info.stInfo.uVIT = 10;

    _char.insert( info.stInfo.sUUID, info );

    RefreshHP( info.stInfo.sUUID );
    RefreshSP( info.stInfo.sUUID );

    QMetaObject::invokeMethod( _parent->GetCharacter(), "WriteCharacter", Q_ARG( QString, info.stInfo.sUUID.toQString() ) );
}

void cCharacterModule::SetCharacterCoord( const QString& sUUID, QPoint pCoord )
{
    if( IsExistCharacter( sUUID ) == true )
    {
        stOFCharacter* info = GetCharacter( sUUID );

        if( info != NULLPTR )
            info->pCoord = pCoord;
    }
}

void cCharacterModule::SetCharacterColor( const QString& sUUID, const QString& sColor )
{
    if( IsExistCharacter( sUUID ) == true )
    {
        stOFCharacter* info = GetCharacter( sUUID );

        if( info != NULLPTR )
            info->sColor = sColor;
    }
}

stOFCharacter* cCharacterModule::GetCharacter( const QString& sUUID )
{
    stOFCharacter* info = NULLPTR;

    if( IsExistCharacter( sUUID ) == true )
    {
        QReadLocker lck( &_lckCharacter );
        info = &_char[ sUUID ];
    }

    return info;
}

bool cCharacterModule::IsExistCharacter( const QString& sUUID )
{
    QReadLocker lck( &_lckCharacter );

    return _char.contains( sUUID ) == true;
}

void cCharacterModule::RefreshHP( const QString& sUUID )
{
    stOFCharacter* info = GetCharacter( sUUID );

    if( info != NULLPTR )
    {
        if( info->stInfo.uCurrentHP == 0 && info->stInfo.uMaximumHP == 0 )
        {
            unsigned int uNewMaxHP = info->stInfo.uVIT * 10;
            info->stInfo.uCurrentHP = uNewMaxHP;
            info->stInfo.uMaximumHP = uNewMaxHP;
        }
        else
        {
            double dRatio = ( double )info->stInfo.uCurrentHP / ( double )info->stInfo.uMaximumHP;
            unsigned int uNewMaxHP = info->stInfo.uVIT * 10;

            double dNewHP = uNewMaxHP * dRatio;

            info->stInfo.uCurrentHP = dNewHP;
            info->stInfo.uMaximumHP = uNewMaxHP;
        }
    }
}

void cCharacterModule::RefreshSP( const QString& sUUID )
{
    stOFCharacter* info = GetCharacter( sUUID );

    if( info != NULLPTR )
    {
        if( info->stInfo.uCurrentSP == 0 && info->stInfo.uMaximumSP == 0 )
        {
            unsigned int uNewMaxSP = info->stInfo.uINT * 10;
            info->stInfo.uCurrentSP = uNewMaxSP;
            info->stInfo.uMaximumSP = uNewMaxSP;
        }
        else
        {
            double dRatio = ( double )info->stInfo.uCurrentSP / ( double )info->stInfo.uMaximumSP;
            unsigned int uNewMaxSP = info->stInfo.uINT * 10;

            double dNewSP = uNewMaxSP * dRatio;

            info->stInfo.uCurrentSP = dNewSP;
            info->stInfo.uMaximumSP = uNewMaxSP;
        }
    }
}

bool cCharacterModule::moduleInit()
{
    return true;
}

bool cCharacterModule::moduleStart()
{
    return true;
}

bool cCharacterModule::moduleStop()
{
    return true;
}

bool cCharacterModule::moduleFinal()
{
    return true;
}

XString cCharacterModule::createCharacterName( const eOFGender& eGender, const XString& sFirstName /*= ""*/ )
{
    XString sRet;

    XString sMakeFirst;

    if( sFirstName.IsEmpty() == true )
        sMakeFirst = createFirstName( eGender );

    XString sMakeSecond = createSecondName( eGender );

    return sRet;
}

XString cCharacterModule::createFirstName( const eOFGender& eGender )
{
    QStringList lstFirstName;

    auto fnFirstName = [&lstFirstName]
    {
        lstFirstName << "금" << "왕" << "장" << "정" << "군" << "탁" << "만" << "선" << "유" << "언";
        lstFirstName << "양" << "나" << "이" << "신" << "공" << "육" << "제갈" << "심" << "공손";
        lstFirstName << "고" << "위" << "남궁" << "적" << "서문" << "한" << "구";
    };

    switch( eGender )
    {
        case OF_GENDER_MALE:
        {
            fnFirstName();
        } break;
        case OF_GENDER_FEMALE:
        {
            fnFirstName();
        } break;
        case OF_GENDER_NONE:
        case OF_GENDER_ASEXUAL:
        default:
        {
            fnFirstName();
        } break;
    }

    auto rand = Ext::Util::cRandom< int >( 0, lstFirstName.size() - 1 );

    return lstFirstName[ rand.Generate() ];
}

XString cCharacterModule::createSecondName( const eOFGender& eGender )
{
    QStringList lstSecondName;

    auto fnSecondNameForMale = [&lstSecondName] {
        lstSecondName << "평" << "공운" << "윤" << "운" << "청" << "정청" << "정" << "명" << "환" << "산";
        lstSecondName << "백" << "천" << "만" << "십" << "일" << "단" << "단야" << "금룡" << "은룡" << "동룡";
        lstSecondName << "승지" << "척" << "유성" << "철" << "서문" << "천승" << "단룡" << "소홍" << "위룡";
        lstSecondName << "천풍" << "풍" << "개" << "풍개" << "룡" << "겸" << "극" << "무극" << "종" << "종보";
    };

    auto fnSecondNameForFemale = [&lstSecondName] {
        lstSecondName << "미미" << "예린" << "련" << "소소" << "설" << "연" << "설연" << "향" << "향아" << "소";
        lstSecondName << "소예" << "예린" << "예림" << "유란" << "소연" << "소현" << "이설" << "설" << "여은";
        lstSecondName << "소보" << "비영" << "가화" << "서애" << "하원" << "나은" << "은";
    };

    switch( eGender )
    {
        case OF_GENDER_MALE:
        {
            fnSecondNameForMale();
        } break;
        case OF_GENDER_FEMALE:
        {
            fnSecondNameForFemale();
        } break;
        case OF_GENDER_NONE:
        case OF_GENDER_ASEXUAL:
        default:
        {
            fnSecondNameForMale();
            fnSecondNameForFemale();
        } break;
    }

    auto rand = Ext::Util::cRandom< int >( 0, lstSecondName.size() - 1 );

    return lstSecondName[ rand.Generate() ];
}

void cCharacterModule::makeCharacterConcept( stOFCharacter& info )
{
    // 힘캐
    // 민캐
}
