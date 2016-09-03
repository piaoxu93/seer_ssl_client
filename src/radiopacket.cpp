#include "radiopacket.h"
#include "lib/crc/crc.h"
#include "singleparams.h"
#include <QSerialPort>
#include <QElapsedTimer>
#include <QtDebug>
#include <iostream>
RadioPacket::RadioPacket(QSerialPort* serialPtr)
    : startPacket1(TRANSMIT_PACKET_SIZE,0)
    , startPacket2(TRANSMIT_PACKET_SIZE,0)
    , transmitPacket(TRANSMIT_PACKET_SIZE,0)
    , serialPtr(serialPtr)
    , shoot(false), ctrl(false), shootMode(false), robotID(0)
    , velX(0), velY(0), velR(0)
    , ctrlPowerLevel(2), shootPowerLevel(0){

    startPacket1[0] = 0xff;
    startPacket1[1] = 0xb0;
    startPacket1[2] = 0x01;
    startPacket1[3] = 0x02;
    startPacket1[4] = 0x03;
    startPacket1[TRANSMIT_PACKET_SIZE - 1] = 0x31;

    startPacket2[0] = 0xff;
    startPacket2[1] = 0xb0;
    startPacket2[2] = 0x04;
    startPacket2[3] = 0x05;
    startPacket2[4] = 0x06;
    updateFrequency(int(SingleParams::instance()->_("crazy.frequency")));
    encode();
}
bool RadioPacket::sendStartPacket(){
    if(serialPtr != NULL){
        serialPtr->write((startPacket1.data()),TRANSMIT_PACKET_SIZE);
        serialPtr->write((startPacket2.data()),TRANSMIT_PACKET_SIZE);
        return true;
    }
    return false;
}
void RadioPacket::updateFrequency(int frequency){
    //int frequency = int(SingleParams::instance()->_("crazy.frequency"));
    qDebug() << "frequency!!! " << frequency;
    startPacket2[5] = 0x10 + frequency;
    startPacket2[TRANSMIT_PACKET_SIZE - 1] = CCrc8::calc((unsigned char*)(startPacket2.data()), TRANSMIT_PACKET_SIZE - 1);
}
bool RadioPacket::sendCommand(){
    static int times = 0;
    static QElapsedTimer timer;
    if(times == 0) timer.start();
    if(serialPtr != NULL){
        encode();
        serialPtr->write((transmitPacket.data()),TRANSMIT_PACKET_SIZE);
        //qDebug() << "Hz : " << double(++times)/timer.nsecsElapsed()*1000000000;
        return true;
    }
    return false;
}
bool RadioPacket::encode(){
    transmitPacket[0] = 0xff;
    if(robotID > 7){
        transmitPacket[1] = (1 << (robotID - 8)) | 0x00;
        transmitPacket[2] = 0x00;
    }
    else{
        transmitPacket[1] = 0x00;
        transmitPacket[2] = 1 << (robotID);
    }
    transmitPacket[3] = (shootMode << 6);
    transmitPacket[3] = transmitPacket[3] | (ctrl ? (ctrlPowerLevel << 4):0);

    //need to fix ,complement mode for vel;
    transmitPacket[4] = ((velX >= 0)?0:0x80) | (abs(velX) & 0x7f);
    transmitPacket[5] = ((velY >= 0)?0:0x80) | (abs(velY) & 0x7f);
    transmitPacket[6] = ((velR >= 0)?0:0x80) | (abs(velR) & 0x7f);
    if(transmitPacket[4] == char(0xff)) transmitPacket[4] = 0xfe;
    if(transmitPacket[5] == char(0xff)) transmitPacket[5] = 0xfe;
    if(transmitPacket[6] == char(0xff)) transmitPacket[6] = 0xfe;
    transmitPacket[15] = transmitPacket[16] = transmitPacket[17] = transmitPacket[18] = transmitPacket[19] = transmitPacket[20] = transmitPacket[21] = transmitPacket[22] = transmitPacket[23] = transmitPacket[24] = 0;
    transmitPacket[15] = ((abs(velX) & 0x180) >> 1) | ((abs(velY) & 0x180) >> 3) | ((abs(velR) & 0x180) >> 5);
    transmitPacket[18] = (shoot ? shootPowerLevel:0) & 0x7f;
    return true;
}
