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

    if(std::find(activeFloors.begin(), activeFloors.end(), b) != activeFloors.end()) {
        //already active
    }
    else
    {
        //new floor activated
        activeFloors.push_back(b);
        postData();
    }

}


void webWindow::handleRFID()
{
    // qDebug()<<"notifs";

    struct input_event ev;


    while(read(fd, &ev, sizeof(struct input_event))>0)
    {
        // qDebug()<<ev.code;


            int buf = ev.code;

            tagBuf.push_back(buf);
            if(tagBuf.size()>=19)
                nuTag();

    }

}


void webWindow::nuTag()
{

    qDebug()<<"tag:"<<tagBuf;
    tagBuf.clear();
}
