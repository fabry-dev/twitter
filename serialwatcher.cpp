#include "serialwatcher.h"

#include <fcntl.h>
#include <unistd.h>




#define maxPage 14

serialWatcher::serialWatcher(QObject *parent)

    : QObject(parent)
{



    port = new QSerialPort;
    port->setPortName("/dev/ttyACM0");


    if(port->open(QIODevice::ReadWrite))
        qDebug()<<"serial created";
    else
        qDebug()<<"serial error";


    port->setBaudRate(QSerialPort::Baud9600)

            && port->setDataBits(QSerialPort::Data8)

            && port->setParity(QSerialPort::NoParity)

            && port->setStopBits(QSerialPort::OneStop)

            && port->setFlowControl(QSerialPort::NoFlowControl);


    port->clear();

    connect(port, SIGNAL(readyRead()), this, SLOT(readData()));

}






void serialWatcher::readData()
{

    const QByteArray data = port->readAll();
    uchar b = data.at(0)-100;

    if(b==4)
        b=5;
    else if(b==5)
        b=4;
    else if(b==2)
        b=3;
    else if(b==3)
        b=2;
    else if(b==0)
        b=1;
    else if(b==1)
        b=0;
    else if(b==-1)
        b=-2;
    else if(b==-2)
        b=-1;


    emit nuData(b);

}


serialWatcher::~serialWatcher()
{
    qDebug()<<"closed";

}


