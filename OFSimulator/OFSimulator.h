#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_OFSimulator.h"

#include "Base/ExtConfig.hpp"

#include "UI/QMakeProfile.hpp"
#include "UI/QMainWorld.hpp"

class OFSimulator : public QMainWindow
{
    Q_OBJECT

public:
    OFSimulator( QWidget* parent = nullptr );
    ~OFSimulator();

    void                                      WorldStart();

public slots:
    void                                      on_btnNewGame_clicked();
    void                                      on_btnContinue_clicked();
    void                                      on_btnExit_clicked();

protected:

private:
    Ui::OFSimulatorClass                      ui;

    QMakeProfile*                             _QProfile = NULLPTR;
    QMainWorld*                               _World = NULLPTR;

};
