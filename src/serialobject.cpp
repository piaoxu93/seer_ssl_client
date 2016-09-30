#include <QtSerialPort/QSerialPortInfo>
#include <QtDebug>
#include <QTest>
#include "serialobject.h"
#include "singleparams.h"
SerialObject::SerialObject(QObject *parent):QObject(parent),radioPacket(&serial){
    // add needed settings
    char tempIndex[] = {0,4,3,0,0,8};
    tempIndex[5] = int(SingleParams::instance()->_("crazy.frequency"));
    defaultIndex.append(tempIndex,6);
    currentIndex.append(tempIndex,6);
    const auto& ports = QSerialPortInfo::availablePorts();
    for(const auto &port : ports){
        this->ports.append(port.portName());
    }
    settingsName.append("Ports");
    settingsName.append("BaudRate");
    settingsName.append("DataBits");
    settingsName.append("Parity");
    settingsName.append("StopBits");
    settingsName.append("Frequency");
    baudRate.append(QSerialPort::Baud9600);     stringBaudRate.append("9600");
    baudRate.append(QSerialPort::Baud19200);    stringBaudRate.append("19200");
    baudRate.append(QSerialPort::Baud38400);    stringBaudRate.append("38400");
    baudRate.append(QSerialPort::Baud57600);    stringBaudRate.append("57600");
    baudRate.append(QSerialPort::Baud115200);   stringBaudRate.append("115200");
    dataBits.append(QSerialPort::Data5);        stringDataBits.append("5");
    dataBits.append(QSerialPort::Data6);        stringDataBits.append("6");
    dataBits.append(QSerialPort::Data7);        stringDataBits.append("7");
    dataBits.append(QSerialPort::Data8);        stringDataBits.append("8");
    parity.append(QSerialPort::NoParity);       stringParity.append("NoParity");
    parity.append(QSerialPort::EvenParity);     stringParity.append("EvenParity");
    parity.append(QSerialPort::OddParity);      stringParity.append("OddParity");
    parity.append(QSerialPort::SpaceParity);    stringParity.append("SpaceParity");
    parity.append(QSerialPort::MarkParity);     stringParity.append("MarkParity");
    stopBits.append(QSerialPort::OneStop);      stringStopBits.append("One");
#ifdef Q_OS_WIN
    stopBits.append(QSerialPort::OneAndHalfStop);stringStopBits.append("OneAndHalf");
#endif
    stopBits.append(QSerialPort::TwoStop);      stringStopBits.append("Two");
    frequency.append(0);                        stringFrequency.append("0");
    frequency.append(1);                        stringFrequency.append("1");
    frequency.append(2);                        stringFrequency.append("2");
    frequency.append(3);                        stringFrequency.append("3");
    frequency.append(4);                        stringFrequency.append("4");
    frequency.append(5);                        stringFrequency.append("5");
    frequency.append(6);                        stringFrequency.append("6");
    frequency.append(7);                        stringFrequency.append("7");
    frequency.append(8);                        stringFrequency.append("8");
    frequency.append(9);                        stringFrequency.append("9");
}
QString SerialObject::getName(int itemIndex) const{
    return settingsName[itemIndex];
}
QStringList SerialObject::getCrazySetting(int itemIndex) const{
    switch(itemIndex){
    case 0:
        return ports;
    case 1:
        return stringBaudRate;
    case 2:
        return stringDataBits;
    case 3:
        return stringParity;
    case 4:
        return stringStopBits;
    case 5:
        return stringFrequency;
    default:
        break;
    }
    return QStringList();
}
void SerialObject::sendCrazySetting(int itemIndex,int index){
    currentIndex[itemIndex] = index;
    qDebug() << "---------------------";
    qDebug() << "port name -- " << ports[currentIndex[0]];
    qDebug() << "baud rate -- " << stringBaudRate[currentIndex[1]];
    qDebug() << "data bits -- " << stringDataBits[currentIndex[2]];
    qDebug() << "parity    -- " << stringParity[currentIndex[3]];
    qDebug() << "stop bits -- " << stringStopBits[currentIndex[4]];
    qDebug() << "frequency -- " << stringFrequency[currentIndex[5]];
    qDebug() << "---------------------" << endl;
}
int SerialObject::getDefaultIndex(int itemIndex) const{
    return defaultIndex[itemIndex];
}
void SerialObject::openSerialPort(){
    serial.setPortName(ports[currentIndex[0]]);
    serial.setBaudRate(baudRate[currentIndex[1]]);
    serial.setDataBits(dataBits[currentIndex[2]]);
    serial.setParity(parity[currentIndex[3]]);
    serial.setStopBits(stopBits[currentIndex[4]]);

//    serial.setBaudRate(QSerialPort::Baud115200);
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.open(QIODevice::ReadWrite)) {
        qDebug() << "SerialPort Connected...";

    } else {
        qDebug() << "SerialPort connect failed...";
    }
}
void SerialObject::closeSerialPort(){
    if (serial.isOpen()){
        serial.close();
        qDebug() << "SerialPort Disconnected...";
    }
}
void SerialObject::sendStartPacket(){
    qDebug() << "Set Frequency...";
    radioPacket.updateFrequency(frequency[currentIndex[5]]);
    radioPacket.sendStartPacket();
}
