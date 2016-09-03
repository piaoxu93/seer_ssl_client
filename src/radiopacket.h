#ifndef RADIOPACKET_H
#define RADIOPACKET_H

#include <QSerialPort>
#include <QtDebug>
class RadioPacket
{
public:
    explicit RadioPacket(QSerialPort* serialPtr);
    bool sendStartPacket();
    bool sendCommand();
    void updateCommandParams(int robotID,int velX,int velY,int velR,bool ctrl,bool mode,bool shoot,int power){
        this->robotID = robotID;
        this->velX = velX; this->velY = velY; this->velR = velR;
        this->ctrl = ctrl;
        this->shootMode = mode;this->shoot = shoot; this->shootPowerLevel = power;
    }
    void updateFrequency(int);
private:
    static const int TRANSMIT_PACKET_SIZE = 25;
    QByteArray startPacket1;
    QByteArray startPacket2;
    QByteArray transmitPacket;
    QSerialPort* serialPtr;
    bool encode();
private:
    bool shoot;
    bool ctrl;
    bool shootMode;//false is "flat shoot" and true is "lift shoot".
    quint8 robotID;
    qint16 velX;
    qint16 velY;
    qint16 velR;
    quint16 ctrlPowerLevel;
    quint16 shootPowerLevel;
};

#endif // RADIOPACKET_H
