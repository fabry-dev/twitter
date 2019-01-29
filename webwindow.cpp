#include "webwindow.h"

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

    // buf = "{\"data\":{\"floors\":["+buf+"]}}";

    buf = "{\"data\":{\"floors\":[]}}";
    QByteArray jsonString = buf.toUtf8();
    QByteArray postDataSize = QByteArray::number(jsonString.size());
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

    qDebug()<<dataReply;


}



void webWindow::getButton(int b)
{
    b-=2;


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
