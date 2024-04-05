#pragma once

#ifndef __HDR_OF_TIME_CONTROLLER__
#define __HDR_OF_TIME_CONTROLLER__

#include "ui_OFSimulator.h"

class QTimeController : public QWidget
{
    Q_OBJECT
public:
    QTimeController( QWidget* parent, Ui::OFSimulatorClass _ui );
    ~QTimeController();

    void                                     SetIcon( QPushButton* btn, const QString& sResFile );
    short                                    GetSpeed() const { return _rSpeed; }
    QString                                  GetSpeedText();
    QString                                  GetSpeedText( unsigned short rSpeed );

public slots:
    bool                                     eventFilter( QObject* watched, QEvent* event ) override;

private:
    void                                     RefreshSpeed();
    void                                     RefreshSpeed( unsigned short rSpeed );

private:
    Ui::OFSimulatorClass                     ui;
    bool                                     _isPause = true;
    unsigned short                           _rSpeed  = 0;
};

#endif