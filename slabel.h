#ifndef SLABEL_H
#define SLABEL_H


#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "qprocess.h"
#include "qdebug.h"
#include "QFile"
#include "webwindow.h"
#include "serialwatcher.h"
#include "qlabel.h"








class slabel : public QLabel
{
    Q_OBJECT
private:
    QProcess *chrome;

public:
    slabel(QLabel *parent):QLabel(parent){
        chrome = new QProcess(this);
       // start();
        // connect(chrome,SIGNAL(finished(int)),qApp,SLOT(quit()));
    }


protected:
    void mousePressEvent( QMouseEvent* ev ){

        chrome->terminate();

        connect(chrome,SIGNAL(finished(int)),this,SLOT(start()));
    }
signals:
    void clicked();
private slots:
    void start()
    {
        chrome->start("google-chrome --incognito  --kiosk http://elevator.webagencydubai.com/");

    }

    void closeForReal()
    {
        chrome->terminate();
        qApp->closeAllWindows();
    }
};





class slabel2 : public QLabel
{
    Q_OBJECT
private:
    QProcess *chrome;

public:
    slabel2(QLabel *parent):QLabel(parent){


    }


protected:
    void mousePressEvent( QMouseEvent* ev ){

        emit clicked();
    }
signals:
    void clicked();


};



#endif // SLABEL_H
