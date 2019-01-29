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
public:
   slabel(QLabel *parent):QLabel(parent){}
   // virtual ~slabel() {}
protected:
    void mousePressEvent( QMouseEvent* ev ){emit clicked();}
signals:
    void clicked();

};




#endif // SLABEL_H
