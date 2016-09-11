#include <QNetworkInterface>
#include "interaction.h"
#include "visionmodule.h"
#include "field.h"
#include "threadmanager.h"
Interaction::Interaction(QObject *parent) : QObject(parent) {
    connect(this,SIGNAL(fieldChange(bool)),VisionModule::instance(),SLOT(updateVisionControl(bool)),Qt::AutoConnection);
    //connect(this,SIGNAL(visionSettingChanged(quint16,const QString&,quint16,const QString,quint16))
    //        ,VisionModule::instance(),SLOT(changeSetting(quint16,const QString&,quint16,const QString&,quint16)),Qt::AutoConnection);
    //connect(this,SIGNAL(abortVision()),VisionModule::instance(),SLOT(abortSetting()),Qt::AutoConnection);
}
void Interaction::startVision(quint16 interface,const QString& address,quint16 port,const QString& senderAddress,quint16 senderPort){
    //emit visionSettingChanged(interface,address,port,senderAddress,senderPort);
    VisionModule::instance()->changeSetting(interface,address,port,senderAddress,senderPort);
    VisionModule::instance()->udpSocketConnect();
    //ThreadManager::instance()->visionThreadStart();
}
void Interaction::stopVision(){
    //ThreadManager::instance()->visionThreadStop();
    VisionModule::instance()->udpSocketDisconnect();
    //emit abortVision();
    VisionModule::instance()->abortSetting();
}
QString Interaction::getDefaultVisionAddress(){
    std::string str = SingleParams::instance()->_("vision.address");
    return QString(str.c_str());
}
quint16 Interaction::getDefaultVisionPort(){
    return quint16(SingleParams::instance()->_("vision.port"));
}
QStringList Interaction::getNetworkInterfaces(){
    const auto& interfaces = QNetworkInterface::allInterfaces();
    QStringList interfacesNames;
    for(int i = 0; i < interfaces.length(); ++i){
        interfacesNames.append(interfaces[i].name());
    }
    return interfacesNames;
}
QString Interaction::getDefaultRefereeAddress(){
    std::string str = SingleParams::instance()->_("referee.address");
    return QString(str.c_str());
}
quint16 Interaction::getDefaultRefereePort(){
    return quint16(SingleParams::instance()->_("referee.sendPort"));
}
QString Interaction::getDefaultVisionSenderAddress(){
    std::string str = SingleParams::instance()->_("vision.send.address");
    return QString(str.c_str());
}
quint16 Interaction::getDefaultVisionSenderPort(){
    return quint16(SingleParams::instance()->_("vision.send.port"));
}
