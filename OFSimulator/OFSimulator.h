﻿#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_OFSimulator.h"

#include "Base/ExtConfig.hpp"
#include "Base/Singleton.hpp"

#include "UI/QCharacter.hpp"
#include "UI/QMakeProfile.hpp"
#include "UI/QMainWorld.hpp"

class OFSimulator : public QMainWindow
{
    Q_OBJECT

public:
    OFSimulator( QWidget* parent = nullptr );
    ~OFSimulator();

    void                                      WorldStart();
    void                                      CharacterStart();

    QMainWorld*                               GetWorld() { return _World; }
    QCharacter*                               GetCharacter() { return _Character; }

    void                                      WorldInit();

public slots:
    void                                      on_btnNewGame_clicked();
    void                                      on_btnContinue_clicked();
    void                                      on_btnExit_clicked();

    void                                      CharacterViewDoubleClicked( const QListWidgetItem* item );

    void                                      CharacterViewClear();

protected:

private:
    Ui::OFSimulatorClass                      ui;

    QMakeProfile*                             _QProfile  = NULLPTR;
    QMainWorld*                               _World     = NULLPTR;
    QCharacter*                               _Character = NULLPTR;
};