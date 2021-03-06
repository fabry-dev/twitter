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
#include "slabel.h"
#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/twitter/files/"















int main(int argc, char *argv[])

{
    QApplication a(argc, argv);





    QString PATH;
    QStringList params = a.arguments();
    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;

    bool HIDE_CURSOR=false;




    QFile file(PATH+"config.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QString paramName,paramValue;
        QStringList params;

        while(!in.atEnd()) {
            line = in.readLine();
            line = (line.split("#"))[0];
            params = line.split("=");
            if(params.size()>=2)
            {
                paramName = params[0];
                paramValue = params[1];

                if (paramName.mid(0,6)=="CURSOR")
                    HIDE_CURSOR = (paramValue=="NO");

                else
                    qDebug()<<paramName<<" - "<<paramValue;
            }
        }
        file.close();

    }




    if (HIDE_CURSOR)
    {
        QCursor cursor(Qt::BlankCursor);
        a.setOverrideCursor(cursor);
        a.changeOverrideCursor(cursor);
    }






    serialWatcher *sw = new serialWatcher(NULL);
    webWindow *ww = new webWindow(NULL,PATH);

    a.connect(sw,SIGNAL(nuData(int)),ww,SLOT(getButton(int)));



    slabel *w = new slabel(NULL);

    w->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Window | Qt::FramelessWindowHint);
    QScreen* screen0 = a.screens().at(0);
    int W = 20;
    int H = 20;
    w->setGeometry(screen0->geometry().x()+screen0->size().width()-W,screen0->geometry().y()+screen0->size().height()-H,W,H);
    w->raise();
    w->setStyleSheet("QLabel { background-color : black; }");
    w->show();



    slabel2 *w2 = new slabel2(NULL);

    w2->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Window | Qt::FramelessWindowHint);
    w2->setGeometry(screen0->geometry().x(),screen0->geometry().y()+screen0->size().height()-H,W,H);
    w2->raise();
    w2->setStyleSheet("QLabel { background-color : black; }");
    w2->show();
    a.connect(w2,SIGNAL(clicked()),w,SLOT(closeForReal()));



    return a.exec();

}
