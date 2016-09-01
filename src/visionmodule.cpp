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
        detection = packet.detection();
        setNewCameraID(detection.camera_id());
        int balls_n = detection.balls_size();
        int robots_blue_n =  detection.robots_blue_size();
        int robots_yellow_n =  detection.robots_yellow_size();
        for (int i = 0; i < balls_n; i++) {
            SSL_DetectionBall ball = detection.balls(i);
            GlobalData::Instance()->setBall(newCameraID, i, ball.x(),ball.y(),ball.confidence());
        }
        for (int i = 0; i < robots_blue_n; i++) {
            SSL_DetectionRobot robot = detection.robots_blue(i);
                GlobalData::Instance()->setBlueRobot(newCameraID, i,robot.robot_id(),robot.x(),robot.y(),robot.orientation(),robot.confidence());
        }
        for (int i = 0; i < robots_yellow_n; i++) {
            SSL_DetectionRobot robot = detection.robots_yellow(i);
            GlobalData::Instance()->setYellowRobot(newCameraID, i,robot.robot_id(),robot.x(),robot.y(),robot.orientation(),robot.confidence());
        }
        if (collectNewVision()) {
            if(GlobalData::Instance()->smsg.Cycle % 1 == 0)
                GlobalData::Instance()->copyReceiveMsg();
            if(_cycle > 32766)
                _cycle = 1;
            transmit.setCycle(followCheckCycle, _cycle++);
            Field::instance()->draw();
//            doTransmit();
            sendSmsg();
            std::fill_n(cameraUpdate,PARAM::CAMERA,false);
        }
    }
}

CVisionModule::CVisionModule(QObject *parent)
    : QObject(parent)
    , sendAddresses(0)
    , sendPorts(0)
    , cameraMode(FourCamera)
    , followCheckCycle(10)
    , m_sendFalse(false)
    , distorterr(25)
    , maxBallDist(int(SingleParams::instance()->_("vision.ballMaxSpeed"))*10/PARAM::FOLLOW::FRAMERATE)
    , maxVehicleDist(int(SingleParams::instance()->_("vision.robotMaxSpeed"))*10/PARAM::FOLLOW::FRAMERATE){
    std::fill_n(cameraUpdate,PARAM::CAMERA,false);

    minAddFrame = SingleParams::instance()->_("vision.minAddFrame");
    minLostFrame = SingleParams::instance()->_("vision.minLostFrame");

    std::string addressStr = SingleParams::instance()->_("vision.address");
    int port = SingleParams::instance()->_("vision.port");
    groupAddress = QString(addressStr.c_str());
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(output()));

    //
    sendUdp = new QUdpSocket();
    sendUdp->bind(QHostAddress::AnyIPv4, port+1, QUdpSocket::ShareAddress);

    json sendArray = SingleParams::instance()->_("vision.send");
    for(auto& a : sendArray){
        std::string address = a["address"];
        sendAddresses.push_back(QHostAddress(QString(address.c_str())));
        sendPorts.push_back(a["port"]);
    }
}
void CVisionModule::output(){
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        parse((void*)datagram.data(),datagram.size());
//        parseAndStoreVisionData((void *)datagram.data(),datagram.size(),currentVision);
//        GlobalData::Instance()->camera[currentVision.camID].push(currentVision);
//        cameraUpdate[currentVision.camID] = true;
//        if (collectNewVision()){
//            ballFollow.startFollowDouble( ballAddFrame, ballLostFrame, ballMaxSpeed*10/PARAM::FOLLOW::FRAMERATE);
//            blueFollow.startFollowDouble( robotAddFrame, robotLostFrame, robotMaxSpeed*10/PARAM::FOLLOW::FRAMERATE);
//            yellowFollow.startFollowDouble( robotAddFrame, robotLostFrame, robotMaxSpeed*10/PARAM::FOLLOW::FRAMERATE);
//            //mix();
//            //send();
//            sendSmsg();
//            Field::instance()->draw();
//        }
    }
}
bool CVisionModule::collectNewVision() {
    for (int i=0;i<PARAM::CAMERA;i++) {
        if (GlobalData::Instance()->cameraControl[i] && !cameraUpdate[i])
            return false;
    }
    return true;
}
void CVisionModule::updateVisionControl(int index){
//    bool& c = GlobalData::Instance()->cameraControl[index];
//    c = !c;
}
void CVisionModule::mix(){
//    static VisionMessage vision;
//    static quint32 maintainBall;
//    vision.init();
//    maintainBall = 0xffffffff;
//    bool exist;
//    auto& lastBall = GlobalData::Instance()->vision[0].ball[0];
//    for (int i=0;i<PARAM::CAMERA;i++){
//        if (GlobalData::Instance()->cameraControl[i]) {
//            auto& t = GlobalData::Instance()->camera[i][0];
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
//    GlobalData::Instance()->vision.push(vision);
}

int min_int(int a,int b){
    return a < b ? a : b;
}
void CVisionModule::send(){
//    auto& t = GlobalData::Instance()->vision[0];
//    SendVisionMessage sendMessage;
//    sendMessage.Cycle = GlobalData::Instance()->vision.cycle();
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
    GlobalData::Instance()->msg.push(transmit_msg);
    for(int i = 0; i < QNetworkInterface::allInterfaces().length(); ++i){
        for(int j = 0;j <sendAddresses.size(); j++){
            sendUdp->setMulticastInterface(QNetworkInterface::allInterfaces()[i]);
            sendUdp->writeDatagram((char*)&transmit_msg,sizeof(transmit_msg),sendAddresses[j], sendPorts[j]);
        }
    }
}
