#ifndef VISIONMODULE_H
#define VISIONMODULE_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QVector>

#include <algorithm>
#include "singleparams.h"
#include "params.h"
#include "singleton.hpp"
namespace Vision{
    typedef struct _Pos{
        qreal x;
        qreal y;
        _Pos():x(-9999),y(-9999){}
    }Pos2d;
    typedef struct _Ball{
        Pos2d pos;
    }Ball;
    typedef struct _Robot{
        Pos2d pos;
        qreal angel;
        quint8 id;
        _Robot():id(-1){}
    }Robot;
}
//
typedef struct SENDVISIONMESSAGE
{
    int Cycle;
    bool  BallFound;
    float Ballx;
    float Bally;

    int nCameraID;
    int BallImagex;
    int BallImagey;

    unsigned char  RobotINDEX[2][6];
    bool  RobotFound[2][6];
    float RobotPosX[2][6];
    float RobotPosY[2][6];
    float RobotRotation[2][6];

}SendVisionMessage;
//
class VisionMessage{
public:
    Vision::Robot blue[PARAM::ROBOTNUM];
    Vision::Robot yellow[PARAM::ROBOTNUM];
    Vision::Ball ball;
    int blueSize;
    int blueIndex[PARAM::ROBOTMAXID];
    int yellowSize;
    int yellowIndex[PARAM::ROBOTMAXID];
    VisionMessage():blueSize(0),yellowSize(0){
        std::fill_n(blueIndex,PARAM::ROBOTMAXID,-1);
        std::fill_n(yellowIndex,PARAM::ROBOTMAXID,-1);
    }
    void init(){
        blueSize = 0;yellowSize = 0;
        std::fill_n(blueIndex,PARAM::ROBOTMAXID,-1);
        std::fill_n(yellowIndex,PARAM::ROBOTMAXID,-1);
    }
};
class ReceiveVisionMessage{
public:
    quint8 camID;
    quint32 frame;
    qreal captureTime;
    quint8 ballNum;
    quint8 blueNum;
    quint8 yellowNum;
    Vision::Robot blue[PARAM::ROBOTNUM];
    Vision::Robot yellow[PARAM::ROBOTNUM];
    Vision::Ball ball[PARAM::BALLNUM];
    ReceiveVisionMessage():camID(-1),frame(-1),captureTime(-1),blueNum(0),yellowNum(0){}
};
typedef struct _VisionData{

}VisionData;


class CVisionModule : public QObject
{
    Q_OBJECT
public:
    CVisionModule(QObject *parent = 0);
    void mix();
    void send();
public slots:
    void updateVisionControl(int);
signals:

private slots:
    void output();
private:
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    ReceiveVisionMessage currentVision;
    bool collectNewVision();
    bool cameraUpdate[PARAM::CAMERA];
private:
    QVector<QHostAddress> sendAddresses;
    QVector<quint16> sendPorts;
    QUdpSocket *sendUdp;
};
typedef Singleton<CVisionModule> VisionModule;
#endif // VISIONMODULE_H
