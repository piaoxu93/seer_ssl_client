#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QtDebug>
#include <algorithm>
#include "singleparams.h"
#include "params.h"
#include "singleton.hpp"

#include "./follow/Transmit.h"
#include "./follow/BallFollow.h"
#include "./follow/RobotFollow.h"
#include "./proto/messages_robocup_ssl_wrapper.pb.h"
class CVisionModule : public QObject
{
    Q_OBJECT
public:
    CVisionModule(QObject *parent = 0);
    void parse(void * ptr,int size);
    void mix();
    void send();
    void sendSmsg();
    void resume(){
        sync.lock();
        ifPause = false;
        sync.unlock();
        pauseCond.wakeAll();
    }
    void pause(){
        sync.lock();
        ifPause = true;
        sync.unlock();
    }
public slots:
    void updateVisionControl(bool);
    void process();
    void changeSetting(quint16 interface,const QString& address,quint16 port,const QString& senderAddress,quint16 senderPort){
        changeReceiverSetting(interface, address, port);
        changeSenderSetting(senderAddress, senderPort);
    }
    void abortSetting();
public:
    void changeReceiverSetting(quint16 interface,const QString& address,quint16 port);
    void changeSenderSetting(const QString&,quint16);
signals:
    void needDraw(bool,bool,bool);
private slots:
private:
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    int interface;
    int port;
    ReceiveVisionMessage currentVision;
    bool collectNewVision();
    bool cameraUpdate[PARAM::CAMERA];
private:
    BallFollow ballFollow;
    RobotFollow yellowFollow;
    RobotFollow blueFollow;
    qreal ballMaxSpeed;
    qreal robotMaxSpeed;

    void setNewCameraID(int id);

    SendVisionMessage  transmit_msg;
    SSL_DetectionFrame detections[PARAM::CAMERA];
    int newCameraID;
    int _cycle;
    CAMERAMODE cameraMode;
    int followCheckCycle;
    bool m_sendFalse;//false
    Transmit transmit;
    int minAddFrame;
    int minLostFrame;
    float maxBallDist;//speed*1000/60
    float maxVehicleDist;//speed*1000/60
    float distorterr;//25
private:
    QHostAddress sendAddress;
    quint16 sendPort;
    QUdpSocket *sendUdp;
private:
    QMutex sync;
    QWaitCondition pauseCond;
    bool ifPause;
};
typedef Singleton<CVisionModule> VisionModule;
#endif // VISIONMODULE_H
