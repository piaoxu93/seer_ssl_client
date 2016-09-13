#include "visionmodule.h"
#include <QtDebug>
#include <QNetworkInterface>

#include <string>
#include "singleparams.h"
#include "field.h"
#include "./lib/json.hpp"
#include "./proto/messages_robocup_ssl_detection.pb.h"
#include "./proto/messages_robocup_ssl_geometry.pb.h"
#include "./proto/messages_robocup_ssl_wrapper.pb.h"

void CVisionModule::setNewCameraID(int id){
    newCameraID = id;
    cameraUpdate[id] = true;
}

void CVisionModule::parse(void * ptr,int size){
    SSL_WrapperPacket packet;
    packet.ParseFromArray(ptr,size);
    if (packet.has_detection()) {
        int id = packet.detection().camera_id();
        setNewCameraID(id);
        detections[id] = packet.detection();
        int balls_n = detections[id].balls_size();
        int robots_blue_n =  detections[id].robots_blue_size();
        int robots_yellow_n =  detections[id].robots_yellow_size();
        for (int i = 0; i < balls_n; i++) {
            SSL_DetectionBall ball = detections[id].balls(i);
            GlobalData::instance()->setBall(newCameraID, i, ball.x(),ball.y(),ball.confidence());
        }
        for (int i = 0; i < robots_blue_n; i++) {
            SSL_DetectionRobot robot = detections[id].robots_blue(i);
                GlobalData::instance()->setBlueRobot(newCameraID, i,robot.robot_id(),robot.x(),robot.y(),robot.orientation(),robot.confidence());
        }
        for (int i = 0; i < robots_yellow_n; i++) {
            SSL_DetectionRobot robot = detections[id].robots_yellow(i);
            GlobalData::instance()->setYellowRobot(newCameraID, i,robot.robot_id(),robot.x(),robot.y(),robot.orientation(),robot.confidence());
        }
        if (collectNewVision()) {
            if(GlobalData::instance()->smsg.Cycle % 1 == 0)
                GlobalData::instance()->copyReceiveMsg();
            if(_cycle > 32766)
                _cycle = 1;
            transmit.setCycle(followCheckCycle, _cycle++);
            emit needDraw(true,false,true);
            sendSmsg();
            std::fill_n(cameraUpdate,PARAM::CAMERA,false);
        }
    }
}

CVisionModule::CVisionModule(QObject *parent)
    : QObject(parent)
    , cameraMode(FourCamera)
    , followCheckCycle(10)
    , m_sendFalse(false)
    , distorterr(25)
    , interface(0)
    , ifPause(false)
    , maxBallDist(int(SingleParams::instance()->_("vision.ballMaxSpeed"))*10/PARAM::FOLLOW::FRAMERATE)
    , maxVehicleDist(int(SingleParams::instance()->_("vision.robotMaxSpeed"))*10/PARAM::FOLLOW::FRAMERATE){
    std::fill_n(cameraUpdate,PARAM::CAMERA,false);

    minAddFrame = SingleParams::instance()->_("vision.minAddFrame");
    minLostFrame = SingleParams::instance()->_("vision.minLostFrame");

    std::string addressStr = SingleParams::instance()->_("vision.address");
    port = SingleParams::instance()->_("vision.port");
    groupAddress = QString(addressStr.c_str());

    sendUdp = new QUdpSocket;
    sendUdp->bind(QHostAddress::AnyIPv4, port+1, QUdpSocket::ShareAddress);

//    json sendArray = SingleParams::instance()->_("vision.send");
//    for(auto& a : sendArray){
//        std::string address = a["address"];
//        sendAddresses.push_back(QHostAddress(QString(address.c_str())));
//        sendPorts.push_back(a["port"]);
//    }
    sendPort = SingleParams::instance()->_("vision.send.port");
    std::string str = SingleParams::instance()->_("vision.send.address");
    sendAddress = QString(str.c_str());

    udpSocket = new QUdpSocket;
}
void CVisionModule::process(){

    while(true){
        sync.lock();
//        qDebug() << "ifPause : " << ifPause;
        if(ifPause)
            pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
        sync.unlock();
        dealWithData();
    }
}
void CVisionModule::dealWithData(){
    static QByteArray datagram;
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        parse((void*)datagram.data(),datagram.size());
    }
}
bool CVisionModule::collectNewVision() {
    for (int i=0;i<PARAM::CAMERA;i++) {
        if (GlobalData::instance()->cameraControl[i] && !cameraUpdate[i])
            return false;
    }
    return true;
}
void CVisionModule::updateVisionControl(bool mode){
    cameraMode = mode ? FourCamera : TwoCamUp;
    Field::instance()->changeMode(mode);
}
void CVisionModule::mix(){
//    static VisionMessage vision;
//    static quint32 maintainBall;
//    vision.init();
//    maintainBall = 0xffffffff;
//    bool exist;
//    auto& lastBall = GlobalData::instance()->vision[0].ball[0];
//    for (int i=0;i<PARAM::CAMERA;i++){
//        if (GlobalData::instance()->cameraControl[i]) {
//            auto& t = GlobalData::instance()->camera[i][0];
//            //* mix method one
//            for (int j=0;j<t.blueNum;j++){
//                exist = false;
//                for (int k=0;k<vision.blueSize;k++){
//                    if (vision.blue[k].id == t.blue[j].id){
//                        exist = true;
//                        break;
//                    }
//                }
//                if (!exist){
//                    vision.blueID[t.blue[j].id] = vision.blueSize;
//                    vision.blue[vision.blueSize++] = t.blue[j];
//                }
//            }
//            for (int j=0;j<t.yellowNum;j++){
//                exist = false;
//                for (int k=0;k<vision.yellowSize;k++){
//                    if (vision.yellow[k].id == t.yellow[j].id){
//                        exist = true;
//                        break;
//                    }
//                }
//                if (!exist){
//                    vision.yellowID[t.yellow[j].id] = vision.yellowSize;
//                    vision.yellow[vision.yellowSize++] = t.yellow[j];
//                }
//            }
//            for (int j=0;j<t.ballNum;j++) {
//                auto deltaX = qint16(lastBall.pos.x - t.ball[j].pos.x);
//                auto deltaY = qint16(lastBall.pos.y - t.ball[j].pos.y);
//                auto dis = quint32(deltaX * deltaX + deltaY * deltaY);
//                if ( dis < maintainBall ) {
//                    vision.ball[0] = t.ball[j];
//                    maintainBall = dis;
//                }
//            }
//            //*/
//            /* mix method two */

//        }
//        cameraUpdate[i] = false;
//    }
//    GlobalData::instance()->vision.push(vision);
}

int min_int(int a,int b){
    return a < b ? a : b;
}
void CVisionModule::send(){
//    auto& t = GlobalData::instance()->vision[0];
//    SendVisionMessage sendMessage;
//    sendMessage.Cycle = GlobalData::instance()->vision.cycle();
//    sendMessage.BallFound = t.ball[0].pos.x+9999 > 0.01 ;
//    sendMessage.Ballx = t.ball[0].pos.x;
//    sendMessage.Bally = t.ball[0].pos.y;
//    sendMessage.nCameraID = 4;
//    for(int i=0;i<min_int(t.yellowSize,6);i++){
//        sendMessage.RobotFound[0][i] = true;
//        sendMessage.RobotINDEX[0][i] = t.yellow[i].id;
//        sendMessage.RobotPosX[0][i] = t.yellow[i].pos.x;
//        sendMessage.RobotPosY[0][i] = t.yellow[i].pos.y;
//        sendMessage.RobotRotation[0][i] = t.yellow[i].angel;
//    }
//    for(int i=0;i<min_int(t.blueSize,6);i++){
//        sendMessage.RobotFound[1][i] = true;
//        sendMessage.RobotINDEX[1][i] = t.blue[i].id;
//        sendMessage.RobotPosX[1][i] = t.blue[i].pos.x;
//        sendMessage.RobotPosY[1][i] = t.blue[i].pos.y;
//        sendMessage.RobotRotation[1][i] = t.blue[i].angel;
//    }
//    for(int i = 0; i < QNetworkInterface::allInterfaces().length(); ++i){
//        for(int j = 0;j <sendAddresses.size(); j++){
//            sendUdp->setMulticastInterface(QNetworkInterface::allInterfaces()[i]);
//            sendUdp->writeDatagram((char*)&sendMessage,sizeof(sendMessage),sendAddresses[j], sendPorts[j]);
//        }
//    }
}
void CVisionModule::sendSmsg(){
    transmit_msg = transmit.smsgUpdate(cameraMode,minAddFrame, minLostFrame,maxVehicleDist + distorterr,m_sendFalse);
    GlobalData::instance()->msg.push(transmit_msg);
    sendUdp->writeDatagram((char*)&transmit_msg,sizeof(transmit_msg),sendAddress, sendPort);
    sendUdp->writeDatagram((char*)&transmit_msg,sizeof(transmit_msg),sendAddress2, sendPort2);
}
void CVisionModule::changeReceiverSetting(quint16 interface,const QString& address,quint16 port){

    this->interface = interface;
    this->groupAddress = address;
    this->port = port;
    udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(QHostAddress(groupAddress),QNetworkInterface::allInterfaces()[interface]);
}
void CVisionModule::changeSenderSetting(const QString& address,quint16 port,const QString& address2,quint16 port2){
    sendAddress = address;
    sendPort = port;
    sendAddress2 = address2;
    sendPort2 = port;
}
void CVisionModule::abortSetting(){
    udpSocket->abort();
}
void CVisionModule::udpSocketConnect(){
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(dealWithData()),Qt::DirectConnection);
}
void CVisionModule::udpSocketDisconnect(){
    disconnect(udpSocket,0,this,0);
}
