#ifndef INTERACTION_H
#define INTERACTION_H

#include <QObject>
#include "params.h"
class Interaction : public QObject
{
    Q_OBJECT
public:
    explicit Interaction(QObject *parent = 0);
public:
    Q_INVOKABLE int getCameraNumber() { return PARAM::CAMERA; }
    Q_INVOKABLE void startVision(quint16,const QString&,quint16,const QString&,quint16,const QString&,quint16);
    Q_INVOKABLE void stopVision();
    Q_INVOKABLE QString getDefaultVisionAddress();
    Q_INVOKABLE quint16 getDefaultVisionPort();
    Q_INVOKABLE QString getDefaultVisionSenderAddress();
    Q_INVOKABLE QString getDefaultVisionSenderAddress2();
    Q_INVOKABLE quint16 getDefaultVisionSenderPort();
    Q_INVOKABLE quint16 getDefaultVisionSenderPort2();
    Q_INVOKABLE QStringList getNetworkInterfaces();
    Q_INVOKABLE QString getDefaultRefereeAddress();
    Q_INVOKABLE quint16 getDefaultRefereePort();
    Q_INVOKABLE void demoStart(bool ifBlue,quint8 index){}
    Q_INVOKABLE void demoStop(bool ifBlue,quint8 index){}
signals:
    void fieldChange(bool);
    void visionSettingChanged(quint16 interface,const QString& address,quint16 port,const QString& senderAddress,quint16 senderPort);
    void abortVision();
public slots:
};

#endif // INTERACTION_H
