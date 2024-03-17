#pragma once

#ifndef __HDR_OF_UI_CHARACTER__
#define __HDR_OF_UI_CHARACTER__

#include "ui_OFSimulator.h"

#include "def/OFWorlds.hpp"

class QCharacter : public QWidget
{
    Q_OBJECT
public:
    QCharacter( QWidget* parent, Ui::OFSimulatorClass _ui );
    ~QCharacter();

    void                                     Init();

    QString                                  MakeComfortableColor();

public slots:
    Q_INVOKABLE void                         WriteCharacter( QString sUUID );

    //bool                                     eventFilter( QObject* watched, QEvent* event ) override;

protected:

private:

private:
    Ui::OFSimulatorClass                     ui;
};

#endif
