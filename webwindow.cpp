#include "webwindow.h"
#define RESET_DELAY (60*1000)

webWindow::webWindow(QObject *parent, QString PATH):QObject(parent),PATH(PATH)
{
    QString fileName = "/dev/input/by-id/usb-SpringCard_ProxNRoll_RFID_Scanner_517F095D-event-kbd";
    fd = open(fileName.toUtf8().data(), O_RDONLY|O_NONBLOCK);
    if( fd==-1 ){
        qWarning("can not open RFID socket");
        //return;
    }
    notifier = new QSocketNotifier( fd, QSocketNotifier::Read,this);

    connect( notifier,SIGNAL(activated(int)),this,SLOT(handleRFID()));


    for(int i=-2;i<=6;i++)
    {

        specialTimer *t = new specialTimer(this,i);
        connect(t,SIGNAL(triggered(int)),this,SLOT(getRFID(int)));
        t->setSingleShot(true);
        t->setInterval(RESET_DELAY);
        timers.push_back(t);
    }


    networkManager = new QNetworkAccessManager(this);
    activeFloors.clear();

    postData();

}



void webWindow::postData()
{
    std::sort(activeFloors.begin(),activeFloors.end());

    QString buf = "";
    for (auto f:activeFloors)
    {
        buf.append(QString::number(f));
        buf.append(",");
    }

    buf = buf.left(buf.size()-1);


    qDebug()<<"activating floors: "<<buf;

    buf = "{\"data\":{\"floors\":["+buf+"]}}";

    //buf = "{\"data\":{\"floors\":[]}}";
    QByteArray jsonString = buf.toUtf8();
    QByteArray postDataSize = QByteArray::number(jsonString.size());
    // QUrl serviceURL("142.93.241.114");
    QUrl serviceURL("http://elevator.webagencydubai.com/elevatordata");
    QNetworkRequest request(serviceURL);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postDataSize);

    QNetworkReply *reply = networkManager->post(request,jsonString);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray data = reply->readAll();
    QString dataReply(data);

    qDebug()<<dataReply.left(6);


}



void webWindow::getButton(int b)
{


    timers[b+2]->start();


    qDebug()<<"button"<<b;
    if(std::find(activeFloors.begin(), activeFloors.end(), b) != activeFloors.end())
    {
        //already active
    }
    else
    {
        //new floor activated
        activeFloors.push_back(b);
        postData();
    }

}



void webWindow::getRFID(int r)
{
    timers[r+2]->stop();

    if(std::find(activeFloors.begin(), activeFloors.end(), r) != activeFloors.end())
    {
        activeFloors.erase(std::remove(activeFloors.begin(), activeFloors.end(), r), activeFloors.end());
        postData();
    }
    else
    {

    }

}




void webWindow::handleRFID()
{
    // qDebug()<<"notifs";

    struct input_event ev;


    while(read(fd, &ev, sizeof(struct input_event))>0)
    {

        if((ev.type == 1)&&(ev.value==0))
        {
            if(ev.code == 28)
                nuTag();
            else
                tagBuf.push_back(ev.code);

        }
    }

}


void webWindow::nuTag()
{


    QString tag = "";

    for(auto i:tagBuf)
        tag.append(QString::number((i-1)%10));

    tagBuf.clear();
    //tag.toInt() //1 - 19


    int level = (tag.toInt())/2-2;

    QString pos;
    if((tag.toInt())%2)
        pos = "+";
    else pos = "-";


    getRFID(level);
    qDebug()<<"RFID level "<<level<<pos;

}
