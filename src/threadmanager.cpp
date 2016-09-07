#include "threadmanager.h"
#include "visionmodule.h"
#include "visionthread.h"
CThreadManager::CThreadManager(QObject *parent) : QObject(parent){
    visionControl = false;
    visionThread = new QThread();
    CVisionThread* vision = VisionThread::instance();

    vision->moveToThread(visionThread);
    connect(visionThread, SIGNAL(started()), vision, SLOT(process()));
    connect(vision, SIGNAL(finished()), visionThread, SLOT(quit()));
    connect(vision, SIGNAL(finished()), vision, SLOT(deleteLater()));
    connect(visionThread, SIGNAL(finished()), visionThread, SLOT(deleteLater()));
//    visionThread->start();
}
void CThreadManager::errorString(QString str){

}
