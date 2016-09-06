#include "QNetworkInterface"
#include "interaction.h"
#include "visionmodule.h"
#include "field.h"
Interaction::Interaction(QObject *parent) : QObject(parent) {
    connect(this,SIGNAL(fieldChange(bool)),VisionModule::instance(),SLOT(updateVisionControl(bool)));
}
void Interaction::startVision(quint16 interface,const QString& address,quint16 port,const QString& senderAddress,quint16 senderPort){
    VisionModule::instance()->changeSenderSetting(senderAddress,senderPort);
    VisionModule::instance()->start(interface,address,port);
}
void Interaction::stopVision(){
    VisionModule::instance()->stop();
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
