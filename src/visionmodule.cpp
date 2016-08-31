#include "visionmodule.h"
#include <QtDebug>
#include <QNetworkInterface>

#include <string>
#include "singleparams.h"
#include "field.h"
#include "globaldata.h"
#include "./lib/json.hpp"
#include "proto/messages_robocup_ssl_detection.pb.h"
#include "proto/messages_robocup_ssl_geometry.pb.h"
#include "proto/messages_robocup_ssl_wrapper.pb.h"

void parseAndStoreRobotData(const SSL_DetectionRobot & robot,Vision::Robot& currentRobotMessage){
//    printf("CONF=%4.2f ", robot.confidence());
    if (robot.has_robot_id()) {
//        printf("ID=%3d ",robot.robot_id());
        currentRobotMessage.id = robot.robot_id();
    } else {
//        printf("ID=N/A ");
    }
//    printf(" HEIGHT=%6.2f POS=<%9.2f,%9.2f> ",robot.height(),robot.x(),robot.y());
    currentRobotMessage.pos.x = robot.x();
    currentRobotMessage.pos.y = robot.y();
    if (robot.has_orientation()) {
//        printf("ANGLE=%6.3f ",robot.orientation());
        currentRobotMessage.angel = robot.orientation();
    } else {
//        printf("ANGLE=N/A    ");
    }
    //printf("RAW=<%8.2f,%8.2f>\n",robot.pixel_x(),robot.pixel_y());
}
void parseAndStoreVisionData(void * ptr,int size,ReceiveVisionMessage & currentMessage){
    // copy from SSL-VISION :: client/main.cpp

    SSL_WrapperPacket packet;
    packet.ParseFromArray(ptr,size);
//    printf("-----Received Wrapper Packet---------------------------------------------\n");
    //see if the packet contains a robot detection frame:
    if (packet.has_detection()) {
        SSL_DetectionFrame detection = packet.detection();
        //Display the contents of the robot detection results:
        //double t_now = GetTimeSec();

//        printf("-[Detection Data]-------\n");
        //Frame info:
//        printf("Camera ID=%d FRAME=%d T_CAPTURE=%.4f\n",detection.camera_id(),detection.frame_number(),detection.t_capture());

        currentMessage.camID = detection.camera_id();
        currentMessage.frame = detection.frame_number();
        currentMessage.captureTime = detection.t_capture();

//        printf("SSL-Vision Processing Latency                   %7.3fms\n",(detection.t_sent()-detection.t_capture())*1000.0);
        int balls_n = detection.balls_size();
        int robots_blue_n =  detection.robots_blue_size();
        int robots_yellow_n =  detection.robots_yellow_size();

        currentMessage.ballNum = detection.balls_size();
        currentMessage.yellowNum = detection.robots_yellow_size();
        currentMessage.blueNum = detection.robots_blue_size();

        //Ball info:
        for (int i = 0; i < balls_n; i++) {
            SSL_DetectionBall ball = detection.balls(i);
//            printf("-Ball (%2d/%2d): CONF=%4.2f POS=<%9.2f,%9.2f> ", i+1, balls_n, ball.confidence(),ball.x(),ball.y());

            currentMessage.ball[i].pos.x = ball.x();
            currentMessage.ball[i].pos.y = ball.y();

/*
            if (ball.has_z()) {
//                printf("Z=%7.2f ",ball.z());
            } else {
//                printf("Z=N/A   ");
            }
//            printf("RAW=<%8.2f,%8.2f>\n",ball.pixel_x(),ball.pixel_y());
*/
        }
        //Blue robot info:
        for (int i = 0; i < robots_blue_n; i++) {
            SSL_DetectionRobot robot = detection.robots_blue(i);
//            printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);

            parseAndStoreRobotData(robot,currentMessage.blue[i]);
        }

        //Yellow robot info:
        for (int i = 0; i < robots_yellow_n; i++) {
            SSL_DetectionRobot robot = detection.robots_yellow(i);
//            printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
            parseAndStoreRobotData(robot,currentMessage.yellow[i]);
        }

    }
/*
    //see if packet contains geometry data:
    if (packet.has_geometry()) {
        const SSL_GeometryData & geom = packet.geometry();
//        printf("-[Geometry Data]-------\n");

        const SSL_GeometryFieldSize & field = geom.field();
//        printf("Field Dimensions:\n");
//        printf("  -field_length=%d (mm)\n",field.field_length());
//        printf("  -field_width=%d (mm)\n",field.field_width());
//        printf("  -boundary_width=%d (mm)\n",field.boundary_width());
//        printf("  -goal_width=%d (mm)\n",field.goal_width());
//        printf("  -goal_depth=%d (mm)\n",field.goal_depth());
//        printf("  -field_lines_size=%d\n",field.field_lines_size());
//        printf("  -field_arcs_size=%d\n",field.field_arcs_size());

        int calib_n = geom.calib_size();
        for (int i=0; i< calib_n; i++) {
            const SSL_GeometryCameraCalibration & calib = geom.calib(i);
//            printf("Camera Geometry for Camera ID %d:\n", calib.camera_id());
//            printf("  -focal_length=%.2f\n",calib.focal_length());
//            printf("  -principal_point_x=%.2f\n",calib.principal_point_x());
//            printf("  -principal_point_y=%.2f\n",calib.principal_point_y());
//            printf("  -distortion=%.2f\n",calib.distortion());
//            printf("  -q0=%.2f\n",calib.q0());
//            printf("  -q1=%.2f\n",calib.q1());
//            printf("  -q2=%.2f\n",calib.q2());
//            printf("  -q3=%.2f\n",calib.q3());
//            printf("  -tx=%.2f\n",calib.tx());
//            printf("  -ty=%.2f\n",calib.ty());
//            printf("  -tz=%.2f\n",calib.tz());

            if (calib.has_derived_camera_world_tx() && calib.has_derived_camera_world_ty() && calib.has_derived_camera_world_tz()) {
//              printf("  -derived_camera_world_tx=%.f\n",calib.derived_camera_world_tx());
//              printf("  -derived_camera_world_ty=%.f\n",calib.derived_camera_world_ty());
//              printf("  -derived_camera_world_tz=%.f\n",calib.derived_camera_world_tz());
            }
        }
    }
*/
}
CVisionModule::CVisionModule(QObject *parent)
    : QObject(parent)
    , sendAddresses(0)
    , sendPorts(0){
    for (int i=0;i<PARAM::CAMERA;i++) {
        cameraUpdate[i] = false;
    }
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
        parseAndStoreVisionData((void *)datagram.data(),datagram.size(),currentVision);
        GlobalData::Instance()->camera[currentVision.camID].push(currentVision);
        cameraUpdate[currentVision.camID] = true;
        if (collectNewVision()){
            mix();
            send();
            Field::instance()->draw();
        }
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
    bool& c = GlobalData::Instance()->cameraControl[index];
    c = !c;
}
void CVisionModule::mix(){
    static VisionMessage vision;
    static quint32 maintainBall;
    vision.init();
    maintainBall = 0xffffffff;
    bool exist;
    auto& lastBall = GlobalData::Instance()->vision[0].ball;
    for (int i=0;i<PARAM::CAMERA;i++){
        if (GlobalData::Instance()->cameraControl[i]) {
            auto& t = GlobalData::Instance()->camera[i][0];
            //* mix method one
            for (int j=0;j<t.blueNum;j++){
                exist = false;
                for (int k=0;k<vision.blueSize;k++){
                    if (vision.blue[k].id == t.blue[j].id){
                        exist = true;
                        break;
                    }
                }
                if (!exist){
                    vision.blue[vision.blueSize++] = t.blue[j];
                }
            }
            for (int j=0;j<t.yellowNum;j++){
                exist = false;
                for (int k=0;k<vision.yellowSize;k++){
                    if (vision.yellow[k].id == t.yellow[j].id){
                        exist = true;
                        break;
                    }
                }
                if (!exist)
                    vision.yellow[vision.yellowSize++] = t.yellow[j];
            }
            for (int j=0;j<t.ballNum;j++) {
                auto deltaX = qint16(lastBall.pos.x - t.ball[j].pos.x);
                auto deltaY = qint16(lastBall.pos.y - t.ball[j].pos.y);
                auto dis = quint32(deltaX * deltaX + deltaY * deltaY);
                if ( dis < maintainBall ) {
                    vision.ball = t.ball[j];
                    maintainBall = dis;
                }
            }
            //*/
            /* mix method two */

        }
        cameraUpdate[i] = false;
    }
    GlobalData::Instance()->vision.push(vision);
}

int min_int(int a,int b){
    return a < b ? a : b;
}
void CVisionModule::send(){
    auto& t = GlobalData::Instance()->vision[0];
    SendVisionMessage sendMessage;
    sendMessage.Cycle = GlobalData::Instance()->vision.cycle();
    sendMessage.BallFound = t.ball.pos.x+9999 > 0.01 ;
    sendMessage.Ballx = t.ball.pos.x;
    sendMessage.Bally = t.ball.pos.y;
    sendMessage.nCameraID = 4;
    for(int i=0;i<min_int(t.yellowSize,6);i++){
        sendMessage.RobotFound[0][i] = true;
        sendMessage.RobotINDEX[0][i] = t.yellow[i].id;
        sendMessage.RobotPosX[0][i] = t.yellow[i].pos.x;
        sendMessage.RobotPosY[0][i] = t.yellow[i].pos.y;
        sendMessage.RobotRotation[0][i] = t.yellow[i].angel;
    }
    for(int i=0;i<min_int(t.blueSize,6);i++){
        sendMessage.RobotFound[1][i] = true;
        sendMessage.RobotINDEX[1][i] = t.blue[i].id;
        sendMessage.RobotPosX[1][i] = t.blue[i].pos.x;
        sendMessage.RobotPosY[1][i] = t.blue[i].pos.y;
        sendMessage.RobotRotation[1][i] = t.blue[i].angel;
    }
    for(int i = 0; i < QNetworkInterface::allInterfaces().length(); ++i){
        for(int j = 0;j <sendAddresses.size(); j++){
            sendUdp->setMulticastInterface(QNetworkInterface::allInterfaces()[i]);
            sendUdp->writeDatagram((char*)&sendMessage,sizeof(sendMessage),sendAddresses[j], sendPorts[j]);
        }
    }
}
