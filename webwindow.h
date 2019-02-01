#ifndef WEBWINDOW_H
#define WEBWINDOW_H

#include "qobject.h"
#include "qwidget.h"
#include "QDebug"
#include <QtWebKit/QtWebKit>
#include <QtWebKitWidgets/QtWebKitWidgets>
#include <QUrl>
#include <QNetworkRequest>
#include <qsocketnotifier.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>


class specialTimer:public QTimer
{
    Q_OBJECT
public:
    specialTimer(QObject *parent=NULL,int id=0):QTimer(parent),id(id) {connect(this,SIGNAL(timeout()),this,SLOT(goTimeOut()));}
private:
    int id;
private slots:
    void goTimeOut(){emit triggered(id);}
signals:
    void triggered(int);
};


class webWindow : public QObject
{
    Q_OBJECT
public:
    webWindow(QObject *parent=NULL, QString PATH="");


private:
    QString PATH;
    QNetworkAccessManager *networkManager;
    std::vector<int> activeFloors;
    QSocketNotifier *notifier;
    int fd;
    std::vector<int>tagBuf;
    void nuTag();

    std::vector<specialTimer*> timers;
private slots:
    void postData();
    void getButton(int b);
    void handleRFID();
    void getRFID(int r);


};

#endif // WEBWINDOW_H
