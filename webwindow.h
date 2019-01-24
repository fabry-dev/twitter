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
private slots:
    void postData();
    void getButton(int b);
    void handleRFID();


};

#endif // WEBWINDOW_H
