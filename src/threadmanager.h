#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "singleton.hpp"
#include "visionmodule.h"
#include "visionthread.h"
class CThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit CThreadManager(QObject *parent = 0);
    void visionThreadStart(){
//        VisionThread::instance()->resume();
//        //VisionModule::instance()->resume();
//        if(!visionControl){
//            visionControl = true;
//            visionThread->start();
//        }
    }
    void visionThreadStop(){
        //VisionThread::instance()->pause();
        //visionThread->quit();
        //VisionModule::instance()->pause();
    }
signals:

public slots:
    void errorString(QString);
private:
    QThread* visionThread;
    bool visionControl;
};
typedef Singleton<CThreadManager> ThreadManager;
#endif // THREADMANAGER_H
