#pragma once

#ifndef __HDR_OF_MAKE_PROFILE_PAGE__
#define __HDR_OF_MAKE_PROFILE_PAGE__

#include "ui_OFSimulator.h"

class QMakeProfile : public QWidget
{
    Q_OBJECT
public:
    QMakeProfile( QWidget* parent, Ui::OFSimulatorClass _ui );
    ~QMakeProfile();

    void                                     Init();

public slots:
    void                                     OnBtnProfileCreateClicked();
    void                                     OnBtnProfileCloseClicked();

private:
    Ui::OFSimulatorClass                     ui;
    int                                      _nPrevIndex = -1;
};

#endif