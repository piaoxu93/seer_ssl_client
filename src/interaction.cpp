#include <QNetworkInterface>
#include <QProcess>
#include <QDir>
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
Interaction::~Interaction() {
#if defined(Q_OS_WIN32)
    if (zeusProcess != nullptr) {
        if (zeusProcess->isOpen()) {
            zeusProcess->close();
        }
        delete zeusProcess;
    }
#endif
}
void Interaction::startVision(quint16 interface,const QString& address,quint16 port,const QString& senderAddress,quint16 senderPort,const QString& senderAddress2,quint16 senderPort2){
    //emit visionSettingChanged(interface,address,port,senderAddress,senderPort);
    VisionModule::instance()->changeSetting(interface,address,port,senderAddress,senderPort,senderAddress2,senderPort2);
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
quint16 Interaction::getDefaultVisionPort(bool ifBig){
    return ifBig ? quint16(SingleParams::instance()->_("vision.port.big")) :
                   quint16(SingleParams::instance()->_("vision.port.small"));
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
QString Interaction::getDefaultVisionSenderAddress2(){
    std::string str = SingleParams::instance()->_("vision.send.address2");
    return QString(str.c_str());
}
quint16 Interaction::getDefaultVisionSenderPort2(){
    return quint16(SingleParams::instance()->_("vision.send.port2"));
}

void Interaction::demoStart(bool ifBlue, quint8 index,bool ifBigField) {
#if defined(Q_OS_WIN32)
    QStringList args;
    if (ifBlue) {
        args << "cb" << "sl";
    } else {
        args << "cy" << "sr";
    }

    switch (index) {
    case 0:
		args << "dribble";
        break;
    case 1:
		args << "pass";
        break;
    case 2:
		args << "advance";
        break;
	case 3:
		args << "chipkick";
		break;
    case 4:
		args << "attack";
        break;
    case 5:
		args << "defend";
        break;
    default:
		args << "dribble";
        break;
    }
	QDir dir;
	QString path;
	if (ifBigField) {
		path = dir.currentPath() + QString("/big");
    } else {
		path = dir.currentPath() + QString("/small");
	}
	QString exe = path + QString("/ssl.exe");
	zeusProcess = new QProcess;
	zeusProcess->setWorkingDirectory(path);
	zeusProcess->start(exe, args);
#else
//    qDebug() << "FieldOption : " << ifBigField;
#endif
}

void Interaction::demoStop(bool ifBlue, quint8 index,bool ifBigField) {
#if defined(Q_OS_WIN32)
	if (zeusProcess != nullptr) {
		if (zeusProcess->isOpen()) {
			zeusProcess->close();
		}
		delete zeusProcess;
		zeusProcess = nullptr;
	}
#endif
}
