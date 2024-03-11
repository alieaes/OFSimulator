#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OFSimulator.h"
#include "Base/ExtConfig.hpp"
#include "UI/QMakeProfile.hpp"

class OFSimulator : public QMainWindow
{
    Q_OBJECT

public:
    OFSimulator( QWidget* parent = nullptr );
    ~OFSimulator();

public slots:
    void                                      on_btnNewGame_clicked();
    void                                      on_btnContinue_clicked();
    void                                      on_btnExit_clicked();

private:
    Ui::OFSimulatorClass                      ui;
    QMakeProfile*                             _QProfile = NULLPTR;
};
