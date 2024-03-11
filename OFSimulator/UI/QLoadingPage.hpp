#pragma once

#ifndef __HDR_OF_LOADING_PAGE__
#define __HDR_OF_LOADING_PAGE__

#include <ui_OFSimulator.h>

enum eLoadingType
{
    OF_LOADING_TYPE_NONE      = 0,
    OF_LOADING_TYPE_NEW_GAME  = 1,
    OF_LOADING_TYPE_CONTINUE  = 2
};

class QLoadingPage : public QWidget
{
    Q_OBJECT

public:
    QLoadingPage( QWidget* parent, Ui::OFSimulatorClass _ui, eLoadingType type );
    ~QLoadingPage();

    void                                     Init();

    void                                     Start();
    void                                     Stop();

    void                                     SetLoadingGIF( const QString& sFileRes );
    void                                     SetMaximumPercent( unsigned int uMax );

private:
    void                                     mainWorker();

    QString                                  getTextNewGame();

private:
    Ui::OFSimulatorClass                     ui;

    bool                                     _isStop          = false;

    eLoadingType                             _eLoadingType    = OF_LOADING_TYPE_NONE;

    int                                      _nPrevIndex      = -1;
    unsigned int                             _uCurrentPercent = 0;
    unsigned int                             _uMaximumPercent = 100;

    QMovie*                                  _mvLoading;

    QString                                  _sLoadingGIF;

    std::thread                              _thMainWorker;
};


#endif